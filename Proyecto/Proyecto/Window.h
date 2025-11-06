#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Camera.h"

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	
	// Luces Spot
	GLboolean getPrendida1() { return hk_lampara_1; }
	GLboolean getPrendida2() { return hk_lampara_2; }

	// Puerta
	GLboolean getPuertaCerrada() { return puertaCerrada; }
	GLboolean getPuertaAbriendose() { return puertaAbriendose; }
	void setPuertaCerrada() { puertaCerrada = !puertaCerrada; }
	void setPuertaAbriendose() { puertaAbriendose = !puertaAbriendose; }

	// Hollow Knight
	GLboolean getHK_Parado() { return hk_parado; }
	GLboolean getHK_Dash() { return hk_dash; }
	void setHK_Parado() { hk_parado = !hk_parado; }
	void setHK_Dash() { hk_dash = !hk_dash; }

	GLboolean getBS_Ulti() { return bs_ulti; }
	void setBS_Ulti() { bs_ulti = !bs_ulti; }


	// TNT de Crash Bandicoot
	GLboolean getTNT_Normal() { return tnt_normal; }
	GLboolean getTNT_Explotando() { return tnt_explotando; }
	void setTNT_Normal() { tnt_normal = !tnt_normal; }
	void setTNT_Explotando() { tnt_explotando = !tnt_explotando; }

	// Mega caja 
	GLboolean getMegacaja() { return bs_megacaja; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }


	// Crash bandicoot
	GLfloat getmovex_cb() { return movex_cb; }
	GLfloat getmovez_cb() { return movez_cb; }
	GLfloat getrotz_cb() { return rotz_cb; }
	GLfloat getAngulo() { return anguloMovimiento; }
	GLfloat getAngulo2() { return anguloMovimiento2; }
	GLfloat getAngulo3() { return anguloMovimiento3; }
	void procesarMovimiento(Camera& camera);


	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	GLfloat muevex;
	GLboolean adelante;

	GLfloat articulacion1;
	GLfloat dir1;

	// Crash bandicoot
	GLfloat movex_cb, movez_cb;
	GLfloat rotz_cb;
	GLfloat velocidad_cb = 0.5f;
	GLfloat anguloMovimiento = 0.0f;
	GLfloat dirAnguloMovimiento = 1.0f;
	GLfloat anguloMovimiento2 = 0.0f;
	GLfloat dirAnguloMovimiento2 = 1.0f;
	GLfloat anguloMovimiento3 = 0.0f;
	GLfloat dirAnguloMovimiento3 = 1.0f;


	// Puerta
	GLboolean puertaCerrada;
	GLboolean puertaAbriendose;


	// Hollow Knight
	GLboolean hk_parado;
	GLboolean hk_dash;
	GLboolean bs_ulti;


	// Tnt de Crash Bandicoot
	GLboolean tnt_normal;
	GLboolean tnt_explotando;


	// Mega caja brawl stars
	GLboolean bs_megacaja;

	// Luces Spot
	GLboolean hk_lampara_1;
	GLboolean hk_lampara_2;


	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
	static void actualizarAnguloMovimiento(float& anguloMovimiento, float& dirAnguloMovimiento, float velocidadAngular, float topeInf, float topeSup);

};

