#ifndef _CONTROLER_H_
#define _CONTROLER_H_

#include <c_linked_list.h>
#include <c_queue.h>
#include <stdlib.h>
#include <unistd.h>
#include <s7lib.h>


#include "model.h"
#include "glass_info.h"

#define QUEUE_FILE_PATH "queue.dat"

struct _controler_;
typedef struct _controler_ controler;


controler * controler_new(char *, int, int, int);
void controler_handler(controler *);
void controler_finalize(controler *);


#endif
