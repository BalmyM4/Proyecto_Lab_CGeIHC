#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>
#include <vector>

// === estructura auxiliar para cambiar de camara ===
struct CameraSpot {
	glm::vec3 position;
	GLfloat yaw;
	GLfloat pitch;
	const char* name;
};
// ===============================

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


	// ===================================================================
	// Se agregó un sistema de spots de cámara predefinidos que se activan
	// con teclas numéricas (5, 6, 7, 8, 9). Cada spot tiene:
	// - Posición específica en el mundo 3D
	// - Orientación (yaw y pitch) predefinida
	// - Nombre descriptivo para identificación
	// ===================================================================
	std::vector<CameraSpot> cameraSpots;  // Lista de vistas predefinidas
	int currentSpotIndex;                 // Índice del spot actualmente activo


	// Permite verificar si la cámara está en modo teletransporte
	// Se usa para bloquear el movimiento del mouse durante el teletransporte
	bool isTeleportingActive() { return isTeleporting; }
	GLboolean getthirdperson() { return thirdperson; };
	GLboolean getaerial() { return aerial; };
	GLfloat getyaw() { return yaw; };

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 offset;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	//radio al que se encuentra la cámara del objetivo
	float r;
	float focus;
	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;
	GLboolean thirdperson;
	GLboolean aerial;
	// Estas variables guardan el estado anterior de la cámara antes del
	// teletransporte. Cuando se suelta la tecla, la cámara regresa
	// exactamente a su posición y orientación original.
	// 
	// previousPosition: Guarda la posición antes del teletransporte
	// previousFront:    Guarda la dirección de vista anterior
	// previousYaw:      Guarda la rotación horizontal anterior  
	// previousPitch:    Guarda la rotación vertical anterior
	// isTeleporting:    Flag que indica si estamos en modo teletransporte
	glm::vec3 previousPosition;
	glm::vec3 previousFront;
	GLfloat previousYaw;
	GLfloat previousPitch;
	bool isTeleporting;


	void update();
	void updateaerial();
	void updatethird(glm::vec3 objetivo);

};

