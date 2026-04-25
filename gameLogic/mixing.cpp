
#include<iostream>
#include <limits>
#include <gameLogic/mixing.h>
#include <learnopengl/model.h>
#include <gameLogic/customer.h>

#include <learnopengl/animation.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>
#include <cstdlib> // for rand
#include <ctime> // for time
using namespace std;
map<string, vector<string>> recipyMap = {
	{"Latte", {"Espresso", "Milk"}},
	{"Hot Chocolate", {"Chocolate", "Milk"}},
	{"Double espresso", {"Espresso", "Espresso"} },
};

vector<string> syrup = {
"Banana",
"Stawberry"

};

vector<string> orderForCustomer= {
    "Latte", "Milk", "Espresso", "Coffee","Double espresso","Hot Chocolate"
};





string makeorder() {
    string e = "";
    int isice = std::rand() % 2;
    if (isice == 1) {
        e += "Iced";
    }
    int ranmain = std::rand() % orderForCustomer.size();

    int erm = std::rand() % 2;
    if (erm == 1) {
        int ransyrup = std::rand() % syrup.size();
        if (e != "") {
            e += " ";
       }
        e += syrup[ransyrup];
        }
    if (e != "") { e += " "; }
    e += orderForCustomer[ranmain];
    return e;
}

void Gamesystem::initCustomer() {

    glm::vec3 startPos1 = glm::vec3(1.0f, 0.0f, 4.0f);
    glm::vec3 endPos1 = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 startPos2 = glm::vec3(1.8f, 0.0f, 4.0f);
    glm::vec3 endPos2 = glm::vec3(1.8f, 0.0f, 0.0f);

    customers[0].Init(startPos1, endPos1, &model, &walkAnim, &greetAnim, &idleAnim);
    customers[1].Init(startPos2, endPos2, &model, &walkAnim, &greetAnim, &idleAnim);

    customers[0].name = "customer0";
    customers[1].name = "customer1";

    customers[0].order = makeorder();
    customers[1].order = makeorder();
}

void Gamesystem::initCup() {
    currentCup.cupName = "Empty Cup";
    currentCup.isIced = false;
    currentCup.ingridents = {
        {"main", {}},
        {"syrup", {}}
    };
    currentCup.equiped = false;
}

void Gamesystem::Interacted(string name, string type) {

    if (name == "Cup") {
        currentCup.equiped = true;

        return;
    }

    if (type == "Customer") {
        std::cout << "talked to customer!\n";
        if (name == "customer0") {
            //customer 0

            if (!customers[0].greeted) {
                customers[0].speach = "hello, can i have_" + customers[0].order;
                customers[0].Interact();
                return;
            }
            if (currentCup.equiped) {
                std::cout << customers[0].order<< "|" << std::endl << currentCup.cupName << "|" << std::endl;
                if (customers[0].order == currentCup.cupName) {
                    customers[0].speach = "thank you";
                    money += 5;
                }
                else {
                    customers[0].speach = "this isnt what i order";
                }
                customers[0].Interact();
                customers[0].order = makeorder();
                
            }
        }
        else {
            //customer 1

            if (!customers[1].greeted) {
                customers[1].speach = "hello, can i have_" + customers[1].order;
                customers[1].Interact();
                return;
            }
            if (currentCup.equiped) {
                std::cout << customers[1].order << "|" << std::endl << currentCup.cupName << "|" << std::endl;
                if (customers[1].order == currentCup.cupName) {
                    customers[1].speach = "thank you";
                    money += 5;
                }
                else {
                    customers[1].speach = "this isnt what i order";
                }
                customers[1].Interact();
                customers[1].order = makeorder();
            }
        }
        initCup();
        return;
     }


    if (currentCup.equiped) {
        addIngrident(name, type);
    }


}

TargetResult FindTarget(Customer* c, glm::vec3 playerPos, glm::vec3 forward, float maxDistance)
{
    TargetResult result;
    glm::vec3 customerpos = c->GetPosition();
    customerpos += glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 toCustomer = customerpos - playerPos;


    float distance = glm::length(toCustomer);

    if (distance > maxDistance)
        return result;

    glm::vec3 dir = glm::normalize(toCustomer);
    float dot = glm::dot(forward, dir);

    result.target = c;
    result.dot = dot;
    result.distance = distance;

    return result;
}

TargetResult Gamesystem::FindNearestCustomer(glm::vec3 playerPos, glm::vec3 forward) {
    TargetResult nearest;
    float bestDot = 0.93f;
    float bestDistance = std::numeric_limits<float>::max();

    for (auto& c : customers) {
        // allow all customers to be targetable, not only idle ones
        TargetResult r1 = FindTarget(&c, playerPos, forward, 5.0f);
        if (!r1.target || r1.dot <= bestDot)
            continue;

        if (r1.dot > bestDot || r1.distance < bestDistance) {
            nearest = r1;
            bestDot = r1.dot;
            bestDistance = r1.distance;
        }
    }

    return nearest;
}


void Gamesystem::addIngrident(string ingrident, string type) {
    if (ingrident == "Ice") {
        currentCup.isIced = true;

    }

    if (type == "syrup") {
        currentCup.ingridents["syrup"].push_back(ingrident);

    }

    if (type == "main") {
        std::cout << "main" << std::endl;
        if (currentCup.ingridents["main"].empty()) {
            std::cout << "a" << std::endl;
            currentCup.ingridents["main"].push_back(ingrident);
        }
        else {
         //check if currentcup vec is same size as menu vec
            //sort current vec
        //for i in menu size
        //menu[i] == current[i]
            bool found;
            currentCup.ingridents["main"].push_back(ingrident);
            std::sort(currentCup.ingridents["main"].begin(), currentCup.ingridents["main"].end());
            for (auto pair : recipyMap) {
                found = true;
                std::cout << "current menu" << pair.first << std::endl;
                if (pair.second.size() == currentCup.ingridents["main"].size()) {
                    for (int i = 0; i < pair.second.size(); i++) {
                        std::cout << "compair" << pair.second[i]<<" "<< currentCup.ingridents["main"][i] << std::endl;
                        if (pair.second[i] != currentCup.ingridents["main"][i]) {
                            found = false;
                            break;
                        }
                    }
                    if (found) {
                        std::cout << "found!" << pair.first << std::endl;
                        currentCup.ingridents["main"].clear();
                        currentCup.ingridents["main"].push_back(pair.first);
                        break;
                    }
                }
        }





        }


    }
    currentCup.cupName = getName();
}

string Gamesystem::getName() {
    string name = "";

    auto addWord = [&](const string& word) {
        if (word.empty()) return;
        if (!name.empty()) name += " ";
        name += word;
        };

    if (currentCup.isIced)
        addWord("Iced");

    for (const string& s : currentCup.ingridents["syrup"])
        addWord(s);

    for (const string& m : currentCup.ingridents["main"])
        addWord(m);

    return name;
}
