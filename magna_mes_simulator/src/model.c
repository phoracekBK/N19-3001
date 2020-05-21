#include "include/model.h"
#include <time.h>


struct _model_
{
	s7lib * s7lib_ref;
	c_queue * glass_in_production_queue;
};



static uint32_t model_find_glass_in_queue(c_linked_list *, char *);
static bool model_is_id_occupied(c_linked_list *, uint32_t);
static uint32_t model_generate_default_id(c_linked_list *, uint32_t);


model * model_new()
{
	model * this = malloc(sizeof(model));

	this->s7lib_ref = s7lib_new(PLC_IP_ADDRESS, PLC_RACK, PLC_SLOT, PLC_DB_NUMBER);
	this->glass_in_production_queue = c_queue_new(0);

	return this;
}

uint32_t model_glass_queue_size(model * this)
{
	return c_queue_size(this->glass_in_production_queue);
}

bool model_glass_queue_delete_glass_at_index(model * this, uint32_t glass_index)
{
	return c_queue_delete_at_index_with_release(this->glass_in_production_queue, glass_index, glass_info_finalize);
}

static uint32_t model_find_glass_in_queue(c_linked_list * list, char * vehicle_number)
{
	if(list != NULL)
	{
		glass_info * glass = c_linked_list_node_get_data(list);

		if(strcmp(glass_info_get_vehicle_number(glass), vehicle_number) != 0)
			return model_find_glass_in_queue(c_linked_list_get_next(list), vehicle_number) + 1;
	}

	return 0;
}

bool model_glass_queue_delete_glass_with_vehicle_number(model * this, char * vehicle_number)
{
	uint32_t glass_index = model_find_glass_in_queue(c_queue_get_tail(this->glass_in_production_queue), vehicle_number);

	return c_queue_delete_at_index_with_release(this->glass_in_production_queue, glass_index, glass_info_finalize);
}

bool model_glass_queue_insert_glass(model * this, glass_info * glass)
{
	return c_queue_enqueue(this->glass_in_production_queue, glass);
}

bool model_glass_queue_priority_insert_glass(model * this, glass_info * glass)
{
	return c_queue_priority_enqueue(this->glass_in_production_queue, glass);
}

glass_info * model_glass_queue_peek(model * this, uint32_t glass_index)
{
	return c_queue_peek(this->glass_in_production_queue, glass_index);
}

bool model_glass_queue_dequeue_glass(model * this)
{
	return c_queue_dequeue_with_release(this->glass_in_production_queue, glass_info_finalize);
}

static bool model_is_id_occupied(c_linked_list * list, uint32_t id)
{
	if(list != NULL)
	{
		glass_info * glass = c_linked_list_node_get_data(list);

		if(glass_info_get_id(glass) != id)
			return model_is_id_occupied(c_linked_list_get_next(list), id);
		else
			return false;
	}

	return true;
}

static uint32_t model_generate_default_id(c_linked_list * list, uint32_t id)
{
	if(list != NULL)
	{
		if(model_is_id_occupied(list, id) == false)
			return model_generate_default_id(list, id + 1);
	}

	return id;
}

bool model_enqueu_glass(model * this, glass_info * glass)
{
	if(c_queue_enqueue(this->glass_in_production_queue, glass) == true)
		return true;

	return false;
}

bool model_priority_enqueu_glass(model * this, glass_info * glass)
{
	return false;
}

bool model_dequeue_glass(model * this, glass_info * glass)
{
	return false;
}

static int32_t controler_find_glass_in_queue(c_linked_list * tail, glass_info * glass)
{
	if(tail != NULL)
	{
		glass_info * glass_in_queue = c_linked_list_node_get_data(tail);

		if(strcmp(glass_info_get_vehicle_number(glass_in_queue), glass_info_get_vehicle_number(glass)) == 0)
			return 0;
		else
			return controler_find_glass_in_queue(c_linked_list_get_next(tail), glass) + 1;
	}

	return -1;
}

bool model_delete_glass(model * this, glass_info * glass)
{
	int32_t glass_index = controler_find_glass_in_queue(c_queue_get_tail(this->glass_in_production_queue), glass);

	if(glass_index >= 0)
		return c_queue_delete_at_index_with_release(this->glass_in_production_queue, glass_index, glass_info_finalize);

	return false;
}

glass_info * model_get_default_glass_info(model * this)
{
	srand((unsigned) time(0));
	uint8_t model_choice = rand() % 2;
	uint8_t rear_window_type_choice = rand() % 2;

	c_linked_list * list = c_queue_get_tail(this->glass_in_production_queue);
	uint32_t list_size = c_queue_size(this->glass_in_production_queue);

	uint32_t id = model_generate_default_id(list, list_size);
	char vehicle_number[VEHICLE_NUMBER_LENGTH+1];
	char rear_window_type[REAR_WINDOW_TYPE_LENGTH+1];
	char vehicle_model = model_choice == 0 ? '7' : 'I';

	snprintf(vehicle_number, VEHICLE_NUMBER_LENGTH, "%09d 00", id);
	snprintf(rear_window_type, REAR_WINDOW_TYPE_LENGTH+1, "%s", rear_window_type_choice == 0 ? "ABC DEF GHI JK LMN" : "ABC DEF GHI");

	glass_info * glass = glass_info_new(vehicle_number, rear_window_type, vehicle_model, id);

	return glass;
}

glass_info * model_get_next_glass_in_order(model * this)
{
	return c_queue_peek(this->glass_in_production_queue, 0);
}

uint32_t model_get_glass_in_production_queue_size(model * this)
{
	return c_queue_size(this->glass_in_production_queue);
}

c_linked_list * model_get_glass_list(model * this)
{
	return c_queue_get_tail(this->glass_in_production_queue);
}


uint16_t model_get_versionsnummer(model * this)
{
	return s7lib_read_int(this->s7lib_ref, 0);
}

bool model_set_sersionsnummer(model * this, uint16_t sersionsnumber)
{
	return s7lib_write_int(this->s7lib_ref, 0, sersionsnumber);
}

char model_get_tailtype(model * this)
{
	uint8_t * byte = s7lib_read(this->s7lib_ref, 2, 1);
	char tailtype = *byte;

	free(byte);

	return tailtype;
}

bool model_set_tailtype(model * this, char tailtype)
{
	return s7lib_write(this->s7lib_ref, (uint8_t *) &tailtype, 2, 1);
}


bool model_get_flag1(model * this)
{
	return s7lib_read_bool(this->s7lib_ref, 3, 0);
}

bool model_set_flag1(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 3, 0, true);
}

bool model_reset_flag1(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 3, 0, false);
}


char * model_get_vehicle_number1(model * this)
{
	return (char *) s7lib_read(this->s7lib_ref, 4, VEHICLE_NUMBER_LENGTH);
}

bool model_set_vehicle_number1(model * this, char * vehicle_number)
{
	return s7lib_write(this->s7lib_ref, (uint8_t*) vehicle_number, 4, VEHICLE_NUMBER_LENGTH);
}

char * model_get_rear_window_type1(model * this)
{
	return (char*) s7lib_read(this->s7lib_ref, 18, REAR_WINDOW_TYPE_LENGTH);
}

bool model_set_rear_window_type1(model * this, char * rear_window_type)
{
	return s7lib_write(this->s7lib_ref, (uint8_t*) rear_window_type, 18,  REAR_WINDOW_TYPE_LENGTH);
}

char model_get_vehicle_model1(model * this)
{
	uint8_t * byte = s7lib_read(this->s7lib_ref, 36, 1);
	char vehicle_model = *byte;

	free(byte);

	return vehicle_model;
}

bool model_set_vehicle_model1(model * this, char vehicle_model)
{
	return s7lib_write(this->s7lib_ref, (uint8_t *) &vehicle_model, 36, 1);
}

bool model_get_flag2(model * this)
{
	return s7lib_read_bool(this->s7lib_ref, 37, 0);
}

bool model_set_flag2(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 37, 0, true);
}

bool model_reset_flag2(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 37, 0, false);
}


char * model_get_vehicle_number2(model * this)
{
	return (char*) s7lib_read(this->s7lib_ref, 38, VEHICLE_NUMBER_LENGTH);
}

bool model_set_vehicle_number2(model * this, char * vehicle_number)
{
	return s7lib_write(this->s7lib_ref, (uint8_t*) vehicle_number, 38, VEHICLE_NUMBER_LENGTH);
}

char * model_get_rear_window_type2(model * this)
{
	return (char*) s7lib_read(this->s7lib_ref, 52, REAR_WINDOW_TYPE_LENGTH);
}

bool model_set_rear_window_type2(model * this, char * rear_window_type)
{
	return s7lib_write(this->s7lib_ref, (uint8_t*) rear_window_type, 52, REAR_WINDOW_TYPE_LENGTH);
}

bool model_get_flag3(model * this)
{
	return s7lib_read_bool(this->s7lib_ref, 70, 0);
}

bool model_set_flag3(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 70, 0, true);
}

bool model_reset_flag3(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 70, 0, false);
}

char * model_get_vehicle_number3(model * this)
{
	return (char *) s7lib_read(this->s7lib_ref, 72, VEHICLE_NUMBER_LENGTH);
}

bool model_set_vehicle_number3(model * this, char * vehicle_number)
{
	return s7lib_write(this->s7lib_ref, (uint8_t*) vehicle_number, 72, VEHICLE_NUMBER_LENGTH);
}


char * model_get_rear_window_type3(model * this)
{
	return (char*) s7lib_read(this->s7lib_ref, 86, REAR_WINDOW_TYPE_LENGTH);
}

bool model_set_rear_window_type3(model * this, char * rear_window_type)
{
	return s7lib_write(this->s7lib_ref, (uint8_t*) rear_window_type, 86, REAR_WINDOW_TYPE_LENGTH);
}


bool model_get_flag4(model * this)
{
	return s7lib_read_bool(this->s7lib_ref, 104, 0);
}

bool model_set_flag4(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 104, 0, true);
}

bool model_reset_flag4(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 104, 0, false);
}


uint32_t model_get_id(model * this)
{
	return s7lib_read_dword(this->s7lib_ref, 106);
}

bool model_set_id(model * this, uint32_t id)
{
	return s7lib_write_dword(this->s7lib_ref, 106, id);
}


bool model_get_glass_panel_taken_out(model * this)
{
	return s7lib_read_bool(this->s7lib_ref, 116, 0);
}

bool model_set_glass_panel_taken_out(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 116, 0, true);
}

bool model_reset_glass_panel_taken_out(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 116, 0, false);
}




bool model_get_glass_panel_is_in_tailgate(model * this)
{
	return s7lib_read_bool(this->s7lib_ref, 117, 0);
}

bool model_set_glass_panel_is_in_tailgate(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 117, 0, true);
}

bool model_reset_glass_panel_is_in_tailgate(model * this)
{
	return s7lib_write_bool(this->s7lib_ref, 117, 0, false);
}

void model_finalize(model * this)
{
	s7lib_finalize(this->s7lib_ref);

	c_queue_finalize_with_release(this->glass_in_production_queue, glass_info_finalize);

	free(this);
}
