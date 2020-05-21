#ifndef _controler_parser_get_H_
#define _controler_parser_get_H_

#include <stdint.h>
#include <math.h>
#include <s7lib_parser.h>

char * controler_parser_get_window_id(uint8_t *);
char * controler_parser_get_vehicle_number(uint8_t *);
char * controler_parser_get_rear_window_type(uint8_t *);
char * controler_parser_get_vehicle_model(uint8_t *);
char * controler_parser_get_primer_application_time(uint8_t *);
char * controler_parser_get_interval_from_primer_application(uint8_t *);
char * controler_parser_get_glue_application_time(uint8_t *);
char * controler_parser_get_interval_from_glue_application(uint8_t *);
char * controler_parser_get_assembly_time(uint8_t *);
char * controler_parser_get_primer_inspection_bypass_flag(uint8_t *);
char * controler_parser_get_glue_aplication_inspection_result_flag(uint8_t *);
char * controler_parser_get_glue_applied_flag(uint8_t *);
char * controler_parser_get_glass_delete_flag(uint8_t *);
char * controler_parser_get_glue_A_batch_number(uint8_t *);
char * controler_parser_get_glue_B_batch_number(uint8_t *);
char * controler_parser_get_glue_A_expiration_date(uint8_t *);
char * controler_parser_get_glue_B_expiration_date(uint8_t *);
char * controler_parser_get_glue_application_ratio(uint8_t *);
char * controler_parser_get_glue_temperature_during_application(uint8_t *);
char * controler_parser_get_applied_glue_amount(uint8_t *);
char * controler_parser_get_humidity(uint8_t *);
char * controler_parser_get_ambient_temperature(uint8_t *);
uint64_t controler_parser_time_to_seconds(int *);

#endif
