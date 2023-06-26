#ifndef PPMROTATE_H_INCLUDED
#define PPMROTATE_H_INCLUDED

/*
 *     ppmrotate.h
 *     by Theo Dayton and Thomson Brand, 10/15/2021
 *     Locality and the costs of loads and stores
 *
 *     Contains interface for the ppmrotate function, whose purpose is to
 *     read in a file and write the resulting image to stdout after rotating
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

void ppmrotate(Pnm_ppm pixmap, A2Methods_T methods, A2Methods_mapfun *map, int rotate, int width, int height);

#undef PPMROTATE_H_INCLUDED
#endif