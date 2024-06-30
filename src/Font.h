#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <map>

/// Holds all state information relevant to a character as loaded using FreeType
struct Character 
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class Font
{
public:
    Font(std::string filePath);

    std::map<GLchar, Character>& GetCharacters();

private:
    std::map<GLchar, Character> characters;
};

#endif