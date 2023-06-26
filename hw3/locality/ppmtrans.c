#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "ppmrotate.h"
#include "cputiming.h"
#include "timerotation.h"

/*
 *     ppmrotate.c
 *     by Theo Dayton and Thomson Brand, 10/15/2021
 *     Locality and the costs of loads and stores
 *
 *     Driver file for the ppmtrans program, whose purpose is to
 *     read in a ppm file, transform it, and export its result to
 *     stdout
 */

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

int main(int argc, char *argv[]) 
{
        char *time_file_name = NULL;
        (void) time_file_name;
        int   rotation       = 0;
        int   i;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        
        FILE *infile;
        infile = fopen(argv[argc-1], "r");
        Pnm_ppm pixmap = Pnm_ppmread(infile, methods);
        int pixmapWidth = methods->width(pixmap);
        int pixmapHeight = methods->height(pixmap);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                                    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                                        "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                        ppmrotate(pixmap, methods, map, rotation, 
                                  pixmapWidth, pixmapHeight);
                } else if (strcmp(argv[i], "-flip") == 0) {
                        fprintf(stderr, "Unimplemented \n");
                        exit(1);
                } else if (strcmp(argv[i], "-tranpose") == 0) {
                        fprintf(stderr, "Unimplemented \n");
                        exit(1);
                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                        timeRotation(pixmap, methods, map,
                        rotation,time_file_name, pixmapWidth, pixmapHeight);
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                                argv[i]);
                        usage(argv[0]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {  
                        break;
                }
        }
        Pnm_ppmfree(&pixmap);
        fclose(infile);
        return 0;
}
