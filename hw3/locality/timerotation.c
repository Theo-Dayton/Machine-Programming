/*
 *     timerotation.c
 *     by Theo Dayton and Thomson Brand, 10/15/2021
 *     Locality and the costs of loads and stores
 *
 *     Contains interface for the timerotation function, whose purpose is to
 *     time how long cpu takes for 
 */

#include "cputiming.h"
#include "timerotation.h"

/* 
 * timeRotation applies a rotation to the pixmap given, and measures various aspects concerning
 * the runtime of the rotation, such as time taken for the rotation and for each pixel.
*/

void timeRotation(Pnm_ppm pixmap, A2Methods_T methods, A2Methods_mapfun *map, int rotation, char *time_file_name, int width, int height)
{
	CPUTime_T timer;
	double time_used; 

	timer = CPUTime_New();

	CPUTime_Start(timer);
	ppmrotate(pixmap, methods, map, rotation, width, height);
	time_used = CPUTime_Stop(timer);
	FILE *infile;
	infile = fopen(time_file_name, "w+");
	fprintf (infile, "Rotation of %d degrees was computed in %.0f nanoseconds\n", rotation, time_used);
	double widthXheight = width * height;
	double pixelTime = time_used / widthXheight;
	fprintf (infile, "Size of image: %.0f \n", widthXheight);
	fprintf (infile, "Time taken for each pixel is %.0f nanoseconds", pixelTime);
	fclose(infile);
	CPUTime_Free(&timer);
}

