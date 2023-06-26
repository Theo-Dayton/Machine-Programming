#include "tul.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void read_three_unsigned_longs(const char *inputline,struct three_unsigned_longs *p) {
    sscanf(inputline, "%lu %lu %lu", &(p->a), &(p->b), &(p->c));
    return;
}