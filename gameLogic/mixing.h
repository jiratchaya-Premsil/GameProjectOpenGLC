#pragma once
#include <map>
#include <vector>
#include <string>
#include <set>
#define NOMINMAX
#include <Windows.h>
#include <gameLogic/Interactive_Logic.h>

#include <learnopengl/animation.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>
#include <gameLogic/customer.h>
#include <array>
using namespace std;
class Model;
class Customer;
class AnimationModel;
class Animation;
class Animator;

struct cup {
    string cupName = "empty cup";
    bool isIced;
    map<string, vector<string>> ingridents;
    bool equiped;
};
struct TargetResult {
    Customer* target = nullptr;
    float dot = 1000.0f;
    float distance = 1000.0f;
};

class Gamesystem {
public:  
    AnimationModel model;

    Animation walkAnim;
    Animation greetAnim;
    Animation idleAnim;

    Gamesystem()
        : model("resources/animationmodel/Ch46_nonPBR.dae"),
        walkAnim("resources/animation/Walking.dae", &model),
        greetAnim("resources/animation/Standing_Greeting.dae", &model),
        idleAnim("resources/animation/Idle.dae", &model)
    {}

    std::array<Customer, 2> customers;
    int money = 0;
    cup currentCup;
    void initCup();
    void initCustomer();
    void addIngrident(string ingrident, string type);
    string getName();
    void Interacted(string name, string type);
    TargetResult FindNearestCustomer(glm::vec3 playerPos, glm::vec3 forward);
    
};