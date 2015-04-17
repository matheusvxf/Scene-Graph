#pragma once

#include "Config.h"
#include "World.h"
#include "Vector4.h"

#include <cmath>
#include "GL/glut.h"


#define M_PI 3.14159265358979323846

#if (UTILITY_MACRO == TRUE)

#define degree2rad(angle) (angle / 180.0 * M_PI)
#define rad2degree(rad) (180.0 * rad / M_PI )
#define foreach(v, it) for(decltype(v.begin()) it = v.begin(); it != v.end(); ++it)
#define rand_int(min, max) (rand() % (max - min) + min)
#define min_fov(height, distance) rad2degree(atan2(height, distance));
#define draw_line(x1, y1, z1, x2, y2, z2) \
    glBegin(GL_LINES); \
    glVertex3f((x1), (y1), (z1)); \
    glVertex3f((x2), (y2), (z2)); \
    glEnd();

#else

/*******************************************************************************
*
*	Function		degree2rad
*		
*	Convert from degrees to radians
*
*	angle[in]:
*
*	Return	the angle in radians
*
*******************************************************************************/
double degree2rad(double angle);

#endif /* UTILITY_MACRO */

extern bool load_texture(char *TexName, GLuint TexHandle);
extern double distance2plane(const Plane& plane, const Vector4& point);
extern void drawString(void *font, char *s, float x, float y, float z);