#pragma once
#include <map>
#include <string>
#include <glad/glad.h>

struct Glyph {
    float u1, v1;
    float u2, v2;
};

class TextRenderer {
public:
    GLuint texture;
    GLuint VAO, VBO;

    std::map<char, Glyph> fontMap;

    void init();
    void drawText(std::string text, float x, float y, float scale);
};