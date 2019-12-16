/*
 * model.h
 *
 *  Created on: 12. 12. 2019
 *      Author: HORACEK
 */

#ifndef INCLUDE_MODEL_H_
#define INCLUDE_MODEL_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


struct _model_;
typedef struct _model_ model;


model * model_new();
void model_finalize(model *);


#endif /* INCLUDE_MODEL_H_ */
