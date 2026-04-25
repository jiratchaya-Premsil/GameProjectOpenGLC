#pragma once
#include <string>
struct UIElement {
    float* vertices;
    int vertexCount;
    std::string texturePath;
};

UIElement getUI(std::string name);

