#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <c_string.h>

#include "include/config.h"
#include "include/unit.h"

int main(int argv, char ** argc)
{
	unit * unit_ref = NULL;

	if(argv == 5)
		unit_ref = unit_new(argc[1], c_string_to_int(argc[2]), c_string_to_int(argc[3]), c_string_to_int(argc[4]));
	else
		unit_ref = unit_new(PLC_IP_ADDRESS, RACK, SLOT, DB_INDEX);

	if((unit_initialize_plcut(unit_ref) == true) && (unit_set_plcut_enabled(unit_ref) == true))
	{
		if(unit_run_unit_test(unit_ref) == true)
			printf("\nUnit test processed successfully.\n");
		else
			printf("\nUnit test processing error!\n");

		if(unit_set_plcut_disabled(unit_ref) == false)
			printf("Can't dissable plcut!\n");
	}
	else
	{
		printf("Communication error, program exit....\n");
	}

	unit_finalize(unit_ref);

	return 0;
}
