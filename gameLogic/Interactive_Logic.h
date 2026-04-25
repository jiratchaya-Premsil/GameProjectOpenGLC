#pragma once
#include <unordered_map>
#include <cmath>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gameLogic/IRenderable.h>
#include <memory>

class Model;
class Shader;



struct Object : public IRenderable{
public:
        std::shared_ptr<Model> model;
        glm::vec3 pos;
        glm::vec3 scale;

        std::string name;
        std::string type;

        void Draw(Shader& shader) override;
        void Update(float deltaTime) override { }  // Static objects don't need updates



        glm::vec3 GetPosition() override { return pos; }

        bool IsAnimated() override { return false; }
        glm::vec3 GetScale() override { return scale; }
        std::string GetName() override { return name; }
        glm::mat4 GetModelMatrix() override {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::scale(model, scale);
            return model;
        }
        std::string GetType() override { return type; }
        void Draw(Shader& shader, float scale);
};
struct TargetResultObj {
    Object* target = nullptr;
    float dot = 1000.0f;
    float distance = 1000.0f;
};

struct CellKey {
    glm::ivec3 pos;
    bool operator==(const CellKey& other) const {
        return pos == other.pos;
    }

};

TargetResultObj updateInteraction(glm::vec3 playerPos, glm::vec3 forward);
void insertObject(Object& obj);
Object createObject(
    const std::string& name,
    const std::string& type,
    const glm::vec3& pos,
    std::shared_ptr<Model> model,
    const glm::vec3& scale
);