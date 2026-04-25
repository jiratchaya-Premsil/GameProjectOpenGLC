
#define NOMINMAX
#include <windows.h>
#include <iostream>

#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <gameLogic/mixing.h>
#include <glm/glm.hpp>
#include <gameLogic/Interactive_Logic.h>
float CELL_SIZE = 10.0f;

class Model;

struct CellKeyHash {
    std::size_t operator()(const CellKey& k) const {
        return (k.pos.x * 73856093) ^
            (k.pos.y * 19349663) ^
            (k.pos.z * 83492791);
    }
};
std::unordered_map<CellKey, std::vector<Object*>, CellKeyHash> grid;
CellKey getCell(const glm::vec3& pos) {
    return CellKey{
        glm::ivec3(glm::floor(pos / CELL_SIZE))
    };
}

void insertObject(Object& obj) {
    CellKey key = getCell(obj.pos);
    grid[key].push_back(&obj);

}



std::vector<Object*> getNearbyObjects(const glm::vec3& pos, int radius = 1) {
    std::vector<Object*>  result;
    CellKey center = getCell(pos);
    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            for (int z = -radius; z <= radius; z++) {
                CellKey key = {
                    center.pos + glm::ivec3(x, y, z)
                };
                auto it = grid.find(key);

                if (it != grid.end()) {
                    result.insert(result.end(),
                        it->second.begin(),
                        it->second.end());
                }
            }
        }
    }

    return result;
}



TargetResultObj updateInteraction(glm::vec3 playerPos, glm::vec3 forward) {

    auto nearby = getNearbyObjects(playerPos, 1);
    float closetD = 0.93f;
    TargetResultObj closetObj;
    float closetdist = 5.0f;
    for (auto* obj : nearby) {
        if (!obj)
            continue;

        glm::vec3 dir = glm::normalize(obj->pos - playerPos);
        float dist = glm::length(obj->pos - playerPos);
        float d = glm::dot(forward, dir);

        if (dist > closetdist)
            continue;


        


        if (d > closetD) {
            closetObj.target = obj;
            closetObj.dot = d;
            closetObj.distance = dist;
            closetD = d;

            }


    }
    return closetObj;
}

Object createObject(
    const std::string& name,
    const std::string& type,
    const glm::vec3& pos,
    std::shared_ptr<Model> model,
    const glm::vec3& scale

) {
    Object obj;
    obj.name = name;
    obj.type = type;
    obj.pos = pos;
    obj.model = model;
    obj.scale = scale;

    return obj;
}

void Object::Draw(Shader& shader)
{
    Draw(shader, 1.0f);
}

void Object::Draw(Shader& shader, float extraScale)
{
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, pos);
    modelMat = glm::scale(modelMat, scale * extraScale);

    shader.setMat4("model", modelMat);
    model->Draw(shader);
}
