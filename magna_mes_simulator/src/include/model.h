#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <c_linked_list.h>
#include <c_queue.h>
#include <s7lib.h>

#include "glass_info.h"
#include "config.h"


struct _model_;
typedef struct _model_ model;

model * model_new();

uint32_t model_glass_queue_size(model *);
bool model_glass_queue_delete_glass_at_index(model *, uint32_t);
bool model_glass_queue_delete_glass_with_vehicle_number(model *, char *);
bool model_glass_queue_insert_glass(model *, glass_info *);
bool model_glass_queue_priority_insert_glass(model *, glass_info *);
glass_info * model_glass_queue_peek(model *, uint32_t);
bool model_glass_queue_dequeue_glass(model *);
c_linked_list * model_get_glass_list(model *);

bool model_enqueu_glass(model * , glass_info *);
bool model_priority_enqueu_glass(model *, glass_info *);
bool model_dequeue_glass(model *, glass_info *);
bool model_delete_glass(model *, glass_info *);

glass_info * model_get_default_glass_info(model *);
glass_info * model_get_next_glass_in_order(model *);
uint32_t model_get_glass_in_production_queue_size(model *);

uint16_t model_get_versionsnummer(model *);
bool model_set_sersionsnummer(model *, uint16_t);

char model_get_tailtype(model *);
bool model_set_tailtype(model *, char);

bool model_get_flag1(model *);
bool model_set_flag1(model *);
bool model_reset_flag1(model *);

char * model_get_vehicle_number1(model *);
bool model_set_vehicle_number1(model *, char *);

char * model_get_rear_window_type1(model *);
bool model_set_rear_window_type1(model *, char *);

char model_get_vehicle_model1(model *);
bool model_set_vehicle_model1(model *, char);

bool model_get_flag2(model *);
bool model_set_flag2(model *);
bool model_reset_flag2(model *);


char * model_get_vehicle_number2(model *);
bool model_set_vehicle_number2(model *, char *);

char * model_get_rear_window_type2(model *);
bool model_set_rear_window_type2(model *, char *);

bool model_get_flag3(model *);
bool model_set_flag3(model *);
bool model_reset_flag3(model *);

char * model_get_vehicle_number3(model *);
bool model_set_vehicle_number3(model *, char *);

char * model_get_rear_window_type3(model *);
bool model_set_rear_window_type3(model *, char *);

bool model_get_flag4(model *);
bool model_set_flag4(model *);
bool model_reset_flag4(model *);

uint32_t model_get_id(model *);
bool model_set_id(model *, uint32_t);


bool model_get_glass_panel_taken_out(model *);
bool model_set_glass_panel_taken_out(model *);
bool model_reset_glass_panel_taken_out(model *);


bool model_get_glass_panel_is_in_tailgate(model *);
bool model_set_glass_panel_is_in_tailgate(model *);
bool model_reset_glass_panel_is_in_tailgate(model *);

void model_finalize(model *);





#endif
