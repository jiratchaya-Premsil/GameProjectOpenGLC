#include "ui_Info.h"
#include <string>
// vertices
//left = posx
//right = posx + w 
//bottom = posy
//top = posy + b

//float temp[] = {
//        left,  bottom, 0.0f, 0.0f, 0.0f,
//        right, bottom, 0.0f, 1.0f, 0.0f,
//        right, top,    0.0f, 1.0f, 1.0f,
//
//        left,  bottom, 0.0f, 0.0f, 0.0f,
//        right, top,    0.0f, 1.0f, 1.0f,
//        left,  top,    0.0f, 0.0f, 1.0f
//};
static float bgGameScreenVert[] = {
    0, 0, 0.0f, 0.0f, 0.0f,
    975, 0, 0.0f, 1.0f, 0.0f,
    975, 624, 0.0f, 1.0f, 1.0f,

    0, 0, 0.0f, 0.0f, 0.0f,
    975, 624, 0.0f, 1.0f, 1.0f,
    0, 624, 0.0f, 0.0f, 1.0f
};

static float startBtnVert[] = {
    60,  330, 0.0f, 0.0f, 0.0f,
    304, 330, 0.0f, 1.0f, 0.0f,
    304, 420, 0.0f, 1.0f, 1.0f,

    60,  330, 0.0f, 0.0f, 0.0f,
    304, 420, 0.0f, 1.0f, 1.0f,
    60,  420, 0.0f, 0.0f, 1.0f
};

static float exitBtnVert[] = {
    60,  430, 0.0f, 0.0f, 0.0f,
    304, 430, 0.0f, 1.0f, 0.0f,
    304, 520, 0.0f, 1.0f, 1.0f,

    60,  430, 0.0f, 0.0f, 0.0f,
    304, 520, 0.0f, 1.0f, 1.0f,
    60,  520, 0.0f, 0.0f, 1.0f
};

std::string bgGameTexPath = "resources/textures/ui/scene/startgameBG.png";
std::string StartBtnTexPath = "resources/textures/ui/buttons/PlayButton.png";
std::string exitBtnTexPath = "resources/textures/ui/buttons/QuitButton.png";

UIElement getUI(std::string name)
{
    if (name == "Gamebg")
        return { bgGameScreenVert, 6, bgGameTexPath };

    if (name == "StartBtn")
        return { startBtnVert, 6, StartBtnTexPath };

    if (name == "ExitBtn")
        return { exitBtnVert, 6, exitBtnTexPath };

    return { nullptr, 0, "" };
}