#ifndef TIMEROTATION_H_INCLUDED
#define TIMEROTATION_H_INCLUDED

/*
 *     timerotation.h
 *     by Theo Dayton and Thomson Brand, 10/15/2021
 *     Locality and the costs of loads and stores
 *
 *     Contains interface for the timerotation function, whose purpose is to
 *     time how long cpu takes for 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "ppmrotate.h"

void timeRotation(Pnm_ppm pixmap, A2Methods_T methods, A2Methods_mapfun *map, int rotation, char *time_file_name, int width, int height);

#undef TIMEROTATION_H_INCLUDED
#endif