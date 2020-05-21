#ifndef _CONTROLER_H_
#define _CONTROLER_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <s7lib.h>

#include "model.h"



struct _controler_;
typedef struct _controler_ controler;

controler * controler_new(model * model_ref);

bool controler_move_glass_to_production_request(controler *, glass_info *);
bool controler_move_priority_glass_to_production_request(controler *, glass_info *);
bool controler_delete_glass_from_production_request(controler *, glass_info *);
bool controler_send_glass_to_gluing_request(controler *, glass_info *);

void controler_finalize(controler *);



#endif
