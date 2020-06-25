#include "include/controler_parser.h"

static char * cp_get_flag(uint8_t *, uint32_t, uint8_t, char *, char * buffer);
static int * cp_get_time(uint8_t *, int);
static char * cp_get_time_string(uint8_t *, uint32_t);





char * cp_get_vehicle_number(uint8_t * buffer)
{
  return s7lib_parser_read_string(buffer, 2, 13);
}

char * cp_get_rear_window_type(uint8_t * buffer)
{
  return s7lib_parser_read_string(buffer, 18, 18);
}

char * cp_get_vehicle_model(uint8_t * buffer)
{
  char * vehicle_model = malloc(sizeof(char)*2);

  vehicle_model[0] = buffer[38];
  vehicle_model[1] = '\0';

  return vehicle_model;
}

char * cp_get_window_id(uint8_t * buffer)
{
  return c_string_format("%d", s7lib_parser_read_dword(buffer, 40));
}

char * cp_get_time_from_primer_application(uint8_t * buffer)
{
	if(s7lib_parser_read_bool(buffer, 202,1) == true)
		return cp_get_time_string(buffer, 44);	

	return c_string_init("NA");
}

char * cp_get_primer_detection_with_vision_ok(uint8_t * buffer)
{
	if(s7lib_parser_read_bool(buffer, 202,0) == true)
		return cp_get_flag(buffer, 108, 1, "PASS", "FAIL");

	return c_string_init("NA");
}

char * cp_get_bead_check_pass_fail_area(uint8_t * buffer)
{
	if(s7lib_parser_read_bool(buffer, 202,0) == true)
	{
		return c_string_format("%s-%s-%s-%s", ((buffer[106] & 0b1) ? "PASS" : "FAIL"),
												((buffer[106] & 0b10) ? "PASS" : "FAIL"),
												((buffer[106] & 0b100) ? "PASS" : "FAIL"),
												((buffer[106] & 0b1000) ? "PASS" : "FAIL"));
	}

	return c_string_init("NA");
}

char * cp_get_primer_curring_rack(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, 104));
}

char * cp_get_time_primer_flashoff_complete(uint8_t * buffer)
{
	if(s7lib_parser_read_bool(buffer, 202,1) == true)
		return cp_get_time_string(buffer, 56);

	return c_string_init("NA");
}

char * cp_get_interval_from_primering_util_gluing(uint8_t * buffer)
{
	if(s7lib_parser_read_bool(buffer, 202,1) == true)
	{
		int * primering_time = cp_get_time(buffer, 44);
		int * gluing_time = cp_get_time(buffer, 80);

		uint64_t interval = cp_time_to_seconds(gluing_time) - cp_time_to_seconds(primering_time);

		free(primering_time);
		free(gluing_time);

		return c_string_format("%ld", interval);
	}	

	return c_string_init("NA");
}

char * cp_get_time_from_last_dispense(uint8_t * buffer)
{
	return cp_get_time_string(buffer, 190);
}

char * cp_get_timestamp_of_glue_bead_application(uint8_t * buffer)
{
	return cp_get_time_string(buffer, 68);
}

char * cp_get_robot_finish_time_glue_bead_application(uint8_t * buffer)
{

	return cp_get_time_string(buffer, 80);
}

char * cp_get_interval_from_glue_application_util_output(uint8_t * buffer)
{
	int * time_from_start_glue_application = cp_get_time(buffer, 68);
	int * assembly_time = cp_get_time(buffer, 92);

	uint64_t interval = cp_time_to_seconds(assembly_time) - cp_time_to_seconds(time_from_start_glue_application);

	free(time_from_start_glue_application);
	free(assembly_time);

	return c_string_format("%ld", interval);
}

char * cp_get_barrel_expire_ok_a(uint8_t * buffer)
{
	return cp_get_flag(buffer, 149,0, "nok", "ok");
}

char * cp_get_adhesive_batch_id_a(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, 110, 16);
}

char * cp_get_adhesive_serial_a(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, 128, 16);
}

char * cp_get_material_dispensed_a(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 224)*10.0)/10.0);
}

char * cp_get_pistol_temperature_min_value(uint8_t * buffer)
{
	return c_string_init("35.0");
}

char * cp_get_pistol_temperature(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 216)*10.0)/10.0);
}

char * cp_get_pistol_temperature_max_value(uint8_t * buffer)
{
	return c_string_init("65.0");
}

char * cp_get_pot_temperature_min_value_a(uint8_t * buffer)
{
	return c_string_init("35.0");
}

char * cp_get_pot_temperature_actual_a(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 220)*10.0)/10.0);
}

char * cp_get_pot_temperature_max_value_a(uint8_t * buffer)
{
	return c_string_init("65.0");
}

char * cp_get_barrel_expire_ok_b(uint8_t * buffer)
{
	return cp_get_flag(buffer, 189, 0, "nok", "ok");
}

char * cp_get_adhesive_batch_b(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, 150, 16);
}

char * cp_get_adhesive_serial_b(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, 168, 16);
}

char * cp_get_material_dispensed_b(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 228)*10.0)/10.0);
}

char * cp_get_application_ratio(uint8_t * buffer)
{
	return c_string_format("%.01f:%.01f", floor(s7lib_parser_read_real(buffer, 208)*10.0)/10.0, floor(s7lib_parser_read_real(buffer, 212)*10.0)/10.0);
}

char * cp_get_mixer_tube_life(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_dword(buffer, 204));
}

char * cp_get_robot_completed_cycle_without_fault(uint8_t * buffer)
{
	return cp_get_flag(buffer, 108, 4, "true", "false");
}

char * cp_get_dispense_unit_completed_cycle_without_fault(uint8_t * buffer)
{
	return cp_get_flag(buffer, 108, 5, "true", "false");
}

char * cp_get_rotary_unit_completed_cycle_without_fault(uint8_t * buffer)
{
	return cp_get_flag(buffer, 108, 6, "true", "false");
}

char * cp_get_adhesive_application_complete_summary(uint8_t * buffer)
{
	return cp_get_flag(buffer, 108, 7, "true", "false");
}

char * cp_get_bead_check_laser_sensor(uint8_t * buffer)
{
	return cp_get_flag(buffer, 108, 2, "PASS", "FAIL");
}

char * cp_get_humidity(uint8_t * buffer)
{
  return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 232)*10.0)/10.0);
}

char * cp_get_ambient_temperature(uint8_t * buffer)
{
  return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 236)*10.0)/10.0);
}


static int * cp_get_time(uint8_t * buffer, int byte_index)
{
  int * time = malloc(sizeof(int)*6);

  //year
  time[0] = s7lib_parser_read_int(buffer, byte_index);

  //month
  time[1] = buffer[byte_index+2];

  // day
  time[2] = buffer[byte_index+3];

  //hour
  time[3] = buffer[byte_index+5];

  //minute
  time[4] = buffer[byte_index+6];

  //second
  time[5] = buffer[byte_index+7];

  return time;
}

static char * cp_get_time_string(uint8_t * buffer, uint32_t byte_index)
{
  int * time = cp_get_time(buffer, byte_index);
  char * time_string =  c_string_format("%d-%02d-%02d %02d:%02d:%02d", time[0], time[1], time[2], time[3], time[4], time[5]);

  free(time);

  return time_string;
}

uint64_t cp_time_to_seconds(int * time)
{
    return ((uint64_t)(((((time[0]>0) ? time[0]-1970 : 0))*31556925.216) + (time[1]*30.4368499*86400) + (time[2]*86400) + (time[3]*3600) + (time[4]*60) + time[5]));
}

static char * cp_get_flag(uint8_t * buffer, uint32_t byte_index, uint8_t bit_index, char * option_ok, char * option_nok)
{
  if(s7lib_parser_read_bool(buffer, byte_index, bit_index) == true)
    return c_string_init(option_ok);
  else
    return c_string_init(option_nok);
}


