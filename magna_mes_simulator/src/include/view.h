#ifndef _VIEW_H_
#define _VIEW_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <c_string.h>
#include <c_trie.h>

#include "model.h"
#include "controler.h"

struct _view_;
typedef struct _view_ view;


view * view_new(model *, controler *);
void view_cli_run(view *);
void view_finalize(view *);


#endif
