#ifndef _UNIT_H_
#define _UNIT_H_


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct _unit_;
typedef struct _unit_ unit;

unit * unit_new(char *, int, int, int);
bool unit_run_unit_test(unit *);
bool unit_initialize_plcut(unit *);
bool unit_set_plcut_enabled(unit *);
bool unit_set_plcut_disabled(unit *);
void unit_finalize(unit *);

#endif
