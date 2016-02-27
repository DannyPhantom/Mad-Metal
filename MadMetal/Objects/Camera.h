#pragma once

#include "TestObject.h"

class Camera 
{
private:
	TestObject * m_toFollow;
	glm::vec3 m_up, m_currentPos, m_desiredPos, m_rotation, m_lookAt; 
	float m_rotateScalar, m_distance, m_gravityScalar;
	float m_inclinationAngle;

	bool m_recentlyMoved;
public:
	Camera();
	Camera(TestObject * object);
	void setToFollow(TestObject * toFollow);
	void setLookAt(glm::vec3 eye, glm::vec3 at, glm::vec3 up);
	void rotateCamera(float xpos, float ypos);
	void update(double dtMilli);
	
	glm::vec3 getPosition();
	glm::vec3 getUpVector();
	glm::vec3 getLookAt();


	~Camera();
};

