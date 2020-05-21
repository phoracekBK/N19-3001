#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include <s7lib.h>

#include "include/controler.h"
#include "include/config.h"


int main(int argv, char ** argc)
{
  s7lib * s7lib_ref = s7lib_new(IP_ADDRESS, RACK, SLOT, DB_INDEX);

  if(s7lib_ref != NULL)
  {
    while(true)
    {
        uint8_t * buffer =  s7lib_read(s7lib_ref, 0, DB_SIZE);

        if(buffer != NULL)
        {
          buffer = controler_check_store_request(buffer, CSV_PATH, CSV_SEPARATOR);
          s7lib_write(s7lib_ref, buffer, 0, 69);
          free(buffer);
        }

        sleep(1);
    }
  }
  else
  {
      printf("S7lib initialization error!\n");
  }

  return 0;
}
