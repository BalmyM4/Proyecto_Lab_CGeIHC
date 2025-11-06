#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	muevex = 0.0f;
	adelante = true;

	
	// Crash bandicoot
	movex_cb = 0.0f;
	movez_cb = 0.0f;
	rotz_cb = 0.0f;


	// Puerta
	puertaCerrada = true;
	puertaAbriendose = false;

	// Hollow Knight
	hk_parado = true;
	hk_dash = false;

	// TNT de Crash Bandicoot
	tnt_normal = true;
	tnt_explotando = false;


	// brawl stars
	bs_megacaja = false;


	// Luces Spot
	hk_lampara_1 = true;
	hk_lampara_2 = true;



	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "PracticaXX:Nombre de la practica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if( key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		theWindow->hk_lampara_1 = !(theWindow->hk_lampara_1);
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		theWindow->hk_lampara_2 = !(theWindow->hk_lampara_2);
	}



	// Abrir puerta
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (!theWindow->puertaAbriendose)
		{
			theWindow->puertaAbriendose = true;
		}
	}

	// Dash de Hollow Knight
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		if (!theWindow->hk_dash)
		{
			theWindow->hk_dash = true;
		}
	}

	// Explotar TNT de Crash Bandicoot
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		if (theWindow->tnt_normal)
		{
			theWindow->tnt_normal = false;
		}
	}

	// Abrir megacaja
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		theWindow->bs_megacaja = !theWindow->bs_megacaja;
	}


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}


void Window::actualizarAnguloMovimiento(float& anguloMovimiento, float& dirAnguloMovimiento, float velocidadAngular, float topeInf, float topeSup)
{
	anguloMovimiento += velocidadAngular * dirAnguloMovimiento;

	if (anguloMovimiento >= topeSup)
	{
		anguloMovimiento = topeSup;
		dirAnguloMovimiento *= -1.0f;
	}
	else if (anguloMovimiento <= topeInf)
	{
		anguloMovimiento = topeInf;
		dirAnguloMovimiento *= -1.0f;
	}
}

void Window::procesarMovimiento(Camera& camera)
{
	glm::vec3 dir = camera.getCameraDirection();
	glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)));

	glm::vec3 move(0.0f);
	float velocidad = velocidad_cb;

	float topeSup = 60.0f;
	float topeInf = -60.0f;
	float velocidadAngular = 1.0f;

	float topeSup2 = 60.0f;
	float topeInf2 = 0.0f;
	float velocidadAngular2 = 1.0f;

	float topeSup3 = 60.0f;
	float topeInf3 = 0.0f;
	float velocidadAngular3 = 1.0f;

	bool moviendo = false;


	if (keys[GLFW_KEY_T])  // Adelante
	{
		move += dir * velocidad;
		moviendo = true;
		anguloMovimiento += velocidadAngular * dirAnguloMovimiento;
		actualizarAnguloMovimiento(anguloMovimiento, dirAnguloMovimiento, velocidadAngular, topeInf, topeSup);
		actualizarAnguloMovimiento(anguloMovimiento2, dirAnguloMovimiento2, velocidadAngular2, topeInf2, topeSup2);
		actualizarAnguloMovimiento(anguloMovimiento3, dirAnguloMovimiento3, velocidadAngular3, topeInf3, topeSup3);

	}
	if (keys[GLFW_KEY_G])  // Atrás
	{
		move -= dir * velocidad;
		moviendo = true;
		actualizarAnguloMovimiento(anguloMovimiento, dirAnguloMovimiento, velocidadAngular, topeInf, topeSup);
		actualizarAnguloMovimiento(anguloMovimiento2, dirAnguloMovimiento2, velocidadAngular2, topeInf2, topeSup2);
		actualizarAnguloMovimiento(anguloMovimiento3, dirAnguloMovimiento3, velocidadAngular3, topeInf3, topeSup3);
	}
		
	if (keys[GLFW_KEY_H])  // Derecha
	{
		move += right * velocidad;
		moviendo = true;
		actualizarAnguloMovimiento(anguloMovimiento, dirAnguloMovimiento, velocidadAngular, topeInf, topeSup);
		actualizarAnguloMovimiento(anguloMovimiento2, dirAnguloMovimiento2, velocidadAngular2, topeInf2, topeSup2);
		actualizarAnguloMovimiento(anguloMovimiento3, dirAnguloMovimiento3, velocidadAngular3, topeInf3, topeSup3);
	}
		
	if (keys[GLFW_KEY_F])  // Izquierda
	{
		move -= right * velocidad;
		moviendo = true;
		actualizarAnguloMovimiento(anguloMovimiento, dirAnguloMovimiento, velocidadAngular, topeInf, topeSup);
		actualizarAnguloMovimiento(anguloMovimiento2, dirAnguloMovimiento2, velocidadAngular2, topeInf2, topeSup2);
		actualizarAnguloMovimiento(anguloMovimiento3, dirAnguloMovimiento3, velocidadAngular3, topeInf3, topeSup3);
	}
		
	// Si no hay movimiento, regresar los ángulos al valor neutro
	if (!moviendo)
	{
		float velocidadRegreso = 2.0f;  // qué tan rápido vuelve
		float epsilon = 2.0f;           // zona muerta: si está dentro, se fija a 0

		// Articulaciones 0
		if (fabs(anguloMovimiento) <= epsilon)
			anguloMovimiento = 0.0f;
		else if (anguloMovimiento > 0.0f)
			anguloMovimiento -= velocidadRegreso;
		else if (anguloMovimiento < 0.0f)
			anguloMovimiento += velocidadRegreso;

		// Piernas 1
		if (fabs(anguloMovimiento2) <= epsilon)
			anguloMovimiento2 = 0.0f;
		else if (anguloMovimiento2 > 0.0f)
			anguloMovimiento2 -= velocidadRegreso;
		else if (anguloMovimiento2 < 0.0f)
			anguloMovimiento2 += velocidadRegreso;

		// Brazos 1
		if (fabs(anguloMovimiento3 - 60.0f) <= epsilon)
			anguloMovimiento3 = 60.0f;
		else if (anguloMovimiento3 > 60.0f)
			anguloMovimiento3 -= velocidadRegreso;
		else if (anguloMovimiento3 < 60.0f)
			anguloMovimiento3 += velocidadRegreso;
	}

	// Actualizar posición
	movex_cb += move.x;
	movez_cb += move.z;


	// Calcular rotación del personaje según dirección del movimiento
	if (glm::length(glm::vec2(move.x, move.z)) > 0.001f)
		rotz_cb = glm::degrees(atan2(-move.x, -move.z));
}


void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
