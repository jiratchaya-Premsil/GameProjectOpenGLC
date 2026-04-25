
#include <iostream>
#include <glad/glad.h>
#include "text.h"
#include "stb_image.h"
#include <cctype>
#include <vector>
const int spacing_Letter = 5;

float widthPx = 22.0;
float highPx = 21.0;
int borderPx = 0;

int picwidth = 286;
int pichight = 63;

std::map <char, Glyph> fontMap;

std::string fontPath = "resources/fonts/customeFontBitmap.png";

void TextRenderer::init() {
	//load texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int w, h, ch;
	unsigned char* data = stbi_load(fontPath.c_str(), &w, &h, &ch, 4);
	if (!data) {
		std::cout << "failed to load texture" << std::endl;
		return;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	// setup quad buffer (dynamic)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// simple grid atlas (16x16)
	

	int column = 13;
	
	for (int i = 0; i < 26; i++)
	{
		int col = i % column;   // 0 → 12
		int row = i / column;   // 0 or 1

		float  x =   col * (widthPx );
		float  y = row * (highPx +row-1);

		float  endX = x + widthPx;
		float  endY = y + highPx;

		Glyph g;
		g.u1 = x / picwidth;
		g.v1 = y / pichight;
		g.u2 = endX / picwidth;
		g.v2 = endY / pichight;

		fontMap[(char)(i + 65)] = g;
	}
	std::vector<char> intandStuff = {
		',', '?','0', '1','2','3','4','5','6','7','8','9'
	};
	for (int i = 0; i < intandStuff.size(); i++)
	{
		int col = i % column;   // 0 → 12
		int row = 2;   // 0 or 1

		float  x = col * (widthPx);
		float  y = row * (highPx + row - 2);

		float  endX = x + widthPx;
		float  endY = y + highPx;

		Glyph g;
		g.u1 = x / picwidth;
		g.v1 = y / pichight;
		g.u2 = endX / picwidth;
		g.v2 = endY / pichight;

		fontMap[intandStuff[i]] = g;
	}

}


void TextRenderer::drawText(std::string text, float x, float y, float scale) {
	float originx = x;
	for (char& c : text) {
		c = std::toupper(static_cast<unsigned char>(c));
	}

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	for (char c : text) {
		if (c == ' ') {
					x += widthPx;
					continue;
				}
		if (c == '_') {
			x = originx;
			y += (highPx * scale) + 10;
			continue;
		}
		if (fontMap.find(c) == fontMap.end()) {
			std::cout << "unable to find letter " << c << std::endl;
		}
		
		

		Glyph g = fontMap[c];
		float w = widthPx * scale;
		float h = highPx * scale;
		float vertices[] = {
			x,     y,     g.u1, g.v1,
			x + w,   y,     g.u2, g.v1,
			x + w,   y + h,   g.u2, g.v2,

			x,     y,     g.u1, g.v1,
			x + w,   y + h,   g.u2, g.v2,
			x,     y + h,   g.u1, g.v2
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += w + spacing_Letter;


	}


}