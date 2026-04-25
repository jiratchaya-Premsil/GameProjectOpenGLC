
#pragma once
#include <glad/glad.h>
#include <string>

struct UIRenderData {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int texture;
    int vertexCount;
};

UIRenderData createUI(std::string name);
void drawUI(const UIRenderData& ui);
bool isInside(double mx, double my, float x, float y, float w, float h);