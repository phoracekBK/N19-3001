/*
 ============================================================================
 Name        : webasto_mes_simulator.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/view/view.h"
#include "include/model/model.h"
#include "include/controler/controler.h"

#define DEBUG false

#if DEBUG == true
#include "test/unit_test.h"
#endif


int main(int argv, char ** argc)
{
#if DEBUG == true
	webasto_mes_simulator_test_run();
#else
	model * model_ref = model_new();
	controler * controler_ref = controler_new(model_ref);
	view * view_ref = view_new(&argv, &argc, controler_ref, model_ref);

	view_main(view_ref);

	model_finalize(model_ref);
	controler_finalize(controler_ref);
	view_finalize(view_ref);
#endif

	return EXIT_SUCCESS;
}
