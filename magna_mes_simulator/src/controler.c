#include "include/controler.h"

struct _controler_
{
	model * model_ref;
};


controler * controler_new(model * model_ref)
{
	controler * this = malloc(sizeof(controler));

	this->model_ref = model_ref;

	return this;
}

static bool controler_write_glass_information1(controler * this, glass_info * glass)
{
	if(model_get_flag1(this->model_ref) == false)
	{	
		if(model_set_vehicle_number1(this->model_ref, glass_info_get_vehicle_number(glass)) == false)
			return false;
		
		if(model_set_rear_window_type1(this->model_ref, glass_info_get_rear_window_type(glass)) == false)
			return false;

		if(model_set_vehicle_model1(this->model_ref, glass_info_get_vehicle_model(glass)) == false)
			return false;
		
		if(model_set_flag1(this->model_ref) == false)
			return false;	

		return true;
	}

	return false;
}

static bool controler_wait_for_signal(controler * this, bool (*status_fce)(model *), int delay, bool value)
{
	int fail_counter = 0;

	while(status_fce(this->model_ref) != value)
	{
		if(fail_counter > delay)
			return false;

		fail_counter ++;
		sleep(1);
	}

	return true;
}


bool controler_move_glass_to_production_request(controler * this, glass_info * glass)
{
	if(controler_write_glass_information1(this, glass) == true)
	{
		if(controler_wait_for_signal(this, model_get_flag1, 5, false) == true)
		{
			if(model_enqueu_glass(this->model_ref, glass) == true)	
				return true;
		}
	}

	return false;
}



bool controler_move_priority_glass_to_production_request(controler * this, glass_info * glass)
{
	return false;
}


static bool controler_move_glass_information2(controler * this, glass_info * glass)
{
	if(model_get_flag2(this->model_ref) == false)
	{
		if(model_set_vehicle_number2(this->model_ref, glass_info_get_vehicle_number(glass)) == false)
			return false;

		if(model_set_rear_window_type2(this->model_ref, glass_info_get_rear_window_type(glass)) == false)
			return false;

		if(model_set_flag2(this->model_ref) == false)
			return false;

		return true;
	}

	return false;
}

bool controler_delete_glass_from_production_request(controler * this, glass_info * glass)
{
	if(controler_move_glass_information2(this, glass) == true)
	{
		if(controler_wait_for_signal(this, model_get_flag2, 5, false) == true)
		{
			if(model_delete_glass(this->model_ref, glass) == true)
				return true;
		}
	}

	return false;
}

bool controler_move_glass_information3(controler * this, glass_info * glass)
{
	if(model_get_flag3(this->model_ref) == false)
	{
		if(model_set_vehicle_number3(this->model_ref, glass_info_get_vehicle_number(glass)) == false)
			return false;

		if(model_set_rear_window_type3(this->model_ref, glass_info_get_rear_window_type(glass)) == false)
			return false;

		if(model_set_flag3(this->model_ref) == false)
			return false;

		return true;
	}

	return false;
}

/* refaktor */
bool controler_send_glass_to_gluing_request(controler * this, glass_info * glass)
{
	if(controler_move_glass_information3(this, glass) == true)
	{
		if(controler_wait_for_signal(this, model_get_flag3, 5, false) == true)
		{
			if(model_get_flag4(this->model_ref) == true)
			{
				sleep(1);

				if(model_reset_flag4(this->model_ref) == false)
					return false;

				if(controler_wait_for_signal(this, model_get_glass_panel_taken_out, 60, true) == true)
				{
					sleep(1);

					if(model_reset_glass_panel_taken_out(this->model_ref) == false)
						return false;

					sleep(5);

					if(model_set_glass_panel_is_in_tailgate(this->model_ref) == true)
					{
						if(model_delete_glass(this->model_ref, glass) == true)	
							return true;
					}
				}
			}
		}
	}

	return false;
}


void controler_finalize(controler * this)
{
	free(this);
}





