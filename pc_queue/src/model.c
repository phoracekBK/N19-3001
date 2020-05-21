#include "include/model.h"





bool model_set_error_status(s7lib * this)
{
	return s7lib_write_bool(this, 466, 1, true);
}

bool model_reset_error_status(s7lib * this)
{
	return s7lib_write_bool(this, 466, 1, false);
}

bool model_get_error_status(s7lib * this)
{
	return s7lib_read_bool(this, 466, 1);
}

bool model_set_done_status(s7lib * this)
{
	return s7lib_write_bool(this, 468, 0, true);
}

bool model_reset_done_status(s7lib * this)
{
	return s7lib_write_bool(this, 468, 0, false);
}

bool model_set_priority_input_status(s7lib * this)
{
	return s7lib_write_bool(this, 466, 0, true);
}

bool model_reset_priority_input_status(s7lib * this)
{
	return s7lib_write_bool(this, 466, 0, false);
}

bool model_read_cmd_reload_visu_status(s7lib * this)
{
	return s7lib_read_bool(this, 42, 4);
}

bool model_read_cmd_enqueue_status(s7lib * this)
{
	return s7lib_read_bool(this, 42, 0);
}

bool model_read_cmd_priority_enqueue_status(s7lib * this)
{
	return s7lib_read_bool(this, 42, 1);
}

bool model_read_cmd_dequeue_status(s7lib * this)
{
	return s7lib_read_bool(this, 42, 2);
}

bool model_read_cmd_delete_status(s7lib * this)
{
	return s7lib_read_bool(this, 42, 3);
}

bool model_reset_cmd_byte(s7lib * this)
{
	uint8_t byte = 0;
	return s7lib_write(this, &byte, 42, 1);
}

char * model_read_vehicle_number(s7lib * this, int base)
{
	return s7lib_read_string(this, base, 13);
}

char * model_read_rear_window_type(s7lib * this, int base)
{
	return s7lib_read_string(this, base, 18);
}

char model_read_vehicle_model(s7lib * this, int base)
{
	uint8_t * byte = s7lib_read(this, base, 1);

	if(byte != NULL)
	{
		char model = *byte;

		free(byte);
		return model;
	}

	return 0;
}

bool model_write_queue_size(s7lib * this, int16_t queue_size)
{
	return s7lib_write_int(this, 464, queue_size);
}

uint32_t model_read_id(s7lib * this, int base)
{
	return s7lib_read_dword(this, base);
}

glass_info * model_read_glass_info_from_array(uint8_t * byte_array, int base)
{
	char * vehicle_number = s7lib_parser_read_string(byte_array, base, VEHICLE_NUMBER_LENGTH);
	char * rear_window_type = s7lib_parser_read_string(byte_array, base + 16, REAR_WINDOW_TYPE_LENGTH);
	char vehicle_model = byte_array[base + 36];
	uint32_t id = s7lib_parser_read_dword(byte_array, base + 38);

	glass_info * glass = glass_info_new(vehicle_number, rear_window_type, vehicle_model, id);

	free(vehicle_number);
	free(rear_window_type);

	return glass;
}

uint8_t * model_write_glass_info_to_array(uint8_t * byte_array, glass_info * glass, int base)
{
	byte_array = s7lib_parser_write_string(byte_array, base, glass_info_get_vehicle_number(glass), VEHICLE_NUMBER_LENGTH);
	byte_array = s7lib_parser_write_string(byte_array, base + 16, glass_info_get_rear_window_type(glass), REAR_WINDOW_TYPE_LENGTH);
	byte_array[base + 36] = glass_info_get_vehicle_model(glass);
	byte_array = s7lib_parser_write_dword(byte_array, base + 38, glass_info_get_id(glass));

	return byte_array;
}

glass_info * model_read_glass_info(s7lib * this, int base)
{
	uint8_t * byte_array = s7lib_read(this, base, 42);

		if(byte_array != NULL)
			return model_read_glass_info_from_array(byte_array, 0);
		else
			return NULL;
}

bool model_write_glass_info(s7lib * this, glass_info * glass, int base)
{
	uint8_t * byte_array = malloc(sizeof(uint8_t) * 42);
	byte_array = model_write_glass_info_to_array(byte_array, glass, 0);

	bool return_value = s7lib_write(this, byte_array, base, 42);

	free(byte_array);

	return return_value;
}

bool model_write_visu_queue(s7lib * this, uint8_t * byte_array, int16_t list_size)
{
	return (s7lib_write(this, byte_array, 44, 42*10) && model_write_queue_size(this, list_size));
}
