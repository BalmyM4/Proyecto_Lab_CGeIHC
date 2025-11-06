#ifndef KEYFRAMES_AGUILA_H
#define KEYFRAMES_AGUILA_H

#include <iostream>
#include <random>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iomanip> 

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

#include <string>


#define MAX_FRAMES 100

// ===== Estructura de un frame =====
typedef struct _frame {
    float mov_x, mov_y, mov_z;
    float mov_xInc, mov_yInc, mov_zInc;
    float giro, giroInc;
} FRAME;

// ===== Variables globales =====
extern FRAME KeyFrame[MAX_FRAMES];
extern int FrameIndex;
extern int i_max_steps;
extern int i_curr_steps;
extern bool play;
extern int playIndex;

// ===== Funciones =====
void saveFrame(float& mov_x, float& mov_y, float& mov_z, float& giro);
void resetElements(float& mov_x, float& mov_y, float& mov_z, float& giro);
void interpolation();
void animate(float& mov_x, float& mov_y, float& mov_z, float& giro);
void CargarKeyframesDesdeArchivo(const char* fileLocation, float& mov_x, float& mov_y, float& mov_z, float& giro);
void AgregarKeyframeEnArchivo(const char* fileLocation, float x, float y, float z, float giro);
void inputKeyframes(bool* keys, float& mov_x, float& mov_y, float& mov_z, float& giro);

#endif
