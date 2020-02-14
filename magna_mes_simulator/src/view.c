#include "include/view.h"

struct _view_
{
	model * model_ref;
	controler * controler_ref;
	c_trie * trie;

	bool run_condition;
};

#define CMD_SEND_GLASS_TO_PRODUCTION "enqueue"
#define CMD_DELETE_GLASS_FROM_PRODUCTION "delete"
#define CMD_SEND_GLASS_TO_GLUING "glue"
#define CMD_PRODUCTION_GLASS_QUEUEU_SIZE "size"
#define CMD_PRODUCTION_GLASS_QUEUE_LIST "list"
#define CMD_CLEAR_SCREEN "clear"
#define CMD_EXIT "exit"
#define CMD_HELP "help"



static void view_release_cmd_array(char **);
static void view_cli_init(view *);
static void view_print_glass_content(char *, glass_info *);
static void view_release_cmd_array(char **);
static void view_send_priority_glass_to_production(view *, glass_info *);
static void view_send_glass_to_production(view *, glass_info *);
static glass_info * view_prepare_glass(view *, char **);


void view_clear_terminal_callback(void *, char **);
static void view_exit_request_callback(void *, char **);
static void view_print_help_prompt_callback(void *, char **);
static void view_send_glass_to_production_callback(void *, char **);
static void view_delete_glass_from_production_callback(void *, char **);
static void view_send_glass_to_gluing_callback(void *, char **);
static void view_production_glass_queue_size_callback(void *, char **);
static void view_unknown_command_callback(void *, char **);
static void view_production_glass_queue_list_callback(void *, char **);


view * view_new(model * model_ref, controler * controler_ref)
{
	view * this = malloc(sizeof(view));

	this->model_ref = model_ref;
	this->controler_ref = controler_ref;

	this->trie = c_trie_new(view_unknown_command_callback, NULL);

	this->run_condition = true;
	view_cli_init(this);


	return this;
}

void view_clear_terminal_callback(void * param, char ** cmd_array)
{
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    	system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
   		 system("cls");
    #endif
}

static void view_unknown_command_callback(void * param, char ** cmd_array)
{
	printf("Unknown command: %s", cmd_array[0]);
}


static void view_exit_request_callback(void * param, char ** cmd_array)
{
	((view*) param)->run_condition = false;
	printf("Program exiting....\n");
}

static void view_print_help_prompt_callback(void * param, char ** cmd_array)
{
	printf("mes server simulator - command list:\n\n");

	printf("	%s 	priority <vehicle_number> <rear_window_type> <vehicle_model>\n", CMD_SEND_GLASS_TO_PRODUCTION);
	printf("	%s 	<vehicle_number> <rear_window_type> <vehicle_model>\n", CMD_SEND_GLASS_TO_PRODUCTION);
	printf("	%s 	priority	\n", CMD_SEND_GLASS_TO_PRODUCTION);
	printf("	%s\n", CMD_SEND_GLASS_TO_PRODUCTION);

	printf("	%s 		<vehicle_number>\n", CMD_DELETE_GLASS_FROM_PRODUCTION);
	printf("	%s\n", CMD_SEND_GLASS_TO_GLUING);
	printf("	%s\n", CMD_PRODUCTION_GLASS_QUEUEU_SIZE);
	printf("	%s\n", CMD_PRODUCTION_GLASS_QUEUE_LIST);
	printf("	%s\n", CMD_CLEAR_SCREEN);
	printf("	%s\n", CMD_HELP);
	printf("	%s\n", CMD_EXIT);
}

static void view_print_glass_content(char * msg, glass_info * glass)
{
	char * vehicle_number = glass_info_get_vehicle_number(glass);
	char * rear_window_type = glass_info_get_rear_window_type(glass);
	char vehicle_model = glass_info_get_vehicle_model(glass); 

	printf("\n%s %s - %s - %c\n", msg, vehicle_number, rear_window_type, vehicle_model);
}


static glass_info * view_prepare_glass(view * this, char ** cmd_array)
{
	if(cmd_array[1] != NULL && cmd_array[2] != NULL && cmd_array[3] != NULL)
	{
		return glass_info_new(cmd_array[1], cmd_array[2], cmd_array[3][0], 0);
	}
	else
	{
		glass_info * glass = model_get_default_glass_info(this->model_ref);

		view_print_glass_content("Generating default glass:", glass);
		return glass;
	}
}

static void view_send_glass_to_production_base(view * this, glass_info * glass, bool(*controler_fce)(controler *, glass_info *))
{
	if(controler_fce(this->controler_ref, glass) == true)
	{
		printf("\nSuccessfully finished");
	}
	else
	{
		glass_info_finalize(glass);
		printf("\nFinished with error!");		
	}
}

static void view_send_priority_glass_to_production(view * this, glass_info * glass)
{
	view_send_glass_to_production_base(this, glass, controler_move_priority_glass_to_production_request);
}

static void view_send_glass_to_production(view * this, glass_info * glass)
{
	view_send_glass_to_production_base(this, glass, controler_move_glass_to_production_request);
}

static void view_send_glass_to_production_callback(void * this, char ** cmd_array)
{
	printf("Processing...");
	fflush(stdout);

	if(cmd_array[1] != NULL && strcmp(cmd_array[1], "priority") == 0)
	{
		glass_info * glass = view_prepare_glass(this, cmd_array+1);
		view_send_priority_glass_to_production(this, glass);
	}
	else
	{
		glass_info * glass = view_prepare_glass(this, cmd_array);
		view_send_glass_to_production(this, glass);
	}
}

glass_info * view_get_deleted_glass(view * this, char * param)
{
	if(c_string_is_numeric(param) == true)
		return model_glass_queue_peek(this->model_ref, c_string_to_int(param));
	else
		return glass_info_new(param, "", 0, 0);
}

static void view_delete_glass_from_production_callback(void * this, char ** cmd_array)
{
	if(cmd_array[1] != NULL)
	{
		printf("Processing...");
		fflush(stdout);

		glass_info * glass = view_get_deleted_glass(this, cmd_array[1]);

		if(glass == NULL)
				printf("null\n");
		else
			printf("%s\n", glass_info_get_vehicle_number(glass));

		if(glass != NULL && controler_delete_glass_from_production_request(((view*) this)->controler_ref, glass) == true)
			printf("\nSuccessfully finished");
		else
			printf("\nFinished with error!");
	}
	else
	{
		printf("\nBad input parameter! Vehicle number expected.");
	}
}

static void view_send_glass_to_gluing_callback(void * this, char ** cmd_array)
{
	printf("Processing...");
	fflush(stdout);

	glass_info * glass = model_get_next_glass_in_order(((view*) this)->model_ref);

	if(glass != NULL)
	{
		if(controler_send_glass_to_gluing_request(((view*) this)->controler_ref, glass) == true)
		{
			printf("\nSuccessfully finished");
		}
		else
		{
			printf("\nFinished with error!");	
		}
	}
	else
	{
		printf("\nQueue with glass in production is empty!");
	}
}

static void view_production_glass_queue_size_callback(void * this, char ** cmd_array)
{
	printf("Queue size: %d", model_get_glass_in_production_queue_size(((view*) this)->model_ref));
}


static void view_print_out_glass_queue(c_linked_list * tail, int index)
{
	if(tail != NULL)
	{
		glass_info * glass = c_linked_list_node_get_data(tail);

		char * str_index = c_string_format("%d. ", index);
		view_print_glass_content(str_index, glass);
		free(str_index);

		view_print_out_glass_queue(c_linked_list_get_next(tail), index +1);
	}
}


static void view_production_glass_queue_list_callback(void * this, char ** cmd_array)
{
	printf("Glass in production queue content:\n");
	view_print_out_glass_queue(model_get_glass_list(((view *) this)->model_ref), 0);
}



static void view_cli_init(view * this)
{
	c_trie_connect(this->trie, CMD_SEND_GLASS_TO_PRODUCTION, view_send_glass_to_production_callback, this);
	c_trie_connect(this->trie, CMD_DELETE_GLASS_FROM_PRODUCTION, view_delete_glass_from_production_callback, this);
	c_trie_connect(this->trie, CMD_SEND_GLASS_TO_GLUING, view_send_glass_to_gluing_callback, this);
	c_trie_connect(this->trie, CMD_PRODUCTION_GLASS_QUEUEU_SIZE, view_production_glass_queue_size_callback, this);
	c_trie_connect(this->trie, CMD_PRODUCTION_GLASS_QUEUE_LIST, view_production_glass_queue_list_callback, this);
	c_trie_connect(this->trie, CMD_CLEAR_SCREEN, view_clear_terminal_callback, this);
	c_trie_connect(this->trie, CMD_HELP, view_print_help_prompt_callback, this);
	c_trie_connect(this->trie, CMD_EXIT, view_exit_request_callback, this);
}

static void view_release_cmd_array(char ** cmd_array)
{
	if(cmd_array != NULL)
	{
		if(*cmd_array != NULL)
		{
			free(*cmd_array);
			view_release_cmd_array(++cmd_array);
		}
	}
}

static char * view_read_input(char * buffer, int max)
{
	char input_character = getchar();

	if(input_character != '\n' && max > 0)
	{
		*buffer = input_character;
		return view_read_input(buffer+1, max);
	}
	else
	{
		*buffer = 0;
	}

	return buffer;
}

void view_cli_run(view * this)
{
	char input_buffer[128];

	while(this->run_condition == true)
	{
		printf("\n>>> ");
		fflush(stdout);

		view_read_input(input_buffer, 128);
		char ** cmd_array = c_string_split(input_buffer, " ");

		c_trie_parse(this->trie, cmd_array);

		view_release_cmd_array(cmd_array);
		free(cmd_array);
	}
}

void view_finalize(view * this)
{
	free(this);
}

