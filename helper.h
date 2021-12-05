#ifndef _HELPER_H
#define _HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "multi_threaded_server.h"

void error(char *str);
int cmp_func(const void *first, const void *second);

#endif