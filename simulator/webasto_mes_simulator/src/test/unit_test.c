/*
 * unit_test.c
 *
 *  Created on: 12. 12. 2019
 *      Author: HORACEK
 */


#include "unit_test.h"

#include "controler_test.h"
#include "model_test.h"
#include "view_test.h"

#include <aclib.h>


cu * cu_reference;


static void unit_test_setup();
static void unit_test_run();
static void unit_test_print_failed();
static void unit_test_clean_up();


void webasto_mes_simulator_test_run()
{
	unit_test_setup();
	unit_test_run();
	unit_test_print_failed();
	unit_test_clean_up();
}




static void unit_test_setup()
{
	cu_reference = cu_new();

	//cu_add_test_suite(cu_reference, NULL);
}

static void unit_test_run()
{
	int test_suite_number = cu_get_tc_number(cu_reference);

	printf("Initializing....\nRun");

	char * msg = "";

	while(cu_run_next(cu_reference) == true)
	{
		int test_case_index = cu_get_tc_iterator(cu_reference);

		printf(msg);
	}
}

static void unit_test_print_failed()
{

}

static void unit_test_clean_up()
{

}
