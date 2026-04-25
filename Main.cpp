#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>

#include <ft2build.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//own file
#include "ui.h"
#include "ui_Info.h"
#include "text.h"
#include <gameLogic/Interactive_Logic.h>
#include <gameLogic/mixing.h>
#include <gameLogic/customer.h>
#include <gameLogic/IRenderable.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Gamesystem& gameSystem);
float lastTime = 0.0f;
float GetDeltaTime()
{
    float currentTime = glfwGetTime();
    float dt = currentTime - lastTime;
    lastTime = currentTime;
    return dt;
}

// settings
const unsigned int SCR_WIDTH = 975;
const unsigned int SCR_HEIGHT = 624;



// camera
Camera camera(glm::vec3(0.0f, 2.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float followDistance = 3.0f;
float followHeight = 1.5f;
double mouseX = 0, mouseY = 0;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 playerPos = glm::vec3(0.0f, 2.0f, 0.0f);


float catSpeed = 5.0f;
glm::vec3 moveDir(0.0f);
int score = 0;
float catRadius = 0.6f;
float coinRadius = 0.3f;


bool cursorEnabled = true;
int gameState = 0;
TargetResultObj closetObj;
TargetResult closetCustomer;
IRenderable* highlighted = nullptr;

bool previous = false;


std::vector<Object> StaticObjs;
std::vector<Object> objects;

int main()
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------

    Shader ourShader("shader/1.model_loading.vs", "shader/1.model_loading.fs");
    std::cout << "loaded shader for 3d" << std::endl;

    //set up ui
    Shader uiShader("shader/ui.vs", "shader/ui.fs");
    std::cout << "loaded shader for 2oud" << std::endl;

    //set up highlight render
    Shader HighlightShader("shader/1.model_loading.vs", "shader/highlightShader.fs");
    Shader HighlightAnimationShader("shader/highlightAnimation.vs", "shader/highlightShader.fs");
    std::cout << "loaded shader for highlight" << std::endl;

    //set up animatable object render
    Shader AnimationShader("shader/anim_model.vs", "shader/anim_model.fs");
    std::cout << "loaded animation shader" << std::endl;

    //set up font
    Shader TextShader("shader/text_Shader.vs", "shader/text_Shader.fs");
    TextRenderer text;
    TextShader.use();
    TextShader.setInt("textTexture", 0);

    //set up ui
    UIRenderData bg = createUI("Gamebg");
    UIRenderData startBtn = createUI("StartBtn");
    UIRenderData exitBtn = createUI("ExitBtn");

    // load non interactable objects
    // -----------
    std::cout << "loading floor" << std::endl;
    vector<string> storePath = {

        "resources/objects/Store/walls/MeshWall.obj",
        "resources/objects/Store/roof/MeshWall.obj",
        "resources/objects/Store/light/lights.obj",
        "resources/objects/Store/floor/carpet.obj",
        "resources/objects/Store/floor/floor.obj",
         "resources/objects/Store/counter/counter2.obj",
         "resources/objects/Store/decoration/bush.obj",
          "resources/objects/Store/decoration/DecWall.obj",
          "resources/objects/Store/decoration/RecipyBoard.obj",
          "resources/objects/Store/decoration/Table.obj",
          "resources/objects/Store/decoration/Chairs.obj",
          "resources/objects/Store/decoration/sign.obj"

    };
    for (string path : storePath) {
        auto model = std::make_shared<Model>(path);
        auto Object = createObject(
            "obj",
            "-",
            glm::vec3(0.0f),
            model,
            glm::vec3(0.25f)

        );
        StaticObjs.push_back(Object);
    }

    //add all model that is interactable

    std::cout << "loading interactable" << std::endl;

    auto Coffee = std::make_shared<Model>("resources/objects/gamePlayObj/main/Coffee.obj");
    auto Milk = std::make_shared<Model>("resources/objects/gamePlayObj/main/Milk.obj");
    auto Espresso = std::make_shared<Model>("resources/objects/gamePlayObj/main/Espresso.obj");
    auto Ice = std::make_shared<Model>("resources/objects/gamePlayObj/main/Ice.obj");
    auto Chocolate = std::make_shared<Model>("resources/objects/gamePlayObj/main/Chocolate.obj");

    auto Cup = std::make_shared<Model>("resources/objects/gamePlayObj/cup/Cup.obj");
    auto Cups = std::make_shared<Model>("resources/objects/gamePlayObj/cup/Cups.obj");

    auto Banana = std::make_shared<Model>("resources/objects/gamePlayObj/syrup/Banana.obj");
    auto Stawberry = std::make_shared<Model>("resources/objects/gamePlayObj/syrup/StawBerry.obj");

    auto objCoffee = createObject(
        "Coffee",
        "main",
        glm::vec3(1.75f, 0.85f, -2.5f),
        Coffee,
        glm::vec3(0.25f)

    );
    auto objMilk = createObject(
        "Milk",
        "main",
        glm::vec3(1.3f, 0.85f, -2.5f),
        Milk,
        glm::vec3(0.25f)

    );

    auto objCups = createObject(
        "Cup",
        "tool",
        glm::vec3(0.4f, 0.8f, -2.5f),
        Cups,
        glm::vec3(0.25f)

    );
    auto objIce = createObject(
        "Ice",
        "Ice",
        glm::vec3(2.8f, 0.7f, -2.5f),
        Ice,
        glm::vec3(0.25f)

    );
    auto objEspresso = createObject(
        "Espresso",
        "main",
        glm::vec3(2.21f, 0.85f, -2.5f),
        Espresso,
        glm::vec3(0.25f)

    );

    auto objBanana = createObject(
        "Banana",
        "syrup",
        glm::vec3(3.5f, 0.95f, -2.5f),
        Banana,
        glm::vec3(0.25f)

    );

    auto objStawberry = createObject(
        "Stawberry",
        "syrup",
        glm::vec3(3.8f, 0.95f, -2.5f),
        Stawberry,
        glm::vec3(0.25f)

    );

    auto objChocolate = createObject(
        "Chocolate",
        "main",
        glm::vec3(3.5f, 0.95f,-0.5f),
        Chocolate,
        glm::vec3(0.25f)

    );


    objects.push_back(objCups);
    objects.push_back(objMilk);
    objects.push_back(objIce);
    objects.push_back(objEspresso);
    objects.push_back(objCoffee);
    objects.push_back(objBanana);
    objects.push_back(objStawberry);
    objects.push_back(objChocolate);
    insertObject(objMilk);
    insertObject(objCups);
    insertObject(objIce);
    insertObject(objEspresso);
    insertObject(objCoffee);
    insertObject(objBanana);
    insertObject(objStawberry);
    insertObject(objChocolate);
    std::cout << "loaded model" << std::endl;
    text.init();
    std::cout << "inited text" << std::endl;
    //set the cup sustem


    Gamesystem gameSystem;
    //setcustomer
    gameSystem.initCustomer();

    //set highlight

    glm::vec4 modelscaleHighlight;
    // render loop
    // -----------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_STENCIL_TEST);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, gameSystem);
        glClearColor(0.741f, 0.819f, 0.780f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // =========================
        // 🎮 GAME STATE
        // =========================
        if (gameState == 1)
        {
            glEnable(GL_DEPTH_TEST);

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0x00);
            // camera update ONLY in game
            highlighted = nullptr;
            closetObj = updateInteraction(playerPos, camera.Front);
            closetCustomer = gameSystem.FindNearestCustomer(playerPos, camera.Front);
            float bestDot = 0.93f;

            if (closetCustomer.target && closetCustomer.dot > bestDot) {
                bestDot = closetCustomer.dot;
                highlighted = closetCustomer.target;
            }

            if (closetObj.target && closetObj.dot > bestDot) {
                if (closetCustomer.distance > closetObj.distance) {
                    bestDot = closetObj.dot;
                    highlighted = closetObj.target;
                }

            }
            camera.Position = playerPos;
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;


            ourShader.use();

            // 3D projection
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

            glm::mat4 view = camera.GetViewMatrix();

            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);

            // 🪙 allInteractable Object

            for (auto& Interactable : objects) {
                //if (highlighted == &Interactable) { continue; }


                glm::mat4 ItemModel = glm::mat4(1.0f);
                ItemModel = glm::translate(ItemModel, Interactable.pos);
                ItemModel = glm::scale(ItemModel, Interactable.scale);

                ourShader.setMat4("model", ItemModel);
                Interactable.model->Draw(ourShader);
            }

            if (gameSystem.currentCup.equiped) {


                glm::mat4 ItemModel = glm::mat4(1.0f);

                // Position cup on the right side of the screen relative to camera
                // Calculate position: camera position + right offset + forward offset
                glm::vec3 rightOffset = camera.Right * 0.4f;  // 0.8 units to the right
                glm::vec3 forwardOffset = camera.Front * 0.8f; // 1.2 units in front
                glm::vec3 upOffset = glm::vec3(0.0f, -0.4f, 0.0f); // slightly below eye level

                glm::vec3 itemPos = camera.Position + rightOffset + forwardOffset + upOffset;

                ItemModel = glm::translate(ItemModel, itemPos);

                // Billboard rotation - face camera
                glm::mat4 rotation(1.0f);
                rotation[0] = glm::vec4(camera.Right, 0.0f);
                rotation[1] = glm::vec4(camera.Up, 0.0f);
                rotation[2] = glm::vec4(-camera.Front, 0.0f);
                ItemModel *= rotation;

                // Scale it down
                ItemModel = glm::scale(ItemModel, glm::vec3(0.25f));

                ourShader.setMat4("model", ItemModel);
                Cup->Draw(ourShader);

            }

            // decoration/ non Interactable objs

            for (auto& StaticItem : StaticObjs) {

                glm::mat4 ItemModel = glm::mat4(1.0f);
                ItemModel = glm::translate(ItemModel, StaticItem.pos);
                ItemModel = glm::scale(ItemModel, StaticItem.scale);

                ourShader.setMat4("model", ItemModel);
                StaticItem.model->Draw(ourShader);

            }


            AnimationShader.use();
            // view/projection transformations

            AnimationShader.setMat4("projection", projection);
            AnimationShader.setMat4("view", view);


            for (auto& c : gameSystem.customers) {
                //if (highlighted == &c) { continue; }
                c.Update(deltaTime);
                c.Draw(AnimationShader);
            }

            // outline highlight pass for the selected object/customer
            if (highlighted != nullptr)
            {

                const float outlineScale = 1.05f;
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);

                if (highlighted->IsAnimated()) {
                    Customer* customer = dynamic_cast<Customer*>(highlighted);
                    if (customer) {
                        HighlightAnimationShader.use();
                        HighlightAnimationShader.setMat4("projection", projection);
                        HighlightAnimationShader.setMat4("view", view);
                        customer->Draw(HighlightAnimationShader, outlineScale);
                    }
                }
                else {
                    Object* obj = dynamic_cast<Object*>(highlighted);
                    if (obj) {
                        HighlightShader.use();
                        HighlightShader.setMat4("projection", projection);
                        HighlightShader.setMat4("view", view);
                        obj->Draw(HighlightShader, outlineScale);
                    }
                }

                glCullFace(GL_BACK);
                glDisable(GL_CULL_FACE);
            }

            TextShader.use();
            glDisable(GL_DEPTH_TEST);

            glm::mat4 TextModel(1.0f);
            glm::mat4 TextModel2(1.0f);
            const glm::vec3 customerTextOffset = glm::vec3(0.7f, 1.5f, 0.0f);

            // Position
            TextModel = glm::translate(TextModel, gameSystem.customers[0].GetPosition() + customerTextOffset);
            TextModel2 = glm::translate(TextModel2, gameSystem.customers[1].GetPosition() + customerTextOffset + glm::vec3(0.0f, 0.2f, 0.0f));

            // Billboard rotation
            glm::mat4 rotation(1.0f);
            rotation[0] = glm::vec4(camera.Right, 0.0f);
            rotation[1] = glm::vec4(camera.Up, 0.0f);
            rotation[2] = glm::vec4(-camera.Front, 0.0f);

            // Apply rotation
            TextModel *= rotation;
            TextModel2 *= rotation;
            // Scale
            TextModel = glm::scale(TextModel, glm::vec3(0.003f, -0.003f, 0.003f));
            TextModel2 = glm::scale(TextModel2, glm::vec3(0.003f, -0.003f, 0.003f));

            glm::mat4 textProjection = glm::perspective(
                glm::radians(camera.Zoom),
                (float)SCR_WIDTH / SCR_HEIGHT,
                0.1f, 100.0f
            );
            glm::mat4 Textview = camera.GetViewMatrix();
            TextShader.setMat4("projection", textProjection);
            TextShader.setMat4("view", Textview);
            TextShader.setVec3("textColor", 0.172f, 0.137f, 0.121f);
           // TextShader.setVec3("textColor", 1.0f, 1.0f, 1.0f);
            if (gameSystem.customers[0].greeted) {
                TextShader.setMat4("model", TextModel);
                text.drawText(gameSystem.customers[0].speach.empty() ? "HELLO" : gameSystem.customers[0].speach, 0.0f, 0.0f, 1.0f);
            }
            if (gameSystem.customers[1].greeted) {
                TextShader.setMat4("model", TextModel2);
                text.drawText(gameSystem.customers[1].speach.empty() ? "HELLO" : gameSystem.customers[1].speach, 0.0f, 0.0f, 1.0f);
            }
            glEnable(GL_DEPTH_TEST);



            // UI projection
            glm::mat4 projectionUi = glm::ortho(
                0.0f, (float)SCR_WIDTH,
                (float)SCR_HEIGHT, 0.0f
            );
            glm::mat4 viewUi = glm::mat4(1.0f);   // no camera
            glm::mat4 modelUi = glm::mat4(1.0f);  // no transform

            TextShader.setMat4("projection", projectionUi);
            TextShader.setMat4("view", viewUi);
            TextShader.setMat4("model", modelUi);



            if (highlighted != nullptr)
            {




                glDisable(GL_DEPTH_TEST);
                // collision


                TextShader.use();
                TextShader.setMat4("projection", projectionUi);
                TextShader.setMat4("view", viewUi);
                TextShader.setMat4("model", modelUi);

                    text.drawText(
                        highlighted->GetName(),
                        0.4f * SCR_WIDTH,
                        0.9f * SCR_HEIGHT,
                        1.0f
                    );



            }
            //money status
            if (gameSystem.currentCup.equiped) {

                text.drawText(
                    gameSystem.currentCup.cupName,
                    0.0f * SCR_WIDTH,
                    0.0f * SCR_HEIGHT,
                    1.0f
                );
            }


            text.drawText(
                "Money " + std::to_string(gameSystem.money),
                0.05f * SCR_WIDTH,
                0.9f * SCR_HEIGHT,
                1.0f);

        }


        else if (gameState == 0)
        {
            glDisable(GL_DEPTH_TEST);

            uiShader.use();

            // UI projection
            glm::mat4 projection = glm::ortho(
                0.0f, (float)SCR_WIDTH,
                (float)SCR_HEIGHT, 0.0f
            );

            uiShader.setMat4("projection", projection);
            uiShader.setInt("uiTexture", 0);
            drawUI(bg);
            drawUI(startBtn);
            drawUI(exitBtn);

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                if (isInside(mouseX, mouseY, 60, 330, 244, 90))
                {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    cursorEnabled = false;
                    gameState = 1;
                }

                if (isInside(mouseX, mouseY, 60, 430, 244, 90))
                {
                    glfwSetWindowShouldClose(window, true);
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void processInput(GLFWwindow* window, Gamesystem& gameSystem)
{

    bool current = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    float velocity = catSpeed * deltaTime;
    glm::vec3 forward = glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z));
    glm::vec3 right = glm::normalize(glm::vec3(camera.Right.x, 0.0f, camera.Right.z));
    if (!cursorEnabled) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            playerPos += forward * velocity;

        }


        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            playerPos -= forward * velocity;

        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            playerPos -= right * velocity;

        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            playerPos += right * velocity;

        }

    }
    //std::cout << playerPos[0] << " " << playerPos[2] << std::endl;
    playerPos = glm::vec3(std::min(std::max(playerPos[0], -1.0f),  4.0f), 1.0f, std::min(std::max(playerPos[2], -2.0f), -0.75f));

    if (highlighted && current && !previous) {
        std::cout << highlighted->GetName() << " " << highlighted->GetType();
        gameSystem.Interacted(highlighted->GetName(), highlighted->GetType());
    }

    previous = current;




    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    static bool tabPressedLastFrame = false;

    bool tabPressedNow = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;

    if (tabPressedNow && !tabPressedLastFrame)
    {
        cursorEnabled = !cursorEnabled;

        if (cursorEnabled)
        {

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        }
        else
        {

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    tabPressedLastFrame = tabPressedNow;

}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)

{

    mouseX = xposIn;
    mouseY = yposIn;
    if (cursorEnabled) {
        firstMouse = true;
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (cursorEnabled) return;
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}