
#include "include/plc.h"
#include "include/snap7.h"

struct _plc_
{
	S7Object Client;

	char * IPAddress;
	int Rack;
	int Slot;
	int DB_index;

	uint8_t connection_try;
};

typedef struct _plc_ plc;




plc * plc_new(char * IPAddress, int Rack, int Slot, int DB_index)
{
	plc * this = malloc(sizeof(plc));

	this->Client = Cli_Create();

	this->IPAddress = c_string_init(IPAddress);
	this->Rack = Rack;
	this->Slot = Slot;
	this->DB_index = DB_index;

	this->connection_try = 0;

	if(plc_connect(this) == true)
		printf("connected\n");
	else
		printf("connection error\n");

	return this;
}


bool plc_connect(plc * this)
{
	if(Cli_ConnectTo(this->Client, this->IPAddress, this->Rack, this->Slot) == 0)
		return true;
	else
		return false;
}

bool plc_disconnect(plc * this)
{
	if(Cli_Disconnect(this->Client) == 0)
		return true;
	else
		return false;
}


void plc_finalize(plc * this)
{
	plc_disconnect(this);
	Cli_Destroy(&(this->Client));

	free(this->IPAddress);

	free(this);
}


bool plc_reconnect(plc * this, uint8_t connection_try)
{
	if(connection_try < 3)
	{
		plc_disconnect(this);
		
		if(plc_connect(this) == true)
		{
			return true;
		}
		else
		{
			usleep(50000);
			return plc_reconnect(this, connection_try + 1);
		}
	}

	return false;
}

uint8_t * plc_read(plc * this, int base, int size)
{
	uint8_t * byte_array = malloc(sizeof(uint8_t)*size);
	
	if(Cli_DBRead(this->Client, this->DB_index, base, size, byte_array) != 0)
	{
		free(byte_array);
		plc_reconnect(this, 0);

		return NULL;
	}

	return byte_array;
}

bool plc_write(plc * this, uint8_t * byte_array, int base, int size)
{
	uint8_t * plc_byte_array = plc_read(this, base, size);

	if(plc_byte_array != NULL)
	{
		if(memcmp(plc_byte_array, byte_array, size) != 0)
		{
			if(Cli_DBWrite(this->Client, this->DB_index, base, size, byte_array) != 0)
			{
				return false;
			}
			else
			{
				free(plc_byte_array);
				usleep(50000);
				return plc_write(this, byte_array, base, size);
			}
		}
	
		free(plc_byte_array);
		return true;
	}

	return false;
}


bool plc_read_bool(plc * this, int byte_index, int bit_index)
{
	uint8_t * byte = plc_read(this, byte_index, 1);

	if(byte != NULL)
	{
		bool cmd = plc_read_bool_from_array(byte, 0, bit_index);

		free(byte);

		return cmd;
	}

	return false;
}

bool plc_write_bool(plc * this, int byte_index, int bit_index, bool value)
{
	uint8_t * byte = plc_read(this, byte_index, 1);

	if(byte != NULL)
	{
		byte =  plc_write_bool_to_array(byte, 0, bit_index, value);
		bool ret_val = plc_write(this, byte, byte_index, 1);

		free(byte);

		return ret_val;
	}

	return false;
}

int16_t plc_read_int(plc * this, int byte_index)
{
	uint8_t * byte_array = plc_read(this, byte_index, 2);

	if(byte_array != NULL)
		return plc_read_int_from_array(byte_array, byte_index);

	return 0;
}

bool plc_write_int(plc * this, int byte_index, int16_t value)
{
	uint8_t * byte_array = malloc(sizeof(uint8_t)*2);
	
	byte_array = plc_write_int_to_array(byte_array, 0, value);
	bool ret_val = plc_write(this, byte_array, byte_index, 2);

	free(byte_array);
		
	return ret_val;
}

bool plc_write_dword(plc * this, int byte_index, int32_t value)
{
	uint8_t * byte_array = malloc(sizeof(uint8_t)*4);

	byte_array =  plc_write_dword_to_array(byte_array, byte_index, value);
	bool ret_val = plc_write(this, byte_array, byte_index, 4);

	free(byte_array);

	return ret_val;
}

uint32_t plc_read_dword(plc * this, int byte_index)
{
	uint8_t * byte_array = plc_read(this, byte_index, 4);

	if(byte_array != NULL)
	{
		uint32_t value = plc_read_dword_from_array(byte_array, byte_index);

		free(byte_array);

		return value;
	}

	return 0;
}

bool plc_write_string(plc * this, int byte_index, char * string, uint8_t size)
{
	uint8_t * byte_array = malloc(sizeof(uint8_t)*(size+2));

	byte_array = plc_write_string_to_array(byte_array, byte_index, string, size);
	bool ret_val = plc_write(this, byte_array, byte_index, size+2);

	free(byte_array);

	return ret_val;
}

char * plc_read_string(plc * this, int byte_index, uint8_t size)
{
	uint8_t * byte_array = plc_read(this, byte_index, size);
	
	if(byte_array != NULL)
	{
		char * string = plc_read_string_from_array(byte_array, byte_index, size);

		free(byte_array);

		return string;
	}

	return NULL;
}


bool plc_read_bool_from_array(uint8_t * byte_array, int byte_index, int bit_index)
{	
	return (byte_array[byte_index] & (1 << bit_index));
}

uint8_t * plc_write_bool_to_array(uint8_t * byte_array, int byte_index, int bit_index, bool value)
{
	if(value == true)
		byte_array[byte_index] |= 1 << bit_index;
	else
		byte_array[byte_index] &= (128 << bit_index);

	return byte_array;
}

int16_t plc_read_int_from_array(uint8_t * byte_array, int byte_index)
{
	return byte_array[byte_index + 1] + (byte_array[byte_index]*255);
}

uint8_t * plc_write_int_to_array(uint8_t * byte_array, int byte_index, int16_t value)
{
	byte_array[byte_index + 1] = ((uint8_t*) &value)[0];
	byte_array[byte_index] = ((uint8_t*) &value)[1];

	return byte_array;
}

uint8_t * plc_write_dword_to_array(uint8_t * byte_array, int byte_index, int32_t value)
{	
	byte_array[byte_index + 3] =  ((uint8_t*)&value)[0];
	byte_array[byte_index + 2] =  ((uint8_t*)&value)[1];
	byte_array[byte_index + 1] =  ((uint8_t*)&value)[2];
	byte_array[byte_index] =  ((uint8_t*)&value)[3];

	return byte_array;
}

uint32_t plc_read_dword_from_array(uint8_t * byte_array, int byte_index)
{
	return ((byte_array[byte_index + 3]) + (byte_array[byte_index + 2]*255) + (byte_array[byte_index + 1]*512) + (byte_array[byte_index]*1024));
}

uint8_t * plc_write_string_to_array(uint8_t * byte_array, int byte_index, char * string, uint8_t size)
{
	int string_length = strlen(string);
	uint8_t length = string_length > size ? size : string_length;

	snprintf((char*) (byte_array+byte_index), size, "%c%c%s", size, length, string);

	return byte_array;
}

char * plc_read_string_from_array(uint8_t * byte_array, int byte_index, uint8_t size)
{
	uint8_t length = byte_array[byte_index + 1] > size ? size : byte_array[byte_index + 1];

	char * string = malloc(sizeof(char)*length+1);
	memset(string, 0, length+1);
	strncpy(string, (char*) (byte_array+byte_index+2), length);

	return string;
}

















