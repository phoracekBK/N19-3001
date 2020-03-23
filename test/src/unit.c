#include "include/unit.h"
#include "include/plc.h"


struct _unit_
{
	plc * plc_ref;
};




unit* unit_new(char * IPAddress, int Rack, int Slot, int DB_index)
{
	unit * this = malloc(sizeof(unit));

	this->plc_ref = plc_new(IPAddress, Rack, Slot, DB_index);

	return this;
}

static uint16_t unit_get_ts_size(unit * this)
{
	return plc_read_int(this->plc_ref, 2);	
}

bool unit_initialize_plcut(unit * this)
{
	uint16_t ts_size = unit_get_ts_size(this);
	uint32_t address = 4;

	for(int i = 0; i < ts_size; i++)
	{
		uint16_t tc_size = plc_read_int(this->plc_ref, address + 66);

		for(int j = 0; j < tc_size; j ++)
		{
			uint8_t byte_array[] = {0, 0, 255};

			if(plc_write(this->plc_ref, byte_array, address+66+(68 + 70*j), 3) == false)
				return false;
		}

		address += 68 + 70*tc_size;
	}

	return true;
}

bool unit_set_plcut_enabled(unit * this)
{
	return plc_write_bool(this->plc_ref, 0, 0, true);
}

bool unit_set_plcut_disabled(unit * this)
{
	return plc_write_bool(this->plc_ref, 0, 0, false);
}

bool unit_run_unit_test(unit * this)
{
	uint16_t ts_size = unit_get_ts_size(this);
	uint32_t address = 4;

	for(int i = 0; i < ts_size; i++)
	{
		uint16_t tc_size = plc_read_int(this->plc_ref, address + 66);
		char * ts_name = plc_read_string(this->plc_ref, address, 64);

		if(ts_name != NULL)
		{
			printf("\nTest suite: %s\n", ts_name);
			free(ts_name);
		}

		for(int j = 0; j < tc_size; j ++)
		{
			uint8_t byte_on = 1;
			uint8_t byte_off = 0;
			uint8_t * byte_executed = NULL;
			uint8_t * byte_evaluation = NULL;

			char * tc_name = plc_read_string(this->plc_ref, address+68+(70*j), 64);

			if(tc_name != NULL)
			{
				printf("Executing test case	%s: ", tc_name);
				fflush(stdout);
				free(tc_name);
			}

			if(plc_write(this->plc_ref, &byte_on, address+68+(66 + 70*j), 1) == false)
				return false;

			while((byte_executed = plc_read(this->plc_ref, address+68+(66 + 70*j + 1), 1)) != NULL)
			{
				if(*byte_executed > 0)
				{
					free(byte_executed);
					break;
				}
				else
				{
					free(byte_executed);
				}

				usleep(1000);
			}
			
			byte_evaluation = plc_read(this->plc_ref, address+68+(66 + 70*j+2), 1);

			if(byte_evaluation != NULL)
			{
				if(*byte_evaluation == 0)
					printf("PASS\n");
				else
					printf("FAIL (%d)\n", *byte_evaluation);

				free(byte_evaluation);
			}
			else
			{
				printf("Error");
			}

		
			if(plc_write(this->plc_ref, &byte_off, address+68+(66 + 70*j), 1) == false)
				return false;

			fflush(stdout);
		}

		address += 68 + 70*tc_size;
	}

	return true;
}

void unit_finalize(unit * this)
{
	plc_finalize(this->plc_ref);
	free(this);
}












