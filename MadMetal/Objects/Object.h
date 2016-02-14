#pragma once

#include <vector>
#include "Libraries\glm\vec3.hpp"
#include "Libraries\glm\mat4x4.hpp"
#include "Libraries\glm\gtc\matrix_transform.hpp"
#include "Simulation\PhysicsManager.h"

class Object
{
protected:
	std::vector<Object *> childObjects;
	glm::vec3 position, rotation, scale, forward, up;
	PxRigidActor *physicsActor;

public:
	Object();
	virtual ~Object();

	
	glm::mat4x4 getModelMatrix();
	glm::mat4x4 getPhysicsModelMatrix();
	glm::mat4x4 getLocalModelMatrix();
	void updateRotation(glm::vec3 dr) { rotation += dr; }
	void updatePosition(glm::vec3 dp) { position += dp; }
	glm::vec3  getPosition(){ return position; }
	glm::vec3 getForwardVector() { glm::vec4 toReturn = glm::normalize(getPhysicsModelMatrix() * glm::vec4(forward, 0.0f)); return glm::vec3(toReturn.x, toReturn.y, toReturn.z); }
	void setActor(PxRigidActor *actor) { physicsActor = actor; }
	PxRigidActor& getActor();
};

