#ifndef _FALCON_H_
#define _FALCON_H_

#include "kutils.h"
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "common.h"

#define NUM_FALCON_TEXS 9

#define BOTTOM_TEX       0
#define TOP_TEX          1
#define SIDE_TEX         2
#define AROUND_TEX       3
#define CANOPY_TOP_TEX   4
#define CANOPY_FRONT_TEX 5
#define FORK_TOP_TEX     6
#define FORK_BOTTOM_TEX  7
#define BREAKING_ORB_TEX  8

using namespace kutils;

extern unsigned int falcon_tex[]; // textures
extern char *falcon_tex_names[]; // names of textures

void Draw_falcon(bool thrust_on, thrust_box *tbox);

void Draw_falcon(point3 translation, point4 rotation, point3 scale, bool thrust_on, thrust_box *tbox);

void Draw_falcon(point3 *translations, point4 *rotations, point3 *scales,
				 int ntrans, int nrots, int nscales, bool thrust_on, thrust_box *tbox);
				 				 
void Draw_Braking_Orb(point3 translation, point3 scale, double alpha);

#endif
