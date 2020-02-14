#include "include/glass_info.h"

struct _glass_info_
{
	char vehicle_number[VEHICLE_NUMBER_LENGTH+1];
	char rear_window_type[REAR_WINDOW_TYPE_LENGTH+1];
	char vehicle_model;
	uint32_t id;
};


glass_info * glass_info_new(char * vehicle_number, char * rear_window_type, char vehicle_model, uint32_t id)
{
	glass_info * this = malloc(sizeof(glass_info));

	strncpy(this->vehicle_number, vehicle_number, VEHICLE_NUMBER_LENGTH);
	strncpy(this->rear_window_type, rear_window_type, REAR_WINDOW_TYPE_LENGTH);
	this->vehicle_model = vehicle_model;
	this->id = id;

	return this;
}

char * glass_info_get_vehicle_number(glass_info * this)
{
	return this->vehicle_number;
}

char * glass_info_get_rear_window_type(glass_info * this)
{
	return this->rear_window_type;
}

char glass_info_get_vehicle_model(glass_info * this)
{
	return this->vehicle_model;
}

uint32_t glass_info_get_id(glass_info * this)
{
	return this->id;
}

size_t glass_info_get_sizeof()
{
	return sizeof(glass_info);
}

void glass_info_finalize(void * this)
{
	free(this);
}
