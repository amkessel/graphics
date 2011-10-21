#ifndef _FALCON_H_
#define _FALCON_H_

#define NUM_TEXS 8

#define BOTTOM_TEX       0
#define TOP_TEX          1
#define SIDE_TEX         2
#define AROUND_TEX       3
#define CANOPY_TOP_TEX   4
#define CANOPY_FRONT_TEX 5
#define FORK_TOP_TEX     6
#define FORK_BOTTOM_TEX  7

extern unsigned int falcon_tex[]; // textures
extern char *falcon_tex_names[]; // names of textures

void Draw_falcon();

#endif
