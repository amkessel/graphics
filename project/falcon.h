#ifndef _FALCON_H_
#define _FALCON_H_

#include "kutils.h"
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#define NUM_FALCON_TEXS 8

#define BOTTOM_TEX       0
#define TOP_TEX          1
#define SIDE_TEX         2
#define AROUND_TEX       3
#define CANOPY_TOP_TEX   4
#define CANOPY_FRONT_TEX 5
#define FORK_TOP_TEX     6
#define FORK_BOTTOM_TEX  7

using namespace kutils;

extern unsigned int falcon_tex[]; // textures
extern char *falcon_tex_names[]; // names of textures

void Draw_falcon();

void Draw_falcon(point3 translation, point4 rotation, point3 scale);

void Draw_falcon(point3 *translations, point4 *rotations, point3 *scales,
				 int ntrans, int nrots, int nscales);
				 				 
void Draw_Braking_Orb(point3 translation, point3 scale, double alpha);

#endif
