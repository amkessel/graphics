#ifndef _PLANETS_H_
#define _PLANETS_H_

#include "kutils.h"
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#define NUM_PLANET_TEXS 3

#define SUN_TEX   0
#define EARTH_TEX 1
#define MOON_TEX  2

#define SUN_RAD   1
#define EARTH_RAD 0.2
#define MOON_RAD  0.05

#define SUN_MASS   1
#define EARTH_MASS 0.2
#define MOON_MASS  0.05

using namespace kutils;

extern unsigned int planet_tex[]; // textures
extern char *planet_tex_names[]; // names of textures

void Draw_Sun(point3 translation, point4 rotation, point3 scale);

void Draw_Earth(point3 translation, point4 rotation, point3 scale);

void Draw_Moon(point3 earth_trans, point3 translation, point4 rotation, point3 scale);

void Draw_Comet(point3 translation, point4 rotation, point3 scale);

#endif
