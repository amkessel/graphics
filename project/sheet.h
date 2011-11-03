#ifndef _SHEET_H_
#define _SHEET_H_

#include "kutils.h"

#define SHEET_PTS 100 // how many points along the side of the sheet are there
#define SHEET_DIST 0.1 // how far are the points from one another in the x and z dir
#define SHEET_COLOR 0.7
#define GRID_COLOR 0.1

#define GRAV_FACTOR 2.0 // factor to add emphasis to gravity well

#define GRAV_CUTOFF 10.0 // multiple of radius of planet afterwhich approximate potential as zero

using namespace kutils;

void Draw_sheet(point3 pts[SHEET_PTS][SHEET_PTS], point3 norms[SHEET_PTS][SHEET_PTS], bool sheet, bool grid);

void Calculate_sheet_points(point3 pts[SHEET_PTS][SHEET_PTS], point3 falcon_pos);

void Calculate_sheet_normals(point3 pts[SHEET_PTS][SHEET_PTS], point3 norms[SHEET_PTS][SHEET_PTS]);

void Calculate_sheet_heights(point3 pts[SHEET_PTS][SHEET_PTS], point3 sun, point3 earth, point3 moon);

#endif
