#ifndef ARRAY_QUEUE_H
#define ARRAY_QUEUE_H

#include "structures.h"

void array_push(discriptor* d, char* qu, char c);
char array_pop(discriptor* d, char* qu);
void array_print(discriptor* d, char* qu);

void impliment_array(int n, int interval, time_range t1, time_range t2, time_range t3, time_range t4);

#endif
