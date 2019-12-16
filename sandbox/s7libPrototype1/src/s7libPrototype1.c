/*
 ============================================================================
 Name        : s7libPrototype1.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "snap7.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>

#include <ctype.h>
#ifdef OS_WINDOWS
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif

struct _db_
{
	char reg1[16];
	int16_t integer;
};
typedef struct _db_ db;

char * Address = "192.168.0.1";
int Rack = 0;
int Slot = 1;

char * byte_to_bin_string(char byte)
{
	char * byte_str = malloc(sizeof(char)*9);

	for(int i = 0; i < 8; i++)
	{
		if(byte > 0)
		{
			int number = byte % 2;
			byte /=2;

			if(number > 0 )
				byte_str[i] = '1';
			else
				byte_str[i] = '0';
		}
		else
		{
			byte_str[i] = '0';
		}
	}

	byte_str[8] = 0;
	return byte_str;
}

uint16_t uint16_change_endian(int number)
{
	uint8_t arr[2];

	arr[0] = ((uint8_t)((number & 0xff00) >> 8));
	arr[1] = ((uint8_t)((number & 0x00ff) >> 0));

	uint16_t x = *(uint16_t *) arr;

	return x;
}


int16_t int16_change_endian(int number)
{
	return (int16_t) uint16_change_endian(number);
}


int main(int argc, char** argv)
{

	sleep(5);
	S7Object Client = Cli_Create();

	int res = Cli_ConnectTo(Client, Address,Rack,Slot);

	if(res == 0)
	{
		printf("successfull connected\n");

		db * my_db = malloc(sizeof(db));
		Cli_DBRead(Client, 4, 0, sizeof(db), my_db);

		for(int i = 0; i <16; i++)
		{
			char * str = byte_to_bin_string(my_db->reg1[i]);
			printf("%s\n", str);
			free(str);
			my_db->reg1[i] = 0;
		}

		printf("%d\n", int16_change_endian(my_db->integer));

		my_db->integer = int16_change_endian(6155); //0x0100;

		Cli_DBWrite(Client, 4, 0, sizeof(db), my_db);


		free(my_db);

	}
	else
	{
		printf("connection error!\n");
	}

	Cli_Destroy(&Client);

    return 0;
}






















