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
#include <aclib.h>

#include <unistd.h>

#include <ctype.h>
#ifdef OS_WINDOWS
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif



#define SERVER_INITIAL_0			 	0b00000001
#define SERVER_INITIAL_PLC_ACK_1 		0b00000010
#define SERVER_READY_FOR_CYCLE_2 		0b00000100
#define SERVER_CYCLE_REQUEST_ACK_3 		0b00001000
#define SERVER_CYCLE_REQUEST_REJECT_4	0b00010000
#define SERVER_CYCLE_ACK_5 				0b00100000

#define PLC_INITIAL_0				 	0b00000001
#define PLC_INITIAL_SERVER_ACK_1 		0b00000010
#define PLC_CYCLE_REQUEST_2				0b00000100
#define PLC_CYCLE_SUCCESS_3	 			0b00001000
#define PLC_CYCLE_FAILURE_4				0b00010000
#define PLC_CYCLE_ABORT_5				0b00100000




struct _DB_MES_PLC_;
typedef struct _DB_MES_PLC_ DB_MES_PLC;

struct _DB_MES_SERVER_;
typedef struct _DB_MES_SERVER_ DB_MES_SERVER;


struct _DB_MES_SERVER_
{
	bool LiveCheck;
	int8_t RES_001;
	uint8_t serverConstrolByte;	//InitialServer,InitialPLCAck,ReadyForCycle,CycleRequestAck,CycleRequestReject,CycleAck
	int8_t RES_003;
	char PartSerial[40];
	char ParameterBarcode[100];
	int16_t Error_Reason_Code;
	char ErrorMessage[200];
	char Reserve[154];
};

struct _DB_MES_PLC_
{
	uint8_t plcControlByte; //InitialPLC, InitialServerAck, CycleRequest, CycleSuccess, CycleFailure, CycleAbort
	int8_t RES_501;
	int8_t RES_502;
	int8_t RES_503;
	char PartSerialPLC[40];
	int16_t ProcessDataCount;
	int16_t RES_546[127];
	char KeyProcessData[50];
	char ValueProcessData[50];
};


struct _DB_MES_
{
	S7Object Client;

	char * IPAddress;
	int Rack;
	int Slot;
	int DB_index;

	DB_MES_PLC * DB_PLC;
	DB_MES_SERVER * DB_SERVER;
};
typedef struct _DB_MES_ DB_MES;






DB_MES * db_men_new(char *, int, int, int);
bool db_mes_connect(DB_MES *);
bool db_mes_disconnect(DB_MES * this);
bool db_mes_load(DB_MES *);
bool db_mes_write(DB_MES *);
void db_mes_finalize(DB_MES *);





char * byte_to_bin_string(char byte)
{
	char * byte_str = malloc(sizeof(char)*9);

	for(int i = 7; i >= 0; i--)
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

bool db_mes_load(DB_MES * this)
{
	if(this->DB_SERVER != NULL)
		free(this->DB_PLC);

	this->DB_PLC = malloc(sizeof(DB_MES_PLC));

	if(Cli_DBRead(this->Client, this->DB_index, 500, 400, this->DB_PLC) == 0)
		return true;
	else
		return true;
}

bool db_mes_write(DB_MES * this)
{
	if(this->DB_SERVER != NULL)
	{
		if(Cli_DBWrite(this->Client, this->DB_index, 0, sizeof(DB_MES_SERVER), this->DB_SERVER) == 0)
			return true;
	}

	return false;
}

DB_MES * db_men_new(char * IPAddress, int Rack, int Slot, int DB_index)
{
	DB_MES * this = malloc(sizeof(DB_MES));

	this->DB_PLC = NULL;
	this->DB_SERVER = NULL;

	this->Client = Cli_Create();

	this->IPAddress = c_string_init(IPAddress);
	this->Rack = Rack;
	this->Slot = Slot;
	this->DB_index = DB_index;

	return this;
}

bool db_mes_connect(DB_MES * this)
{
	if(Cli_ConnectTo(this->Client, this->IPAddress, this->Rack, this->Slot) == 0)
		return true;
	else
		return false;
}

bool db_mes_disconnect(DB_MES * this)
{
	if(Cli_Disconnect(this->Client) == 0)
		return true;
	else
		return false;
}

void db_mes_finalize(DB_MES * this)
{
	db_mes_disconnect(this);
	Cli_Destroy(&(this->Client));
	free(this->IPAddress);
	free(this);
}


void print_plc_db_content(DB_MES * this)
{
	if(this->DB_PLC != NULL)
	{
		printf("InitialServer - %s\n", ((this->DB_PLC->plcControlByte & PLC_INITIAL_0) > 0  ? "true" : "false"));
		printf("InitialServerAck - %s\n", ((this->DB_PLC->plcControlByte & PLC_INITIAL_SERVER_ACK_1) > 0  ? "true" : "false"));
		printf("CycleRequest - %s\n", ((this->DB_PLC->plcControlByte & PLC_CYCLE_REQUEST_2) > 0  ? "true" : "false"));
		printf("CycleSuccess - %s\n", ((this->DB_PLC->plcControlByte & PLC_CYCLE_SUCCESS_3) > 0  ? "true" : "false"));
		printf("CycleFailure - %s\n", ((this->DB_PLC->plcControlByte & PLC_CYCLE_FAILURE_4) > 0  ? "true" : "false"));
		printf("CycleAbort - %s\n", ((this->DB_PLC->plcControlByte & PLC_CYCLE_ABORT_5) > 0  ? "true" : "false"));

		printf("PartSerialPLC - %s\n", this->DB_PLC->PartSerialPLC);
		printf("ProcessDataCount - %d\n", this->DB_PLC->ProcessDataCount);
		printf("KeyProcessData - %s\n", this->DB_PLC->KeyProcessData);
		printf("ValueProcessData - %s\n", this->DB_PLC->ValueProcessData);
	}
	else
	{
		printf("No Data\n");
	}
}

void print_server_db_content(DB_MES * this)
{
	if(this->DB_SERVER != NULL)
	{
		/*
		char * control_byte = byte_to_bin_string(this->DB_SERVER->serverConstrolByte);
		printf("control byte: %s\n", control_byte);
		free(control_byte);
		printf("InitialPLC - %s\n", ((this->DB_SERVER->serverConstrolByte & SERVER_INITIAL_0) > 0  ? "true" : "false"));
		printf("InitialPLCAck - %s\n", ((this->DB_SERVER->serverConstrolByte & SERVER_INITIAL_PLC_ACK_1) > 0  ? "true" : "false"));
		printf("ReadyForCycle - %s\n", ((this->DB_SERVER->serverConstrolByte & SERVER_READY_FOR_CYCLE_2) > 0  ? "true" : "false"));
		printf("CycleRequestAck - %s\n", ((this->DB_SERVER->serverConstrolByte & SERVER_CYCLE_REQUEST_ACK_3) > 0  ? "true" : "false"));
		printf("CycleRequestReject - %s\n", ((this->DB_SERVER->serverConstrolByte & SERVER_CYCLE_REQUEST_REJECT_4) > 0  ? "true" : "false"));
		printf("CycleAck - %s\n", ((this->DB_SERVER->serverConstrolByte & SERVER_CYCLE_ACK_5) > 0  ? "true" : "false"));
	*/


	}
	else
	{
		printf("No Data\n");
	}
}



int main(int argc, char** argv)
{
//*
	printf("%I64u - %I64u\n", sizeof(DB_MES_PLC), sizeof(DB_MES_SERVER));

	char * Address = "192.168.0.1";
	int Rack = 0;
	int Slot = 1;
	int db_index = 600;

	DB_MES * mes_con = db_men_new(Address, Rack, Slot, db_index);

	if(db_mes_connect(mes_con) == true)
	{
		printf("connected\n");
		if(db_mes_load(mes_con)== false)
			printf("reading error\n");
		print_plc_db_content(mes_con);

		mes_con->DB_SERVER = malloc(sizeof(DB_MES_SERVER));
		mes_con->DB_SERVER->LiveCheck = true;
		mes_con->DB_SERVER->serverConstrolByte = 0;
		mes_con->DB_SERVER->serverConstrolByte |= SERVER_READY_FOR_CYCLE_2;
		strcpy(mes_con->DB_SERVER->ParameterBarcode, "bar code");

		if(db_mes_write(mes_con) == true)
			printf("data written\n");
		else
			printf("writing failure\n");


		db_mes_disconnect(mes_con);
	}
	else
	{
		printf("connection fault");
	}
//*/

	/*
	S7Object Client = Cli_Create();

	if(Cli_ConnectTo(Client, "192.168.0.1", 0, 1) == 0)
	{
		printf("connected\n");

		char db[900];
		Cli_DBRead(Client, 600, 0, 900, db);

		for(int i = 0; i < 900; i++)
		{
			char * control_byte = byte_to_bin_string(db[i]);
			printf("%d - %s\n", i, control_byte);
			free(control_byte);
		}

		Cli_Destroy(&Client);
	}
	else
	{
		printf("connection error\n");
	}
//*/

    return 0;
}






















