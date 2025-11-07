#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
									GLfloat aIntensity, GLfloat dIntensity,
									GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::setDlight(GLfloat time) 
{
	float angulosol = time * 2.0f * 3.14159265f - 0.5f * 3.14159265f;
	direction = glm::vec3(cos(angulosol), sin(angulosol), 0.0f);
	
	if (direction.y < 0.05)
	{
		ambientIntensity = 0.05;
	}	
	else
	{
		ambientIntensity = direction.y;

		if(direction.y > 0.7)
			diffuseIntensity = 0.7f;
		else
			diffuseIntensity = glm::max(0.0f, direction.y);
	}

	
	//ambientIntensity = 0.8f;
	
	//diffuseIntensity = glm::max(0.0f, direction.y);
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientcolorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

DirectionalLight::~DirectionalLight()
{
}
