#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:
    Texture(unsigned char* data, int width, int height);
    GLuint getID();

private:
    GLboolean IsExtensionSupported(const char* name);

    GLuint ID = 0;
};

#endif