/*
 * view_interface.h
 *
 *  Created on: 12. 12. 2019
 *      Author: HORACEK
 */

#ifndef INCLUDE_VIEW_INTERFACE_H_
#define INCLUDE_VIEW_INTERFACE_H_

#include "../model/model.h"
#include "../controler/controler.h"


#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

struct _view_;
typedef struct _view_ view;


view * view_new(int *, char***, controler *, model *);
void view_main(view *);
void view_main_exit(view *);
void view_finalize(view *);

#endif /* INCLUDE_VIEW_INTERFACE_H_ */
