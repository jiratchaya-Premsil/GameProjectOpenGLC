#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <gameLogic/IRenderable.h>
#include <glm/gtc/matrix_transform.hpp>
class AnimationModel;
class Shader;
class Animation;
class Animator;

enum class CustomerState
{
    Walking,
    Greeting,
    Idle,
    Leaving
};




class Customer : public IRenderable
{
public:
    Customer();
    void Init(const glm::vec3& startPos, const glm::vec3& targetPos,
        AnimationModel* sharedModel,
        Animation* walkAnim,
        Animation* greetAnim,
        Animation* idleAnim);

    void Update(float deltaTime);
    void Draw(Shader& shader)override;
    void Draw(Shader& shader, float scale);
    void Interact();
    void Respawn();
    Animator* GetAnimator() { return animator.get(); }
    CustomerState getState() { return state; }

    glm::vec3 GetPosition() override { return position; }
    glm::vec3 GetScale() override { return glm::vec3(1.0f); }
    std::string GetName() override { return name; }
    std::string GetType() override { return "Customer"; }
    bool IsAnimated() override { return true; }
    glm::mat4 GetModelMatrix() override {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(1.0f));
        return model;
    }




    std::string order;
    std::string name;
    bool greeted;
    bool readyToLeave;
    std::string speach;
    void ChangeState(CustomerState newState);
private:
    void MoveTo(const glm::vec3& target, float deltaTime);



private:
    glm::vec3 position;
    glm::vec3 targetA;
    glm::vec3 targetB;
    glm::vec3 spawnPosition;  // Original spawn position for exit calculation



    float speed;

    // State
    CustomerState state;
    float stateTimer;



    AnimationModel* baseModel;

    std::shared_ptr<AnimationModel> clothModel;
    std::shared_ptr<AnimationModel> pantsModel;
    // std::shared_ptr<AnimationModel> OtherModel;

    std::vector<std::shared_ptr<AnimationModel>> clothOptions;
    std::vector<std::shared_ptr<AnimationModel>> pantsOptions;

    Animation* walkAnimation;
    Animation* greetAnimation;
    Animation* idleAnimation;

    std::unique_ptr<Animator> animator;

};