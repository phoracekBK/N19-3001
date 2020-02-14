#ifndef _PLC_H_
#define _PLC_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <c_string.h>
#include <unistd.h>

struct _plc_;
typedef struct _plc_ plc;



plc * plc_new(char *, int, int, int);
bool plc_connect(plc *);
bool plc_disconnect(plc *);
bool plc_reconnect(plc *, uint8_t);
void plc_finalize(plc *);
uint8_t * plc_read(plc *, int, int);
bool plc_write(plc *, uint8_t *, int, int);

bool plc_read_bool(plc *, int, int);
bool plc_write_bool(plc *, int, int, bool);
int16_t plc_read_int(plc *, int);
bool plc_write_int(plc *, int, int16_t);
bool plc_write_dword(plc *, int, int32_t);
uint32_t plc_read_dword(plc *, int);
bool plc_write_string(plc *, int, char *, uint8_t);
char * plc_read_string(plc *, int, uint8_t);

bool plc_read_bool_from_array(uint8_t *, int, int);
uint8_t * plc_write_bool_to_array(uint8_t *, int, int, bool);
int16_t plc_read_int_from_array(uint8_t *, int);
uint8_t * plc_write_int_to_array(uint8_t *, int, int16_t);
uint8_t * plc_write_dword_to_array(uint8_t *, int, int32_t);
uint32_t plc_read_dword_from_array(uint8_t *, int);
uint8_t * plc_write_string_to_array(uint8_t *, int, char *, uint8_t);
char * plc_read_string_from_array(uint8_t *, int, uint8_t);


#endif
