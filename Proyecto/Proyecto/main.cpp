
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <random>
#include <ctime>

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

// =================================================================== //
//																	   //
//					Variables para animación						   //
//																	   //
// =================================================================== //


// Puerta
float bajadaPuerta = 0.0f;
float rotacionPuerta = 0.0f;

// Cartel
float toffsetcartelu = 0.0f;
float toffsetcartelv = 0.0f;
float tiempoAcumulado = 0.0f;

// =================================================================== //


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;


// =================================================================== //
//																	   //
//					Variables para texturas							   //
//																	   //
// =================================================================== //

// Texturas básicas
Texture plainTexture;
Texture pisoTexture;

// Para el cartel de la puerta
Texture hk_font_Proyecto_GCEIHC;


// =================================================================== //
//																	   //
//					Variables para Modelos							   //
//																	   //
// =================================================================== //

// Puerta reja
Model Puerta_reja_arco;
Model Puerta_reja_derecha;
Model Puerta_reja_central;
Model Puerta_reja_piso;
Model Puerta_reja_puerta_izquierda;
Model Puerta_reja_puerta_derecha;


// Banco Hollow Knight
Model hk_Banco;

// Lampara Hollow Knight
Model hk_Lampara;

// Ring
Model Ring;

// Rocas
Model Roca;
Model RocaPequenia;

// =================================================================== //

// Skybox
Skybox skybox;


//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};
	
	GLfloat verticesCartel[] = {
		//	x      y      z			u		v			nx	  ny    nz
			-2.5f, -1.0f, 0.0f,		0.0f,	0.875f,		0.0f, 0.0f, -1.0f,
			2.5f,  -1.0f, 0.0f,		0.22f,  0.875f,		0.0f, 0.0f, -1.0f,
			2.5f,  1.0f,  0.0f,		0.22f,	1.0f,		0.0f, 0.0f, -1.0f,
			-2.5f, 1.0f,  0.0f,		0.0f,	1.0f,		0.0f, 0.0f, -1.0f
	};

	unsigned int cartelIndices[] = {
		0, 1, 2,
		0, 3, 2,
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número
	
	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(verticesCartel, cartelIndices, 32, 6);
	meshList.push_back(obj8);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	// Ventana principal
	mainWindow = Window(1280, 1024); 
	mainWindow.Initialise();

	// Crear objetos y shaders
	CreateObjects();
	CreateShaders();

	// Camera
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);


	// =================================================================== //
	//																	   //
	//						Importar texturas				       		   //
	//																	   //
	// =================================================================== //

	// Texturas básicas
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/Escenario/4_Piso.png");
	pisoTexture.LoadTextureA();

	// Skybox Textures
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	// Materiales
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// Textura del cartel de la puerta
	hk_font_Proyecto_GCEIHC = Texture("Textures/Hollow_knight/hk_font_Proyecto_GCEIHC.png");
	hk_font_Proyecto_GCEIHC.LoadTextureA();


	// =================================================================== //
	//																	   //
	//						Importar Modelos				       		   //
	//																	   //
	// =================================================================== //
	
	// Puerta reja
	Puerta_reja_arco = Model();
	Puerta_reja_arco.LoadModel("Models/Hollow_knight/hk_Puerta_reja_arco.obj");
	Puerta_reja_derecha = Model();
	Puerta_reja_derecha.LoadModel("Models/Hollow_knight/hk_Puerta_reja_derecha.obj");
	Puerta_reja_central = Model();
	Puerta_reja_central.LoadModel("Models/Hollow_knight/hk_Puerta_reja_puerta_central.obj");
	Puerta_reja_piso = Model();
	Puerta_reja_piso.LoadModel("Models/Hollow_knight/hk_Puerta_reja_piso.obj");
	Puerta_reja_puerta_izquierda = Model();
	Puerta_reja_puerta_izquierda.LoadModel("Models/Hollow_knight/hk_Puerta_reja_puerta_izquierda.obj");
	Puerta_reja_puerta_derecha = Model();
	Puerta_reja_puerta_derecha.LoadModel("Models/Hollow_knight/hk_Puerta_reja_puerta_derecha.obj");


	// Banco Hollow Knight
	hk_Banco = Model();
	hk_Banco.LoadModel("Models/Hollow_knight/hk_Banco.obj");

	// Lampara Hollow Knight
	hk_Lampara = Model();
	hk_Lampara.LoadModel("Models/Hollow_knight/hk_Lampara.obj");

	// Ring
	Ring = Model();
	Ring.LoadModel("Models/Escenario/es_Ring.obj");

	// Rocas
	Roca = Model();
	Roca.LoadModel("Models/Escenario/es_Roca_canasta.obj");
	RocaPequenia = Model();
	RocaPequenia.LoadModel("Models/Escenario/es_Roca_canasta_pequenia.obj");


	// =================================================================== //
	//																	   //
	//						Definir luces					       		   //
	//																	   //
	// =================================================================== //

	
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.6f, 0.6f,
		0.0f, -1.0f, 0.0f);


	// Luces puntuales ----------------------------------------------------

	unsigned int pointLightCount = 0; // Contador de luces puntuales

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Luces spot ---------------------------------------------------------

	unsigned int spotLightCount = 0; // Contador de luces spot

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 0.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	// =================================================================== //



	// Variables
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;

	// Matriz de proyección
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	// Para la lampara
	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	// Variables auxiliares
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 ringCentro(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);



	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		// Calculo del tiempo entre frames
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

	
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		// Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -150.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		// ================================================================================ //
		//																					//
		//									Ring											//
		//																					//
		// ================================================================================ //


		model = glm::mat4(1.0);

		// Posicionamiento global
		model = glm::translate(model, glm::vec3(0.0f, 0.01f, -150.0f));
		ringCentro = model;
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ring.RenderModel();


		// ================================================================================ //
		//																					//
		//									Rocas											//
		//																					//
		// ================================================================================ //

		// Roca grande ---------------------------------------------------------
		model = glm::mat4(1.0);

		// Posicionamiento global
		model = ringCentro;
		model = glm::translate(model, glm::vec3(-30.0f, 55.0f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Roca.RenderModel();

		// Roca pequeña ---------------------------------------------------------
		model = glm::mat4(1.0);

		// Posicionamiento global
		model = ringCentro;
		model = glm::translate(model, glm::vec3(30.0f, 55.0f, -10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RocaPequenia.RenderModel();

		// ================================================================================ //
		//																					//
		//							Bancas_HK y Lamparas_HK									//
		//																					//
		// ================================================================================ //

		// Banca del fondo ---------------------------------------------------------
		model = glm::mat4(1.0);

		// Posicionamiento global
		model = ringCentro;
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -80.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		hk_Banco.RenderModel();

		// Lampara asociada a la banca del fondo -------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.0f, -4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		hk_Lampara.RenderModel();

		// Banca de adelante ---------------------------------------------------------
		model = glm::mat4(1.0);

		// Posicionamiento global
		model = ringCentro;
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, 80.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		hk_Banco.RenderModel();

		// Lampara asociada a la banca de adelante -------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(-15.0f, -4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		hk_Lampara.RenderModel();


		// ================================================================================ //
		//																					//
		//									Puerta 											//
		//																					//
		// ================================================================================ //

		// Animación de la puerta
		if (mainWindow.getPuertaAbriendose()) 
		{
			if (mainWindow.getPuertaCerrada())
			{
				if (bajadaPuerta >= -8.17f)
				{
					bajadaPuerta -= 0.1f * deltaTime;
					rotacionPuerta += 1.46879f * deltaTime;
				}
				else
				{
					mainWindow.setPuertaAbriendose();
					mainWindow.setPuertaCerrada();
				}
			}
			else
			{
				if (bajadaPuerta <= 0.0f)
				{
					bajadaPuerta += 0.1f * deltaTime;
					rotacionPuerta -= 1.46879f * deltaTime;
				}
				else
				{
					mainWindow.setPuertaAbriendose();
					mainWindow.setPuertaCerrada();
				}
			}	
		}
		
		// Animación del cartel de la puerta (textura animada)
		tiempoAcumulado += deltaTime;

		if (tiempoAcumulado >= 11.0f) 
		{
			toffsetcartelu += 0.06;

			if (toffsetcartelu >= 1.0)
			{
				toffsetcartelu = 0.0;
			}

			tiempoAcumulado = 0.0f;
		}
		toffsetcartelv = 0.0f;

		// Arco de la puerta ------------------------------------------------------------
		model = glm::mat4(1.0);
		
		// Posicionamiento global
		model = ringCentro;
		model = glm::translate(model, glm::vec3( 0.0f, 0.0f, 140.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_reja_arco.RenderModel();


		modelaux = model;

		// Parte derecha de la puerta -----------------------------------------------------
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_reja_derecha.RenderModel();
		
		// Parte izquierda de la puerta ---------------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(-23.4f, 0.0f, 00.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_reja_derecha.RenderModel();
		
		// Parte central de la puerta -----------------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + bajadaPuerta, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_reja_central.RenderModel();

		// Piso de la puerta --------------------------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.01f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_reja_piso.RenderModel();

		// Parte central derecha de la puerta -------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(8.25f, 0.0f, 0.0f));
		model = glm::rotate(model, -1 * rotacionPuerta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_reja_puerta_derecha.RenderModel();

		// Parte central izquierda de la puerta ---------------------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.35f, 0.0f, 0.0f));
		model = glm::rotate(model, rotacionPuerta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_reja_puerta_izquierda.RenderModel();
		
		// Para el cartel de la puerta ----------------------------------------------------
		model = modelaux;
		toffset = glm::vec2(toffsetcartelu, toffsetcartelv);

		model = glm::translate(model, glm::vec3(0.0f, 17.19f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		hk_font_Proyecto_GCEIHC.UseTexture();
		meshList[7]->RenderMesh();


		// ================================================================================ //



		glDisable(GL_BLEND);
		

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
