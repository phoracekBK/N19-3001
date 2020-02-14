#include "include/controler.h"


struct _controler_
{
	plc * plc_ref;
	c_queue * queue;

	uint8_t enqueue_step;
	uint8_t priority_enqueue_step;
	uint8_t dequeue_step;
	uint8_t delete_step;
	uint8_t reload_visu_step;
};



static void controler_init(controler *);
static bool controler_synchronize_visu_queue(controler *);
static uint8_t * controler_generate_visu_queue(controler *);
static uint8_t * controler_copy_first_n_glass(c_linked_list *, uint8_t *, int);
static int32_t  controler_find_glass_in_queue(c_linked_list *, glass_info *);
static void controler_read_file(FILE *, c_queue *);
static bool controler_load_from_file(controler *, char *);
static bool controler_load_from_file(controler *, char *);
static bool controler_save_to_file(controler *, char *);


static void controler_enqueue_callback(controler *);
static void controler_priority_enqueue_callback(controler *);
static void controler_dequeue_callback(controler *);
static void controler_delete_callback(controler *);
static void controler_reload_visu_callback(controler *);




controler * controler_new(char * ip_address, int rack, int slot, int db_index)
{
	controler * this = malloc(sizeof(controler));

	this->plc_ref = plc_new(ip_address, rack, slot, db_index);

	this->queue = c_queue_new(0);

	this->enqueue_step = 0;
	this->priority_enqueue_step = 0;
	this->dequeue_step = 0;
	this->delete_step = 0;
	this->reload_visu_step = 0;

	return this;
}

static void controler_read_file(FILE * input, c_queue * queue)
{
	void * buffer = malloc(glass_info_get_sizeof());

	if(fread(buffer, glass_info_get_sizeof(), 1, input) > 0)
	{
		c_queue_enqueue(queue, buffer);
		controler_read_file(input, queue);
	}
	else
	{
		free(buffer);
	}
}


static bool controler_load_from_file(controler * this, char * address)
{
	FILE * input = fopen(address, "r");

	if(input != NULL)
	{
		controler_read_file(input, this->queue);
		fclose(input);
		
		return true;
	}

	return false;
}

static void controler_write_file(FILE * output, c_linked_list * list)
{
	if(list != NULL)
	{
		glass_info * glass = c_linked_list_node_get_data(list);
		
		fwrite((void *) glass, glass_info_get_sizeof(), 1, output);
		controler_write_file(output, c_linked_list_get_next(list));
	}
}

static bool controler_save_to_file(controler * this, char * address)
{	
	FILE * output = fopen(address, "w");

	if(output != NULL)
	{
		controler_write_file(output, c_queue_get_tail(this->queue));	
		fclose(output);
		return true;
	}

	return false;
}

static void controler_init(controler * this)
{
	controler_load_from_file(this, QUEUE_FILE_PATH);

	if(controler_synchronize_visu_queue(this) == false)
		printf("%s\n", "initialize visual controler issue");

	if(model_reset_error_status(this->plc_ref) == false)
		printf("%s\n", "reset error status issue");

	if(model_reset_cmd_byte(this->plc_ref) == false)
		printf("%s\n", "reset command byte issue");

	if(model_reset_priority_input_status(this->plc_ref) == false)
		printf("%s\n", "reset priority output status issue");

	if(model_reset_done_status(this->plc_ref) == false)
		printf("%s\n", "reset done status issue");
}


static void controler_enqueue_callback(controler * this)
{
	if(this->enqueue_step == 0)
	{
		if(model_read_cmd_enqueue_status(this->plc_ref) == true)
			this->enqueue_step = 1;
	}
	else if(this->enqueue_step == 1)
	{
		glass_info * glass = model_read_glass_info(this->plc_ref, 0);

		c_queue_enqueue(this->queue, glass);
		this->enqueue_step = 2;
	}
	else if(this->enqueue_step == 2)
	{
		controler_save_to_file(this, QUEUE_FILE_PATH);
		controler_synchronize_visu_queue(this);

		model_set_done_status(this->plc_ref);
		this->enqueue_step = 3;
	}
	else if(this->enqueue_step == 3)
	{
		if(model_read_cmd_enqueue_status(this->plc_ref) == false)
			this->enqueue_step = 0;
	}
	else
	{
		this->enqueue_step = 0;
	}
}

static void controler_priority_enqueue_callback(controler * this)
{
	if(this->priority_enqueue_step == 0)
	{
		if(model_read_cmd_priority_enqueue_status(this->plc_ref) == true)
		{
			this->priority_enqueue_step = 1;
		}
	}
	else if(this->priority_enqueue_step == 1)
	{
		glass_info * glass = model_read_glass_info(this->plc_ref, 0);
		c_queue_priority_enqueue(this->queue, glass);
		this->priority_enqueue_step = 2;
	}
	else if(this->priority_enqueue_step == 2)
	{
		controler_save_to_file(this, QUEUE_FILE_PATH);
		controler_synchronize_visu_queue(this);

		model_set_priority_input_status(this->plc_ref);
		model_set_done_status(this->plc_ref);

		this->priority_enqueue_step = 3;
	}
	else if(this->priority_enqueue_step == 3)
	{
		if(model_read_cmd_priority_enqueue_status(this->plc_ref) == false)
			this->priority_enqueue_step = 0;
	}
	else
	{
		this->priority_enqueue_step = 0;
	}
}

static void controler_dequeue_callback(controler * this)
{
	if(this->dequeue_step == 0)
	{
		if(model_read_cmd_dequeue_status(this->plc_ref) == true)
			this->dequeue_step = 1;
	}	
	else if(this->dequeue_step == 1)
	{
		c_queue_dequeue_with_release(this->queue, glass_info_finalize);
		this->dequeue_step = 2;
	}
	else if(this->dequeue_step == 2)
	{
		controler_save_to_file(this, QUEUE_FILE_PATH);
		controler_synchronize_visu_queue(this);

		model_set_done_status(this->plc_ref);
		this->dequeue_step = 3;
	}
	else if(this->dequeue_step == 3)
	{
		if(model_read_cmd_dequeue_status(this->plc_ref) == false)
			this->dequeue_step = 0;
	}
	else
	{
		this->dequeue_step = 0;
	}
}

static void controler_delete_callback(controler * this)
{
	if(this->delete_step == 0)
	{
		if(model_read_cmd_delete_status(this->plc_ref) == true)
			this->delete_step = 1;
	}
	else if(this->delete_step == 1)
	{	
		glass_info * glass = model_read_glass_info(this->plc_ref, 0);

		if(glass != NULL)
		{
			int32_t glass_index = controler_find_glass_in_queue(c_queue_get_tail(this->queue), glass);

			printf("delete index: %d\n", glass_index);

			if(glass_index >= 0 && c_queue_delete_at_index_with_release(this->queue, glass_index, glass_info_finalize) == true)
				this->delete_step = 2;
			else
				this->delete_step = 3;
		}
		else
		{
			this->delete_step = 3;
		}
	}
	else if(this->delete_step == 2)
	{
		controler_save_to_file(this, QUEUE_FILE_PATH);
		controler_synchronize_visu_queue(this);

		model_set_done_status(this->plc_ref);

		this->delete_step = 4;
	}
	else if(this->delete_step == 3)
	{
		model_set_error_status(this->plc_ref);
		this->delete_step = 4;
	}
	else if(this->delete_step == 4)
	{
		if(model_read_cmd_delete_status(this->plc_ref) == false)
			this->delete_step = 0;
	}
	else
	{
		this->delete_step = 0;
	}	
}

static int32_t controler_find_glass_in_queue(c_linked_list * list, glass_info * glass)
{
	if(list != NULL)
	{
		glass_info * glass_in_queue = c_linked_list_node_get_data(list);

		if(strcmp(glass_info_get_vehicle_number(glass_in_queue), glass_info_get_vehicle_number(glass)) == 0)
			return 0;
		else
			return controler_find_glass_in_queue(c_linked_list_get_next(list), glass) + 1;
	}

	return -1;
}

static void controler_reload_visu_callback(controler * this)
{
	if(this->reload_visu_step == 0)
	{
		if(model_read_cmd_reload_visu_status(this->plc_ref) == true)
			this->reload_visu_step = 1;
	}
	else if(this->reload_visu_step == 1)
	{
		if(controler_synchronize_visu_queue(this) == true)
			this->reload_visu_step = 3;
		else
			this->reload_visu_step = 2;
	}
	else if(this->reload_visu_step == 2)
	{
		model_set_error_status(this->plc_ref);
		this->reload_visu_step = 4;
	}
	else if(this->reload_visu_step == 3)
	{
		model_set_done_status(this->plc_ref);
		this->reload_visu_step = 4;
	}
	else if(this->reload_visu_step == 4)
	{
		if(model_read_cmd_reload_visu_status(this->plc_ref) == false)
			this->reload_visu_step = 0;
	}
	else
	{
		this->reload_visu_step = 0;
	}
}



void controler_handler(controler * this)
{
	controler_init(this);

	while(true)
	{
		controler_enqueue_callback(this);
		controler_priority_enqueue_callback(this);
		controler_dequeue_callback(this);
		controler_delete_callback(this);	
		controler_reload_visu_callback(this);

		fflush(stdout);
		usleep(200000);
	}
}

void controler_finalize(controler * this)
{
	plc_finalize(this->plc_ref);
	c_queue_finalize_with_release(this->queue, glass_info_finalize);

	free(this);
}

static uint8_t * controler_copy_first_n_glass(c_linked_list * list, uint8_t * byte_array, int index)
{
	if(index < 10)
	{
		if(list != NULL)
		{
			glass_info * glass = c_linked_list_node_get_data(list);
			byte_array = model_write_glass_info_to_array(byte_array, glass, 42*index);
		}
		else
		{
			glass_info * empty_glass = glass_info_new("", "" , 0, 0);
			byte_array = model_write_glass_info_to_array(byte_array, empty_glass, 42*index);
			glass_info_finalize(empty_glass);
		}

		return controler_copy_first_n_glass(c_linked_list_get_next(list), byte_array, index + 1);
	}

	return byte_array;
}

static bool controler_synchronize_visu_queue(controler * this)
{
	uint8_t * visu_controler = controler_generate_visu_queue(this);
	bool ret_val = model_write_visu_queue(this->plc_ref, visu_controler, (int16_t) c_queue_size(this->queue));		

	free(visu_controler);

	return ret_val;
}

static uint8_t * controler_generate_visu_queue(controler * this)
{
	int visu_size = 42*10;
	uint8_t * byte_array = malloc(sizeof(uint8_t) * visu_size);
	memset(byte_array, 0, visu_size);

	return controler_copy_first_n_glass(c_queue_get_tail(this->queue), byte_array, 0);
}
















