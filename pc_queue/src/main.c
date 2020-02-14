
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "include/controler.h"

#define IP_ADDRESS "192.168.0.1"
#define RACK 0
#define SLOT 1
#define DB_INDEX 5


int main(int argv, char ** argc)
{
	HWND var = GetConsoleWindow();

	if(argv > 1 && strcmp(argc[1], "-d") != 0)
		ShowWindow(var, SW_HIDE);

	controler * controler_ref = controler_new(IP_ADDRESS, RACK, SLOT, DB_INDEX);
	controler_handler(controler_ref);

	if(argv > 1 && strcmp(argc[1], "-d") != 0)
		ShowWindow(var, SW_SHOW);
	

	return 0;
}

