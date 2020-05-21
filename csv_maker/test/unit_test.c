#include <stdbool.h>
#include <s7lib_parser.h>

#include "unit_test.h"
#include "../src/include/controler_parser.h"
#include "../src/include/config.h"

static void controler_parser_get_window_id_test();
static void controler_parser_get_vehicle_number_test();
static void controler_parser_get_rear_window_type_test();
static void controler_parser_get_vehicle_model_test();
static void controler_parser_get_primer_application_time_test();
static void controler_parser_get_interval_from_primer_application_test();
static void controler_parser_get_glue_application_time_test();
static void controler_parser_get_interval_from_glue_application_test();
static void controler_parser_get_assembly_time_test();
static void controler_parser_get_primer_inspection_bypass_flag_test();
static void controler_parser_get_glue_aplication_inspection_result_flag_test();
static void controler_parser_get_glue_applied_flag_test();
static void controler_parser_get_glass_delete_flag_test();
static void controler_parser_time_to_seconds_test();
static void controler_parser_get_glue_A_batch_number_test();
static void controler_parser_get_glue_B_batch_number_test();
static void controler_parser_get_glue_A_expiration_date_test();
static void controler_parser_get_glue_B_expiration_date_test();
static void controler_parser_get_glue_application_ratio_test();
static void controler_parser_get_glue_temperature_during_application_test();
static void controler_parser_get_applied_glue_ammount_test();
static void controler_parser_get_humidity_test();
static void controler_parser_get_ambient_temperature_test();
static uint8_t * ut_get_buffer();



void unit_test_run()
{
  controler_parser_get_window_id_test();
  controler_parser_get_vehicle_number_test();
  controler_parser_get_rear_window_type_test();
  controler_parser_get_vehicle_model_test();
  controler_parser_get_primer_application_time_test();
  controler_parser_get_interval_from_primer_application_test();
  controler_parser_get_glue_application_time_test();
  controler_parser_get_interval_from_glue_application_test();
  controler_parser_get_assembly_time_test();
  controler_parser_get_primer_inspection_bypass_flag_test();
  controler_parser_get_glue_aplication_inspection_result_flag_test();
  controler_parser_get_glue_applied_flag_test();
  controler_parser_get_glass_delete_flag_test();
  controler_parser_time_to_seconds_test();
  controler_parser_get_glue_A_batch_number_test();
  controler_parser_get_glue_B_batch_number_test();
  controler_parser_get_glue_A_expiration_date_test();
  controler_parser_get_glue_B_expiration_date_test();
  controler_parser_get_glue_application_ratio_test();
  controler_parser_get_glue_temperature_during_application_test();
  controler_parser_get_applied_glue_ammount_test();
  controler_parser_get_humidity_test();
  controler_parser_get_ambient_temperature_test();
}

static uint8_t * ut_get_buffer()
{
  uint8_t * buffer = malloc(sizeof(char)*DB_SIZE);
  memset(buffer, 0, DB_SIZE);

  return buffer;
}



static void controler_parser_get_window_id_test()
{
  printf("controler_parser_get_window_id: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_dword(buffer, 40, 30);

  char * id_string = controler_parser_get_window_id(buffer);

  if(strcmp(id_string, "30") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(id_string);
  free(buffer);
}

static void controler_parser_get_vehicle_number_test()
{
  printf("controler_parser_get_vehicle_number: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_string(buffer, 2, "qwertzuiopasd", 13);

  char * vehicle_number_string = controler_parser_get_vehicle_number(buffer);

  if(strcmp(vehicle_number_string, "qwertzuiopasd") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(vehicle_number_string);
}

static void controler_parser_get_rear_window_type_test()
{
  printf("controler_parser_get_rear_window_type: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_string(buffer, 18, "qwertzuiopasdYXCVB", 18);

  char * rear_window_type_string = controler_parser_get_rear_window_type(buffer);

  if(strcmp(rear_window_type_string, "qwertzuiopasdYXCVB") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(rear_window_type_string);
}

static void controler_parser_get_vehicle_model_test()
{
  printf("controler_parser_get_vehicle_model: ");
  uint8_t * buffer = ut_get_buffer();
  buffer[38] = '7';

  char * string_model = controler_parser_get_vehicle_model(buffer);

  if(strcmp(string_model, "7") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(string_model);
}

static void controler_parser_get_primer_application_time_test()
{
  printf("controler_parser_get_primer_application_time: ");

  uint8_t * buffer = ut_get_buffer();

  buffer = s7lib_parser_write_int(buffer, 44, 2020);
  buffer[46] = 5;
  buffer[47] = 11;
  buffer[49] = 7;
  buffer[50] = 44;
  buffer[51] = 12;

  char * string_time = controler_parser_get_primer_application_time(buffer);

  if(strcmp(string_time, "05.11.2020-07:44:12") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(string_time);
}

static void controler_parser_get_interval_from_primer_application_test()
{
  printf("controler_parser_get_interval_from_primer_application: ");

  uint8_t * buffer = ut_get_buffer();

  buffer = s7lib_parser_write_int(buffer, 44, 2020);
  buffer[46] = 5;
  buffer[47] = 11;
  buffer[49] = 7;
  buffer[50] = 44;
  buffer[51] = 12;

  buffer = s7lib_parser_write_int(buffer, 68, 2020);
  buffer[70] = 5;
  buffer[71] = 11;
  buffer[73] = 7;
  buffer[74] = 45;
  buffer[75] = 12;

  char * string_interval = controler_parser_get_interval_from_primer_application(buffer);

  if(strcmp(string_interval, "60") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(string_interval);
}

static void controler_parser_get_glue_application_time_test()
{
  printf("controler_parser_get_glue_application_time: ");

  uint8_t * buffer = ut_get_buffer();

  buffer = s7lib_parser_write_int(buffer, 56, 2020);
  buffer[58] = 5;
  buffer[59] = 11;
  buffer[61] = 7;
  buffer[62] = 44;
  buffer[63] = 12;

  char * string_time = controler_parser_get_glue_application_time(buffer);

  if(strcmp(string_time, "05.11.2020-07:44:12") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(string_time);
}

static void controler_parser_get_interval_from_glue_application_test()
{
  printf("controler_parser_get_interval_from_glue_application: ");
  uint8_t * buffer = ut_get_buffer();

  buffer = s7lib_parser_write_int(buffer, 56, 2020);
  buffer[58] = 5;
  buffer[59] = 11;
  buffer[61] = 7;
  buffer[62] = 44;
  buffer[63] = 12;

  buffer = s7lib_parser_write_int(buffer, 68, 2020);
  buffer[70] = 5;
  buffer[71] = 11;
  buffer[73] = 7;
  buffer[74] = 45;
  buffer[75] = 12;

  char * string_interval = controler_parser_get_interval_from_glue_application(buffer);

  if(strcmp(string_interval, "60") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(string_interval);
}

static void controler_parser_get_assembly_time_test()
{
  printf("controler_parser_get_assembly_time: ");

  uint8_t * buffer = ut_get_buffer();

  buffer = s7lib_parser_write_int(buffer, 68, 2020);
  buffer[70] = 5;
  buffer[71] = 11;
  buffer[73] = 7;
  buffer[74] = 44;
  buffer[75] = 12;

  char * string_time = controler_parser_get_assembly_time(buffer);

  if(strcmp(string_time, "05.11.2020-07:44:12") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(string_time);
}

static void controler_parser_get_primer_inspection_bypass_flag_test()
{
  printf("controler_parser_get_primer_inspection_bypass_flag: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_bool(buffer, 80,0,true);

  char * string_flag = controler_parser_get_primer_inspection_bypass_flag(buffer);

  if(strcmp(string_flag, "true") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(string_flag);
}

static void controler_parser_get_glue_aplication_inspection_result_flag_test()
{
  printf("controler_parser_get_glue_aplication_inspection_result_flag: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_bool(buffer, 80,1,true);

  char * string_flag = controler_parser_get_glue_aplication_inspection_result_flag(buffer);

  if(strcmp(string_flag, "pass") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(string_flag);
}

static void controler_parser_get_glue_applied_flag_test()
{
  printf("controler_parser_get_glue_applied_flag: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_bool(buffer, 80,2,true);

  char * string_flag = controler_parser_get_glue_applied_flag(buffer);

  if(strcmp(string_flag, "true") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(string_flag);
}

static void controler_parser_get_glass_delete_flag_test()
{
  printf("controler_parser_get_glass_delete_flag: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_bool(buffer, 80,3,true);

  char * string_flag = controler_parser_get_glass_delete_flag(buffer);

  if(strcmp(string_flag, "true") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(string_flag);
}

static void controler_parser_time_to_seconds_test()
{
  printf("controler_parser_time_to_seconds: ");

  int time [6] = {0,0,0,1,1,10};

  uint64_t interval = controler_parser_time_to_seconds(time);

  if(interval == 3670)
    printf("PASS\n");
  else
    printf("FAIL\n");
}

static void controler_parser_get_glue_A_batch_number_test()
{
  printf("controler_parser_get_glue_A_batch_number: ");
  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_string(buffer, 82, "ertzuiopasdYXCVB", 16);

  char * barrel_a_batch_number = controler_parser_get_glue_A_batch_number(buffer);

  if(strcmp(barrel_a_batch_number,"ertzuiopasdYXCVB") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);

  if(barrel_a_batch_number != NULL)
    free(barrel_a_batch_number);
}

static void controler_parser_get_glue_B_batch_number_test()
{
  printf("controler_parser_get_glue_B_batch_number: ");
  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_string(buffer, 104, "ertzuiopasdYXCVB", 16);

  char * barrel_b_batch_number = controler_parser_get_glue_B_batch_number(buffer);

  if(strcmp(barrel_b_batch_number,"ertzuiopasdYXCVB") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);

  if(barrel_b_batch_number != NULL)
    free(barrel_b_batch_number);
}

static void controler_parser_get_glue_A_expiration_date_test()
{
  printf("controler_parser_get_glue_A_expiration_date: ");
  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_int(buffer, 100, 2019);
  buffer[102] = 2;

  char * barrel_a_expiration_date = controler_parser_get_glue_A_expiration_date(buffer);

  if(strcmp(barrel_a_expiration_date, "2019-02") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);

  if(barrel_a_expiration_date != NULL)
    free(barrel_a_expiration_date);
}

static void controler_parser_get_glue_B_expiration_date_test()
{
  printf("controler_parser_get_glue_B_expiration_date: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_int(buffer, 122, 2019);
  buffer[124] = 2;

  char * barrel_b_expiration_date = controler_parser_get_glue_B_expiration_date(buffer);

  if(strcmp(barrel_b_expiration_date, "2019-02") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);

  if(barrel_b_expiration_date != NULL)
    free(barrel_b_expiration_date);
}

static void controler_parser_get_glue_application_ratio_test()
{
  printf("controler_parser_get_glue_application_ratio: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_int(buffer, 126, 100.0);
  buffer = s7lib_parser_write_int(buffer, 130, 3.0);

  char * glue_application_ratio = controler_parser_get_glue_application_ratio(buffer);

  if(strcmp(glue_application_ratio, "100.0:3.0") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);

  if(glue_application_ratio != NULL)
    free(glue_application_ratio);
}

static void controler_parser_get_glue_temperature_during_application_test()
{
  printf("controler_parser_get_glue_temperature_during_application: ");
  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_real(buffer, 134, 55.1234);

  char * glue_temperature_during_application = controler_parser_get_glue_temperature_during_application(buffer);

  if(strcmp(glue_temperature_during_application, "55.1") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);

  if(glue_temperature_during_application != NULL)
    free(glue_temperature_during_application);
}

static void controler_parser_get_applied_glue_ammount_test()
{
  printf("controler_parser_get_applied_glue_amount: ");
  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_real(buffer, 138, 245.3);

  char * applied_glue_amount = controler_parser_get_applied_glue_amount(buffer);

  if(strcmp(applied_glue_amount, "245.3") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);
  free(applied_glue_amount);
}

static void controler_parser_get_humidity_test()
{
  printf("controler_parser_get_humidity: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_real(buffer, 142, 77.4);

  char * ambient_humidity = controler_parser_get_humidity(buffer);

  if(strcmp(ambient_humidity, "77.4") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);

  if(ambient_humidity != NULL)
    free(ambient_humidity);
}

static void controler_parser_get_ambient_temperature_test()
{
  printf("controler_parser_get_ambient_temperature: ");

  uint8_t * buffer = ut_get_buffer();
  buffer = s7lib_parser_write_real(buffer, 146, 27.8);

  char * ambient_temperature = controler_parser_get_ambient_temperature(buffer);




  if(strcmp(ambient_temperature, "27.8") == 0)
    printf("PASS\n");
  else
    printf("FAIL\n");

  free(buffer);

  if(ambient_temperature != NULL)
    free(ambient_temperature);
}
