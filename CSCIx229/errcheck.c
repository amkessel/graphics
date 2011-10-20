/*
 *  Check for OpenGL errors
 */
#include "CSCIx229.h"

void ErrCheck(char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}
