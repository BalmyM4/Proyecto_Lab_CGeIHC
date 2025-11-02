#pragma once

#include "CommonValues.h"
#include <vector>
#include <string>
#include<glew.h>
#include<glm.hpp>
#include<glfw3.h>
#include <gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include "Mesh.h"
#include "Shader_light.h"
class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> faceLocations, std::vector<std::string> faceLocations2);
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	
	void setFactor(float f) { factor = glm::clamp(f, 0.0f, 1.0f); }

	~Skybox();
private:
	Mesh* skyMesh;
	Shader* skyShader;
	//Dia
	GLuint textureId;
	//Noche
	GLuint textureId2;

	GLint uniformdia;
	GLint uniformnoche;
	GLint uniformMixFactor;
	GLuint uniformProjection, uniformView;
	float factor;

	void Cubemap(const std::vector<std::string>& faces, GLuint& outTextureId);
};

