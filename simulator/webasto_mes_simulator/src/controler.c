/*
 * controler.c
 *
 *  Created on: 12. 12. 2019
 *      Author: HORACEK
 */


#include "include/controler/controler.h"
#include "include/controler/snap7.h"

#include <gtk/gtk.h>
#include <unistd.h>

#include <ctype.h>
#ifdef OS_WINDOWS
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif



struct _controler_
{
	model * mode_ref;
	S7Object Client;

	bool main_loop_execution;
	GThread * main_loop;
};


static gpointer controler_main(gpointer);
static void controler_loop(controler *);
static void controler_setup(controler *);



controler * controler_new(model * model_ref)
{
	controler * this = malloc(sizeof(controler));

	this->main_loop_execution = true;
	this->main_loop = g_thread_new("main_loop", controler_main, this);

	return this;
}

void controler_finalize(controler * this)
{

}

static void controler_setup(controler * this)
{

}

bool controler_main_loop_is_runnig(controler * this)
{
	return this->main_loop_execution;
}

void controler_main_loop_exit(controler * this)
{
	this->main_loop_execution = false;
}

static gpointer controler_main(gpointer param)
{
	controler * this = (controler *) param;

	controler_setup(this);

	do
	{
		controler_loop(this);
		usleep(CYCLE_CLOCK_TIME);
	}
	while(this->main_loop_execution == true);

	return NULL;
}

static void controler_loop(controler * this)
{

}
