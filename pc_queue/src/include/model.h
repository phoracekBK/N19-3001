#ifndef _MODEL_H_
#define _MODEL_H_

#include "plc.h"
#include "glass_info.h"

bool model_set_done_status(plc *);
bool model_set_error_status(plc *);
bool model_reset_error_status(plc *);
bool model_get_error_status(plc *);
bool model_reset_done_status(plc *);
bool model_reset_cmd_byte(plc *);
bool model_write_queue_size(plc *, int16_t);
bool model_set_priority_input_status(plc *);
bool model_reset_priority_input_status(plc *);
bool model_read_cmd_reload_visu_status(plc *);
bool model_read_cmd_enqueue_status(plc *);
bool model_read_cmd_priority_enqueue_status(plc *);
bool model_read_cmd_dequeue_status(plc *);
bool model_read_cmd_delete_status(plc *);
char * model_read_vehicle_number(plc *, int);
char * model_read_rear_window_type(plc *, int);
char model_read_vehicle_model(plc *, int);
uint32_t model_read_id(plc *, int);
glass_info * model_read_glass_info(plc *, int);
bool model_write_glass_info(plc *, glass_info *, int);
uint8_t * model_write_glass_info_to_array(uint8_t *, glass_info *, int);
glass_info * model_read_glass_info_from_array(uint8_t *, int);
bool model_write_visu_queue(plc *, uint8_t *, int16_t);



#endif
