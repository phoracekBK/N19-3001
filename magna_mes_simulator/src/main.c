#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <c_string.h>
#include <c_trie.h>


#include "include/model.h"
#include "include/view.h"
#include "include/controler.h"



void default_callback(void * param, char ** cmd)
{
	printf("Unknown command: %s\n", cmd[0]);

}

void callback_exit(void * param, char ** cmd)
{
	printf("exit command -> %s\n", cmd[0]);
}

void callback_print(void * param, char ** cmd)
{
	printf("print command -> %s\n", cmd[0]);
}


int main(int argv, char ** argc)
{
	model * model_ref = model_new();
	controler * controler_ref = controler_new(model_ref);
	view * view_ref = view_new(model_ref, controler_ref);

	view_cli_run(view_ref);


	model_finalize(model_ref);
	controler_finalize(controler_ref);
	view_finalize(view_ref);	

	return 0;
}
