#include "include/controler.h"
#include "include/controler_parser.h"


static bool controler_write_to_file(char *, char *);
static char * controler_generate_csv_line(uint8_t *, char);
static bool controler_store_csv_line(uint8_t *, char *, char);
static char * controler_generate_csv_header();
static bool controler_write_to_file_without_header(char *, char *);
static bool controler_write_to_file_with_header(char *, char *);
static char * controler_add_csv_line_parameter(char *, char *, char);


uint8_t * controler_check_store_request(uint8_t * buffer, char * csv_path, char separator)
{
    if(s7lib_parser_read_bool(buffer, 0, 0) == true)
    {
        if(controler_store_csv_line(buffer, csv_path, separator) == true)
          return s7lib_parser_write_bool(buffer, 0, 0, false);
    }

    return buffer;
}


/******************************* static functions **********************************/

static bool controler_write_to_file_with_header(char * line, char * csv_path)
{
  FILE * file = fopen(csv_path, "a");

  if(file != NULL)
  {
    fprintf(file, "%s\n%s\n", controler_generate_csv_header(), line);

    fflush(file);
    fclose(file);

    return true;
  }

  return false;
}

static bool controler_write_to_file_without_header(char * line, char * csv_path)
{
  FILE * file = fopen(csv_path, "a");

  if(file != NULL)
  {
    fprintf(file, "%s\n", line);

    fflush(file);
    fclose(file);

    return true;
  }

  return false;
}

static bool controler_write_to_file(char * line, char * csv_path)
{
  if(line != NULL)
  {
    if(access(csv_path, F_OK) != -1)
      return controler_write_to_file_without_header(line, csv_path);
    else
      return controler_write_to_file_with_header(line, csv_path);
  }

  return false;
}

static char * controler_generate_csv_header()
{
  return "ID;\
Vehicle number;\
Rear window type;\
Vehicle model;\
Primer application time;\
Interval from primer application;\
Glue application time;\
Interval from glue application;\
Assembly time;\
Primer inspection bypass flag;\
Glue application inspection result;\
Glue applied flag;\
Glass removed flag;\
Glue A batch number;\
Glue B batch number;\
Glue A expiration date;\
Glue B expiration date;\
Glue application ratio;\
Glue temperature during application;\
Applied glue amount;\
Ambient humidity;\
Ambient temperature;";
}

static char * controler_add_csv_line_parameter(char * line, char * parameter, char separator)
{
  if(line != NULL)
    line = c_string_add(line, parameter);
  else
    line = parameter;

  line = c_string_concat_character(line, separator);

  return line;
}

static char * controler_generate_csv_line(uint8_t * buffer, char separator)
{
  char * csv_line = NULL;
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_window_id(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_vehicle_number(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_rear_window_type(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_vehicle_model(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_primer_application_time(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_interval_from_primer_application(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_glue_application_time(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_interval_from_glue_application(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_assembly_time(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_primer_inspection_bypass_flag(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_glue_aplication_inspection_result_flag(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_glue_applied_flag(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_glass_delete_flag(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_glue_A_batch_number(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_glue_B_batch_number(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_glue_A_expiration_date(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_glue_B_expiration_date(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_glue_application_ratio(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_glue_temperature_during_application(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_applied_glue_amount(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_humidity(buffer), separator);
  csv_line = controler_add_csv_line_parameter(csv_line, controler_parser_get_ambient_temperature(buffer), separator);

  return csv_line;
}

static bool controler_store_csv_line(uint8_t * buffer, char * csv_path, char separator)
{
    char * csv_line = controler_generate_csv_line(buffer, separator);

    if(csv_line != NULL)
    {
      bool result =  controler_write_to_file(csv_line, csv_path);
      free(csv_line);

      return result;
    }

    return false;
}
