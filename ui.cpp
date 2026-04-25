#include "ui.h"
#include "ui_Info.h"
#include <string>
#include <iostream>
#include "stb_image.h"


UIRenderData createUI(std::string name)
{
    UIElement data = getUI(name);

    UIRenderData ui;

    // ===== VAO / VBO =====
    glGenVertexArrays(1, &ui.VAO);
    glGenBuffers(1, &ui.VBO);

    glBindVertexArray(ui.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, ui.VBO);
    glBufferData(GL_ARRAY_BUFFER,
        data.vertexCount * 5 * sizeof(float),
        data.vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ===== TEXTURE =====
    glGenTextures(1, &ui.texture);
    glBindTexture(GL_TEXTURE_2D, ui.texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, ch;
    //std::cout << "loading texture" << data.texturePath << std::endl;

    unsigned char* img = stbi_load(
        data.texturePath.c_str(),
        &w, &h, &ch, 4
    );

    if (img)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture\n";
    }

    stbi_image_free(img);

    ui.vertexCount = data.vertexCount;

    return ui;
}


void drawUI(const UIRenderData& ui)
{
    glBindVertexArray(ui.VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ui.texture);

    glDrawArrays(GL_TRIANGLES, 0, ui.vertexCount);
}

bool isInside(double mx, double my, float x, float y, float w, float h)
{
    return mx >= x && mx <= x + w &&
        my >= y && my <= y + h;
}