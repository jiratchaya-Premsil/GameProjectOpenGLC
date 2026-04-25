#pragma once
#include <glm/glm.hpp>

class Shader;

class IRenderable
{
public:
    virtual ~IRenderable() = default;

    virtual void Draw(Shader& shader) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual bool IsAnimated() = 0;
    virtual glm::mat4 GetModelMatrix() = 0;

    virtual glm::vec3 GetPosition() = 0;
    virtual glm::vec3 GetScale() = 0;
    virtual std::string GetName() = 0;
    virtual std::string GetType() = 0;
};