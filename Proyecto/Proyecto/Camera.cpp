#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	r = 30.0f;
	focus = 1.0f;

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

    // Se definen 5 spots de cámara predefinidos que se activan con teclas:
	previousPosition = position;
	previousFront = front;
	previousYaw = yaw;
	previousPitch = pitch;
	isTeleporting = false;
	currentSpotIndex = -1; // -1 significa ningún spot activo


	cameraSpots = {
		{glm::vec3(0.0f, 200.0f, -150.0f), 90.001f, -90.0f, "Vista Aerea"},     // Tecla 5
		{glm::vec3(75.0f, 35.0f, -150.0f), 180.0f, -20.0f, "Vista Ring"},     // Tecla 6  
		{glm::vec3(-50.0f, 3.0f, 0.0f), 90.0f, -5.0f, "Vista Lateral"},      // Tecla 7
		{glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, "Vista Primera Persona"}, // Tecla 8
		{glm::vec3(30.0f, 20.0f, 30.0f), -135.0f, -30.0f, "Vista Diagonal"}   // Tecla 9
	};


	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
    GLfloat velocity = moveSpeed * deltaTime;

    // Al mantener presionadas teclas numéricas (5-9),
    // la cámara se teletransporta a vistas predefinidas. Al soltar la tecla,
    // regresa automáticamente a su posición anterior.
    for (int i = 0; i < cameraSpots.size(); i++)
    {
        int key = GLFW_KEY_1 + i+4; 
        if (keys[key])
        {
            if (!isTeleporting || currentSpotIndex != i)
            {
                // Guardar estado actual antes de teletransportarse
                previousPosition = position;
                previousFront = front;
                previousYaw = yaw;
                previousPitch = pitch;
                isTeleporting = true;
                currentSpotIndex = i;
                
                // Mover al spot correspondiente
                CameraSpot spot = cameraSpots[i];
                position = spot.position;
                yaw = spot.yaw;
                pitch = spot.pitch;
                
                
                update();
                
                
                printf("Spot activado: %s\n", spot.name);
            }
            return; 
        }
    }
    
    // === RESTAURACIÓN AL SOLTAR TODAS LAS TECLAS DE SPOT ===
    bool anySpotKeyPressed = false;
    for (int i = 0; i < cameraSpots.size(); i++)
    {
        if (keys[GLFW_KEY_1 + i])
        {
            anySpotKeyPressed = true;
            break;
        }
    }
    
    if (!anySpotKeyPressed && isTeleporting)
    {
        // Restaurar estado anterior cuando se sueltan todas las teclas de spot
        position = previousPosition;
        front = previousFront;
        yaw = previousYaw;
        pitch = previousPitch;
        isTeleporting = false;
        currentSpotIndex = -1;
        
        update();
    }

    // === CONTROLES NORMALES (solo cuando no estamos teletransportados) ===
    if (!isTeleporting && thirdperson)
    {
        if (keys[GLFW_KEY_W])
        {
            position += front * velocity;
        }

        if (keys[GLFW_KEY_S])
        {
            position -= front * velocity;
        }

        if (keys[GLFW_KEY_A])
        {
            position -= right * velocity;
        }

        if (keys[GLFW_KEY_D])
        {
            position += right * velocity;
        }
    }
    
    
    // Cámara libre
    if (keys[GLFW_KEY_1])
    {
        thirdperson = GL_TRUE;
    }
    // Tercera persona
    if (keys[GLFW_KEY_3])
    {
        thirdperson = GL_FALSE;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange, glm::vec3 objetivo)
{

    // Se agregó una verificación para evitar que el mouse
    // mueva la cámara mientras se mantiene presionada una tecla de
    // teletransporte. Esto previene comportamientos indeseados durante
    // las vistas predefinidas.
    if (isTeleporting)
    {
        return; 
    }
    


	xChange *= turnSpeed;
	yChange *= turnSpeed;


    yaw += xChange; 
    


	if (!thirdperson)
	{
        pitch -= yChange;

		if (pitch > 49.0f)
		{
			pitch = 49.0f;
		}

		if (pitch < -49.0f)
		{
			pitch = -49.0f;
		}
		updatethird(objetivo);
	}
	else
	{
        pitch += yChange;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}

		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
		update();
	}
		
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::setCameraPosition(glm::vec3 newposition) {
	position = newposition;
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::updatethird(glm::vec3 objetivo)
{
	offset.x = r * cos(glm::radians(pitch)) * sin(glm::radians(-yaw));
	offset.y = r * sin(glm::radians(pitch));
	offset.z = r * cos(glm::radians(pitch)) * cos(glm::radians(-yaw));
	position = offset + objetivo + focus;
	glm::vec3 worldFocus = objetivo + focus;
	front = glm::normalize(worldFocus - position);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
