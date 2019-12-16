/*
 * app.h
 *
 *  Created on: 12. 12. 2019
 *      Author: HORACEK
 */

#ifndef INCLUDE_APP_H_
#define INCLUDE_APP_H_


#include "../model/model.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define CYCLE_CLOCK_TIME 100000 //100ms


struct _controler_;
typedef struct _controler_ controler;


controler * controler_new(model *);
bool controler_main_loop_is_runnig(controler *);
void controler_main_loop_exit(controler *);
void controler_finalize(controler *);

#endif /* INCLUDE_APP_H_ */
