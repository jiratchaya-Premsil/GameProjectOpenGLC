#include "gameLogic/customer.h"
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>
#include <learnopengl/shader_m.h>

#include <cstdlib> // rand
#include <ctime> // time
#include <iostream>

Customer::Customer() {
	speed = 2.0f;
	state = CustomerState::Walking;
	stateTimer = 0.0f;
	greeted = false;
	readyToLeave = false;

	// Seed random number generator if not already done
	static bool seeded = false;
	if (!seeded) {
		srand(static_cast<unsigned int>(time(nullptr)));
		seeded = true;
	}
}

void Customer::Init(const glm::vec3& startPos, const glm::vec3& targetPos,
	AnimationModel* sharedModel,
	Animation* walkAnim,
	Animation* greetAnim,
	Animation* idleAnim)
{
	position = startPos;
	targetA = startPos;
	targetB = targetPos;
	
	baseModel = sharedModel;

	walkAnimation = walkAnim;
	greetAnimation = greetAnim;
	idleAnimation = idleAnim;

	animator = std::make_unique<Animator>(walkAnimation);





	ChangeState(CustomerState::Walking);
}





void Customer::ChangeState(CustomerState newState)
{
	state = newState;
	stateTimer = 0.0f;

	switch (state)
	{
	case CustomerState::Walking:
	case CustomerState::Leaving:
		animator->PlayAnimation(walkAnimation);
		readyToLeave = false;
		break;

	case CustomerState::Greeting:
		animator->PlayAnimation(greetAnimation);
		greeted = false;
		readyToLeave = false;
		break;

	case CustomerState::Idle:
		animator->PlayAnimation(idleAnimation);
		break;
	}
}

void Customer::Update(float deltaTime)
{
	stateTimer += deltaTime;

	// 🔥 update animation every frame
	animator->UpdateAnimation(deltaTime);

	switch (state)
	{
	case CustomerState::Walking:
		MoveTo(targetB, deltaTime);

		if (glm::distance(position, targetB) < 0.1f)
		{
			ChangeState(CustomerState::Idle);
		}
		break;

	case CustomerState::Greeting:
		if (!greeted)
		{
			greeted = true;
		}

		if (stateTimer > 5.0f)
		{
			ChangeState(CustomerState::Idle);
		}
		break;

	case CustomerState::Idle:
		if (readyToLeave && stateTimer > 3.0f)
		{
			ChangeState(CustomerState::Leaving);
		}
		break;

	case CustomerState::Leaving:

		MoveTo(targetA, deltaTime);

		if (glm::distance(position, targetA) < 0.1f)
		{
			Respawn();
			

		}
		break;
	}
}

void Customer::Interact()
{
	if (state == CustomerState::Idle)
	{
		if (!greeted) {
			ChangeState(CustomerState::Greeting);
		}
		else if (!readyToLeave) {
			readyToLeave = true;
			stateTimer = 0.0f;
		}
	}
}

void Customer::Respawn()
{
	std::cout << "respawned!" << std::endl;


    glm::vec3 newStartPos = glm::vec3(position.x, position.y, 4.0);

    // Update position and targetA to new random location
    position = newStartPos;
    targetA = newStartPos;
    // Keep spawnPosition unchanged - this is where they should exit from
    // targetB remains the same (the counter position)

    greeted = false;
    readyToLeave = false;
    speach.clear();
    ChangeState(CustomerState::Walking);
}

void Customer::MoveTo(const glm::vec3& target, float deltaTime)
{
	if (position[2] < 0.0f) { position[2] = 0; return; }
	glm::vec3 direction = target - position;
	if (glm::length(direction) < 0.01f)
		return;

	direction = glm::normalize(direction);

	position += direction * speed * deltaTime;
}

void Customer::Draw(Shader& shader)
{
    Draw(shader, 1.0f);
}

void Customer::Draw(Shader& shader, float scale)
{
	auto transforms = animator->GetFinalBoneMatrices();

	for (int i = 0; i < transforms.size(); ++i)
	{
		shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
	}

	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    glm::vec3 currentTarget = (state == CustomerState::Leaving) ? targetA : glm::vec3(position[0], position[1], -4.0f);
	/*if (state == CustomerState::Idle) {
		currentTarget = glm::vec3(position[0], position[1], -4.0f);
	}*/
    glm::vec3 direction = currentTarget - position;

    if (glm::length(direction) > 0.001f)
    {
        direction = glm::normalize(direction);
        // Calculate angle from direction (assuming model faces +Z by default)
        float angle = atan2(direction.x, direction.z);
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    model = glm::scale(model, glm::vec3(scale));
    shader.setMat4("model", model);
	
	baseModel->Draw(shader);
	//if (clothModel) clothModel->Draw(shader);
	//if (pantsModel) pantsModel->Draw(shader);
}