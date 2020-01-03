/*
 * view.c
 *
 *  Created on: 12. 12. 2019
 *      Author: HORACEK
 */


#include "include/view/view.h"
#include <aclib.h>
#include <stdio.h>

#define INPUT_BUFFER_SIZE 512

#define VIEW_CMD_EXIT "exit"
#define VIEW_CMD_HELP "help"

struct _view_
{
	model * model_ref;
	controler * controler_ref;

	bool run;

	char * input_buffer;
	char * output_buffer;
};


static void view_process_input_query(view *);
static void view_clean_up_query(view *);
static void view_read_input(view *);
static void view_write_output(view *);
static char * view_read_command(char *, char *, int);

static void view_cmd_exit_callback(view *);
static void view_cmd_help_callback(view *);


view * view_new(int * argv, char*** argc, controler * controler_ref, model * model_ref)
{
	view * this = malloc(sizeof(view));

	this->model_ref = model_ref;
	this->controler_ref = controler_ref;

	this->run = true;
	this->input_buffer = malloc(sizeof(char)*(INPUT_BUFFER_SIZE+1));

	return this;
}

void view_main(view * this)
{
	do
	{
		view_read_input(this);
		view_process_input_query(this);
		view_write_output(this);
	}
	while(this->run == true);
}

void view_main_exit(view * this)
{
	controler_main_loop_exit(this->controler_ref);

	while(controler_main_loop_is_runnig(this->controler_ref) == true)
	{
		usleep(10000);
	}

	this->run = false;
}

void view_finalize(view * this)
{
	free(this);
}

static void view_clean_up_query(view * this)
{
	free(this->output_buffer);
	this->output_buffer = NULL;
}

static void view_read_input(view * this)
{
	fputs("> ", stdout);
	fflush(stdout);
	fgets(this->input_buffer,INPUT_BUFFER_SIZE, stdin);
}

static void view_write_output(view * this)
{
	if(this->output_buffer != NULL)
	{
		fputs(this->output_buffer, stdout);
		fflush(stdout);
		view_clean_up_query(this);
	}
}

static void view_cmd_exit_callback(view * this)
{
	view_main_exit(this);
	this->output_buffer = c_string_init("Ridici smycka vypnuta\nHlavni smycka vypnuta\nProgram vypnut...\n");
}

static void view_cmd_help_callback(view * this)
{
	this->output_buffer = c_string_init("Napoveda:\nexit - vypnout program\nhelp - vypise tuto napovedu\n");
}

static char * view_read_command(char * command, char * input_buffer, int input_buffer_index)
{
	if(input_buffer[input_buffer_index] != 0 && input_buffer[input_buffer_index] != ' ' && input_buffer[input_buffer_index] != '\n')
	{
		command = realloc(command, sizeof(char)*(input_buffer_index+1));

		command[input_buffer_index] = input_buffer[input_buffer_index];
		return view_read_command(command, input_buffer, input_buffer_index+1);
	}

	return command;
}


static void view_process_input_query(view * this)
{
	char * command = view_read_command(NULL, this->input_buffer, 0);

	if(strcmp(command, VIEW_CMD_EXIT) == 0)
		view_cmd_exit_callback(this);
	else if(strcmp(command, VIEW_CMD_HELP) == 0)
		view_cmd_help_callback(this);
	else
		this->output_buffer = c_string_format("Unknown command: %s\n", command);

	if(command != NULL)
		free(command);
}
