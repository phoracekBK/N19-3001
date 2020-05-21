#include "include/controler_parser.h"

static char * controler_parser_get_flag(uint8_t *, uint32_t, uint8_t, char *, char *);
static int * controler_parser_get_time(uint8_t *, int);
static char * controler_parser_get_time_string(uint8_t *, uint32_t);


char * controler_parser_get_window_id(uint8_t * buffer)
{
  return c_string_format("%d", s7lib_parser_read_dword(buffer, 40));
}

char * controler_parser_get_vehicle_number(uint8_t * buffer)
{
  return s7lib_parser_read_string(buffer, 2, 13);
}

char * controler_parser_get_rear_window_type(uint8_t * buffer)
{
  return s7lib_parser_read_string(buffer, 18, 18);;
}

char * controler_parser_get_vehicle_model(uint8_t * buffer)
{
  char * vehicle_model = malloc(sizeof(char)*2);

  vehicle_model[0] = buffer[38];
  vehicle_model[1] = '\0';

  return vehicle_model;
}

static int * controler_parser_get_time(uint8_t * buffer, int byte_index)
{
  int * time = malloc(sizeof(int)*6);

  //month
  time[0] = buffer[byte_index+2];

  // day
  time[1] = buffer[byte_index+3];

  //year
  time[2] = s7lib_parser_read_int(buffer, byte_index);

  //hour
  time[3] = buffer[byte_index+5];

  //minute
  time[4] = buffer[byte_index+6];

  //second
  time[5] = buffer[byte_index+7];

  return time;
}

static char * controler_parser_get_time_string(uint8_t * buffer, uint32_t byte_index)
{
  int * time = controler_parser_get_time(buffer, byte_index);
  char * time_string =  c_string_format("%02d.%02d.%d-%02d:%02d:%02d", time[0], time[1], time[2], time[3], time[4], time[5]);

  free(time);

  return time_string;
}

char * controler_parser_get_primer_application_time(uint8_t * buffer)
{
  return  controler_parser_get_time_string(buffer, 44);
}

uint64_t controler_parser_time_to_seconds(int * time)
{
    return ((uint64_t) ((time[0]*30.4368499) + (time[1]*86400) + ((((time[2]>0) ? time[2]-1970 : 0))*31536000) + (time[3]*3600) + (time[4]*60) + time[5]));
}

char * controler_parser_get_interval_from_primer_application(uint8_t * buffer)
{
  int * primer_application_time = controler_parser_get_time(buffer, 44);
  int * glue_application_time = controler_parser_get_time(buffer, 68);

  uint64_t glue_application_time_in_seconds = controler_parser_time_to_seconds(glue_application_time);
  uint64_t primer_application_time_in_seconds = controler_parser_time_to_seconds(primer_application_time);

  uint32_t interval_in_seconds = glue_application_time_in_seconds - primer_application_time_in_seconds;

  free(primer_application_time);
  free(glue_application_time);

  return c_string_format("%d", interval_in_seconds);
}

char * controler_parser_get_glue_application_time(uint8_t * buffer)
{
  return controler_parser_get_time_string(buffer, 56);
}

char * controler_parser_get_interval_from_glue_application(uint8_t * buffer)
{
  int * glue_application_time = controler_parser_get_time(buffer, 56);
  int * assembly_time = controler_parser_get_time(buffer, 68);

  uint64_t assembly_time_in_seconds = controler_parser_time_to_seconds(assembly_time);
  uint64_t primer_application_time_in_seconds = controler_parser_time_to_seconds(glue_application_time);

  uint32_t interval_in_seconds = assembly_time_in_seconds - primer_application_time_in_seconds;

  free(glue_application_time);
  free(assembly_time);

  return c_string_format("%d", interval_in_seconds);
}

char * controler_parser_get_assembly_time(uint8_t * buffer)
{
  return  controler_parser_get_time_string(buffer, 68);
}

static char * controler_parser_get_flag(uint8_t * buffer, uint32_t byte_index, uint8_t bit_index, char * option_ok, char * option_nko)
{
  if(s7lib_parser_read_bool(buffer, byte_index, bit_index) == true)
    return c_string_init(option_ok);
  else
    return c_string_init(option_nko);
}

char * controler_parser_get_primer_inspection_bypass_flag(uint8_t * buffer)
{
  return controler_parser_get_flag(buffer, 80, 0, "true", "false");
}

char * controler_parser_get_glue_aplication_inspection_result_flag(uint8_t * buffer)
{
  return controler_parser_get_flag(buffer, 80, 1, "pass", "fail");
}

char * controler_parser_get_glue_applied_flag(uint8_t * buffer)
{
  return controler_parser_get_flag(buffer, 80, 2, "true", "false");
}

char * controler_parser_get_glass_delete_flag(uint8_t * buffer)
{
  return controler_parser_get_flag(buffer, 80, 3, "true", "false");
}

char * controler_parser_get_glue_A_batch_number(uint8_t * buffer)
{
  return s7lib_parser_read_string(buffer, 82, 16);
}

char * controler_parser_get_glue_B_batch_number(uint8_t * buffer)
{
  return s7lib_parser_read_string(buffer, 104, 16);
}

char * controler_parser_get_glue_A_expiration_date(uint8_t * buffer)
{
  uint16_t year = s7lib_parser_read_int(buffer, 100);
  uint8_t month = buffer[102];

  return c_string_format("%d-%02d", year, month);
}

char * controler_parser_get_glue_B_expiration_date(uint8_t * buffer)
{
  uint16_t year = s7lib_parser_read_int(buffer, 122);
  uint8_t month = buffer[124];

  return c_string_format("%d-%02d", year, month);
}

char * controler_parser_get_glue_application_ratio(uint8_t * buffer)
{
  float application_ratio_a = floorf(s7lib_parser_read_real(buffer, 126) * 100) / 100;
  float application_ratio_b = floorf(s7lib_parser_read_real(buffer, 130) * 100) / 100;

  return c_string_format("%.01f:%.01f", application_ratio_a, application_ratio_b);
}

char * controler_parser_get_glue_temperature_during_application(uint8_t * buffer)
{
  return c_string_format("%.01f", floorf(s7lib_parser_read_real(buffer, 134) * 100) / 100);
}

char * controler_parser_get_applied_glue_amount(uint8_t * buffer)
{
  return c_string_format("%.01f", floorf(s7lib_parser_read_real(buffer, 138) * 100) / 100);
}

char * controler_parser_get_humidity(uint8_t * buffer)
{
  return c_string_format("%.01f", floorf(s7lib_parser_read_real(buffer, 142) * 100) / 100);
}

char * controler_parser_get_ambient_temperature(uint8_t * buffer)
{
  return c_string_format("%.01f", floorf(s7lib_parser_read_real(buffer, 146) * 100) / 100);
}
