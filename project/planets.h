#ifndef _PLANETS_H_
#define _PLANETS_H_

#include "kutils.h"
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#define NUM_PLANET_TEXS 4

#define SUN_TEX     0
#define EARTH_TEX   1
#define MOON_TEX    2
#define JUPITER_TEX 3

#define SUN_RAD     1
#define EARTH_RAD   0.2
#define MOON_RAD    0.05
#define JUPITER_RAD 0.5
#define COMET_RAD   0.01

#define COMET_TAIL_LEN 10

#define SUN_MASS     1
#define EARTH_MASS   0.2
#define MOON_MASS    0.05
#define JUPITER_MASS 0.35

#define CORONA_SPACING    0.05
#define CORONA_LENGTH     1.4
#define CORONA_BASE_ALPHA 0.05

using namespace kutils;

extern unsigned int planet_tex[]; // textures
extern char *planet_tex_names[]; // names of textures

void Draw_Sun(point3 translation, point4 rotation, point3 scale);

void Draw_Earth(point3 translation, point4 rotation, point3 scale);

void Draw_Moon(point3 earth_trans, point3 translation, point4 rotation, point3 scale);

void Draw_Jupiter(point3 translation, point4 rotation, point3 scale);

// tail_len: in terms of radius of comet (e.g. 10x radius)
void Draw_Comet(point3 translation, point4 rotation, point3 scale, double tail_len, double coma_alpha);

void Calculate_Comet_Params(double x, double z, double orbit_rad, double *coma_alpha, double *tail_len, double *rot_angle);

void Draw_Corona(point3 translation, point4 rotation, point3 scale);

#endif
