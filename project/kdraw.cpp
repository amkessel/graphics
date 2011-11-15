#include "kutils.h"
#include "kdraw.h"

using namespace kutils;
using namespace kdraw;

point3 kdraw::ManualTransformAboutY(point3 translation, point4 rotation, point3 scale, point3 pt)
{
	point3 result;
	
	result.x =  scale.x*pt.x*KUTILS_COS(rotation.w) + scale.z*pt.z*KUTILS_SIN(rotation.w) + translation.x;
	result.y =  scale.y*pt.y + translation.y;
	result.z = -scale.x*pt.x*KUTILS_SIN(rotation.w) + scale.z*pt.z*KUTILS_COS(rotation.w) + translation.z;
	
	return result;
}
	 
void kdraw::Transform(point3 *translations, point4 *rotations, point3 *scales,
	  int ntrans, int nrots, int nscales)
{
	for(int i = 0; i < ntrans; i++)
		glTranslated(translations[i].x, translations[i].y, translations[i].z);
	for(int i = 0; i < nrots; i++)
		glRotated(rotations[i].w, rotations[i].x, rotations[i].y, rotations[i].z);
	for(int i = 0; i < nscales; i++)
		glScaled(scales[i].x, scales[i].y, scales[i].z);
}

void kdraw::Transform(point3 *translations, point3 *scales, int ntrans, int nscales)
{
	point4 rotation = {0,0,0,0};
	point4 rotations[1] = { rotation };
	Transform(translations, rotations, scales, ntrans, 1, nscales);
}

void kdraw::Transform(point3 translation, point4 rotation, point3 scale)
{
	point3 translations[1] = { translation };
	point4 rotations[1] = { rotation };
	point3 scales[1] = { scale };
	
	kdraw::Transform(translations, rotations, scales, 1, 1, 1);
}

void kdraw::Transform(point3 translation, point3 scale)
{
	point4 rotation = {0,0,0,0};	
	kdraw::Transform(translation, rotation, scale);
}

void kdraw::DrawAxes(float r=1, float g=1, float b=1, float len=1)
{
	//  Draw axes in white
	glColor3f(r,g,b);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(len,0,0);
	glVertex3d(0,0,0);
	glVertex3d(0,len,0);
	glVertex3d(0,0,0);
	glVertex3d(0,0,len);
	glEnd();
	//  Label axes
	glRasterPos3d(len,0,0);
	Print("X");
	glRasterPos3d(0,len,0);
	Print("Y");
	glRasterPos3d(0,0,len);
	Print("Z");
}

#define KDRAW_LEN 8192  // Maximum length of text string
void kdraw::Print(const char* format , ...)
{
   char    buf[KDRAW_LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,KDRAW_LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void kdraw::Project(double fov,double asp,double dim)
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (fov)
      gluPerspective(fov,asp,dim/16,16*dim);
   //  Orthogonal transformation
   else
      glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}
