#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange, glm::vec3 objetivo);

	glm::vec3 getCameraPosition();
	void setCameraPosition(glm::vec3 newposition);
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	GLboolean getthirdperson() { return thirdperson; };
	GLfloat getyaw() { return yaw; };

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 offset;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	float r;
	float focus;
	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;
	GLboolean thirdperson;

	void update();
	void updatethird(glm::vec3 objetivo);
};

