#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

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

	GLboolean getPrendida() { return luzPrendida; }
	GLboolean getPrendida2() { return luzPrendida2; }

	GLfloat getLucesPuntuales() { return lucesPuntuales; }
	GLfloat getLucesSpotLight() { return lucesSpot; }

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

	// Puerta
	GLboolean puertaCerrada;
	GLboolean puertaAbriendose;


	// Hollow Knight
	GLboolean hk_parado;
	GLboolean hk_dash;


	// Tnt de Crash Bandicoot
	GLboolean tnt_normal;
	GLboolean tnt_explotando;


	// Mega caja brawl stars
	GLboolean bs_megacaja;


	GLboolean luzPrendida;
	GLboolean luzPrendida2;
	GLfloat lucesPuntuales;
	GLfloat lucesSpot;

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

