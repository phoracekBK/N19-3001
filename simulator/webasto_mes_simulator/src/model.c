/*
 * model.c
 *
 *  Created on: 12. 12. 2019
 *      Author: HORACEK
 */

#include "include/model/model.h"


struct _model_
{

};

model * model_new()
{
	model * this = malloc(sizeof(model));


	return this;
}

void model_finalize(model * this)
{
	free(this);
}
