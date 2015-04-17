#include "tga.h"
#include "Utility.h"

#if (UTILITY_MACRO == FALSE)

double degree2rad(double angle){
	return angle / 180 * M_PI;
}

#endif

bool load_texture(char *TexName, GLuint TexHandle)
{
    TGAImg Img;        // Image loader

    // Load our Texture
    if (Img.Load(TexName) != IMG_OK)
        return false;

    glBindTexture(GL_TEXTURE_2D, TexHandle); // Set our Tex handle as current

    // Create the texture
    if (Img.GetBPP() == 24)
        glTexImage2D(GL_TEXTURE_2D, 0, 3, Img.GetWidth(), Img.GetHeight(), 0,
        GL_RGB, GL_UNSIGNED_BYTE, Img.GetImg());
    else if (Img.GetBPP() == 32)
        glTexImage2D(GL_TEXTURE_2D, 0, 4, Img.GetWidth(), Img.GetHeight(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, Img.GetImg());
    else
        return false;

    // Specify filtering and edge actions
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    return true;
}

double distance2plane(const Plane& plane, const Vector4& point)
{
    return (point - plane.center).dot(plane.normal);
}

void drawString(void *font, char *s, float x, float y, float z)
{
    unsigned int i;
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen(s); i++)
        glutBitmapCharacter(font, s[i]);
}