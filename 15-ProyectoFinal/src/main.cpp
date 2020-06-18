#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Shader Freetype include
//#include "Headers/Shaderft.h"


// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//freetype
#include <ft2build.h>
#include FT_FREETYPE_H 

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// Shadow box include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define TAMCOIN 16
#define TAMGEMA 7

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow *window;
GLFWwindow *window2;

//Para freetype
struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;
unsigned int VAO, VBO;
FT_Face face;
FT_Library ft;

Shader shader;
//Shader con skybox
Shader shaderSkyBox;
Shader shaderSkyboxDia;
Shader shaderSkyboxNoche;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;
//Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
//Shader para el texto
Shader shaderText;
//Shader para las particulas del topito
Shader shaderParticlesTopito;
//Shader para las particulas de nitro
Shader shaderParticlesNitro;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 15.0; //25
int cambiarCamara = 0; // Permite cambiar el distance target
bool enableCamaraSelected = true;

bool enableGirarCamaraSelected = true;
int girarCamara = 2;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Cylinder cylinderCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;

ShadowBox * shadowBox;

Box boxInicio;
Box boxFinal;

glm::vec3 colorLetras = glm::vec3(0.5f, 0.5f, 0.0f);
float contLetras = 0;

// Models complex instances
Model modelLambo;
Model modelLamboLeftDor;
Model modelLamboRightDor;
Model modelLamboFrontLeftWheel;
Model modelLamboFrontRightWheel;
Model modelLamboRearLeftWheel;
Model modelLamboRearRightWheel;

float velocidad = 0.0f;

float moverAku = 0.0f;
int stateAku = 0;

bool creditos = false;
int contCreditos = 0;


// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;
// Hierba
Model modelGrass;
// Fountain
Model modelFountain;
// Model animate instance
// Mayow
Model mayowModelAnimate;

//Modelos de escenografia
Model fabricaModel;
Model gymModel;
Model houseModel;
Model track1Model;
Model semaforoModel;

Model akuModel;

//Model animate harley
Model harleyModel;
//Model animate flash
Model flashModel;
//Model animate Superman
Model supermanModel;
//Modelos de obstaculos
Model TopitoModel;

//Modelos pared
Model muro1Model;
Model muro2Model;

//Modelo arco
Model arcoModel;

//Modelos de ganar y perder 
Model ganarModel;
Model perderModel;

//Recorrido
Model HoohModel;
//Eclipse
Model modelEclipse;
Model modelEclipseLLDelanteras;
Model modelEclipseLLTraseras;

// Posiciones de coin
std::vector<glm::vec3> coinPosition = { glm::vec3(35.55, 0, -91.41), glm::vec3(91.8, 0, -46.48),
	glm::vec3(53.12, 0, 21.09), glm::vec3(-3.12, 0, -12.89), glm::vec3(-38.67, 0, -33.98),
	glm::vec3(-69.53, 0, -0.78), glm::vec3(-58.2, 0, 52.34), glm::vec3(-25.78, 0, 60.55),
	glm::vec3(33.59, 0, 53.52), glm::vec3(83.2, 0, 80.86), glm::vec3(-10.94, 0, 87.89),
	glm::vec3(-65.62, 0, 81.64), glm::vec3(-85.55, 0, 35.55), glm::vec3(-91.02, 0, -3.52),
	glm::vec3(-84.38, 0, -44.53), glm::vec3(-61.33, 0, -67.58) };

std::string coinValor[TAMCOIN] = { "coin0", "coin1", "coin2", "coin3", "coin4", "coin5", "coin6", "coin7",
"coin8", "coin9", "coin10", "coin11", "coin12", "coin13", "coin14", "coin15" };

// Modelo para monedas
Model modelCoin[TAMCOIN];

// Posiciones de gemas
std::vector<glm::vec3> gemaPosition = { glm::vec3(69.14, 0, -83.2), glm::vec3(83.2, 0, 0.0),
	glm::vec3(16.8, 0, 12.11), glm::vec3(5.08, 0, 62.11), glm::vec3(90.62, 0, 51.17),
	glm::vec3(29.69, 0, 85.16), glm::vec3(-91.41, 0, 61.33) };

std::string gemaValor[TAMGEMA] = { "gemas0", "gemas1", "gemas2", "gemas3", "gemas4", "gemas5", "gemas6" };

// Models para gemas
Model modelGema[TAMGEMA];

//Posiciones para el arco
std::vector<glm::vec3> arcoPosition = { glm::vec3(92.97, 0, -39.84), glm::vec3(-32.03, 0, -36.72),
	glm::vec3(-15.23, 0, 63.89), glm::vec3(18.75, 0, 84.88), glm::vec3(-87.89, 0, 17.19) };

//Posicion del topito
std::vector<glm::vec3> posicionTopito = { glm::vec3(-8.59, 0, -74.61), glm::vec3(-8.98, 0, -38.28),
	glm::vec3(-55.47, 0, -2.34), glm::vec3(7.03, 0, 53.52), glm::vec3(-78.91, 0, 75.0) };

std::string gemaModelo[TAMGEMA] = { "../models/Proyecto/Gemas/GemGreen.obj",
"../models/Proyecto/Gemas/GemGreenHud.obj", "../models/Proyecto/Gemas/GemPurple.obj",
"../models/Proyecto/Gemas/GemRed.obj", "../models/Proyecto/Gemas/GemYellow.obj",
"../models/Proyecto/Gemas/GemGreenHud.obj", "../models/Proyecto/Gemas/GemPurple.obj" };


// Terrain model instance
Terrain terrain(-1, -1, 200, 12, "../Textures/testHeightProyecto2.png");
// Terreno para limitar pista
Terrain terrainLogic(-1, -1, 200, 12, "../Textures/logicTextureMap2.png");
// Terreno para detectar inicio
Terrain terrainLogicInicio(-1, -1, 200, 12, "../Textures/logicTextureMapInicio.png");

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;
GLuint textureVidaID;
GLuint textureParticleTopitoID, textureParticleNitroID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

// Skybox
std::string fileNames[1][6] = {
	  { "../Textures/Isla/Isla1_ft.png",
		"../Textures/Isla/Isla1_bk.png",
		"../Textures/Isla/Isla1_up.png",
		"../Textures/Isla/Isla1_dn.png",
		"../Textures/Isla/Isla1_rt.png",
		"../Textures/Isla/Isla1_lf.png"
		} };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

//Variables para el tiempo

int timecount = 0;
bool enabletime = false;
int minutos = 0;
int segundos = 0; //Tiempo inicial 20 segundos

//Menu
bool presionaTecla = false;

// Vector de matrices para coin
std::vector<glm::mat4> matrixModelCoin = { glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0),
glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0),
glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0) };

bool isCollisionCoin[TAMCOIN];

// Vector de matrices para gemas
std::vector<glm::mat4> matrixModelGemas = { glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0),
glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0) };

bool isCollisionGema[TAMGEMA];

// Model matrix definitions
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixMayow = glm::mat4(1.0f);
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);

// Model matrix escenografia
glm::mat4 modelMatrixFabrica = glm::mat4(1.0f);
glm::mat4 modelMatrixGym = glm::mat4(1.0f);
glm::mat4 modelMatrixHouse = glm::mat4(1.0f);
glm::mat4 modelMatrixTrack1 = glm::mat4(1.0f);
glm::mat4 modelMatrixHarley = glm::mat4(1.0f);
glm::mat4 modelMatrixFlash = glm::mat4(1.0f);
glm::mat4 modelMatrixSuperman = glm::mat4(1.0f);

glm::mat4 modelMatrixAku = glm::mat4(1.0f);
glm::mat4 modelMatrixCamaraFinal = glm::mat4(1.0f);

// Model matrix muros
glm::mat4 modelMatrixMuro1 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro2 = glm::mat4(1.0f);

//Model matrix Arco
glm::mat4 modelMatrixArco = glm::mat4(1.0f);

//Semaforo
glm::mat4 modelMatrixSemaforo = glm::mat4(1.0f);

//Model matrix ganar/perder
glm::mat4 modelMatrixGanar = glm::mat4(1.0f);
glm::mat4 modelMatrixPerder = glm::mat4(1.0f);

//Matrices de los obstaculos
glm::mat4 modelMatrixTopito = glm::mat4(1.0);
int animationIndex = 1;
int modelSelected = 1;
bool enableCountSelected = true;

//Eclipse
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);

//Recorido
glm::mat4 modelMatrixHooh = glm::mat4(1.0);

//Variables para Animacion del coche
bool avanza = false;
bool retrocede = false;
bool frenado1 = false;
bool frenado2 = false;
bool frenar = false;
bool chocar = false;
bool deteccionEnfrente = false;
bool deteccionAtras = false;
float vida = 100;
float contVida = 0;
bool vuelta = false;
int vueltaCont = 0;
bool iniciaCarrera = false;
bool ganar = false;

//Variables para los obstaculos
int ActivaTopito = 0;
bool subetopito = true;
float MovimientoenYTopito = 0.0f;

//Variables para inicios
bool harley = true;
bool flash = true;
bool superman = true;

//Variable para cambiar posicion de los arcos
bool detectarArco = false;
int contadorArco = 0;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false, ActivaRecorrido = false;
float rotacionLLantas; //Para el eclipse

// Recorrido Hooh
std::vector<std::vector<glm::mat4>> keyFramesHooh;
int numPasosHooh = 0;
int indexFrameHooh = 0;
int indexFrameHoohNext = 1;
float interpolationHooh = 0.0;
int maxNumPasosHooh = 100;

// Recorrido Eclipse
std::vector<std::vector<glm::mat4>> keyFramesEclipse;
int numPasosEclipse = 0;
int indexFrameEclipse = 0;
int indexFrameEclipseNext = 1;
float interpolationEclipse = 0.0;
int maxNumPasosEclipse = 2;

// Var animate Coin
float rotCoin = 0.1;

// Var animate Nitro
int nitro = 0;
bool usarNitro = false;
float contadorNitro = 0.0;

//Vectores para la luz direccional 
//Crear los efectos de dia, tarde y noche

std::vector<glm::vec3> Ambiental = { glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.12, 0.12, 0.12), glm::vec3(0.005, 0.005, 0.005) };

std::vector<glm::vec3> Difusa = { glm::vec3(0.5, 0.5, 0.5),
			glm::vec3(0.6, 0.3, 0.15), glm::vec3(0.009, 0.009, 0.009) };

std::vector<glm::vec3> Especular = { glm::vec3(0.2, 0.2, 0.2),
			glm::vec3(0.09, 0.09, 0.09), glm::vec3(0.01, 0.01, 0.01) };

int SelectorDeDia = vueltaCont;

std::vector<glm::vec3> ColoresFog = { glm::vec3(0.6, 0.6, 0.6),glm::vec3(0.7, 0.35, 0.175),glm::vec3(0.2,0.2,0.2) };
//Shader para el skybox, terreno y ambiente
std::string  ShaderDelSkybox[3] = { "../Shaders/skyBox.fs" , "../Shaders/skyBox.fs" ,
"../Shaders/skyBox_fog.fs" };

// Lamps positions
std::vector<glm::vec3> lamp1Position = { glm::vec3(5.08, 0, -56.25),
glm::vec3(-63.28, 0, 58.2), glm::vec3(-66.41, 0, -26.95), };
std::vector<float> lamp1Orientation = { -17.0, -82.67, 23.70 };
std::vector<glm::vec3> lamp2Position = { glm::vec3(-95.31, 0, -73.05),
glm::vec3(-13.28, 0, -75.78) };
std::vector<float> lamp2Orientation = { 21.37 + 90, -65.0 + 90 };

// Semaforo
glm::vec3 posicionSemaforo = glm::vec3(-6.0f, 5.0f, -65.0f);
std::vector<glm::vec3> SemaforoRojo = { posicionSemaforo + (0.0f,0.0f,-2.75f),
	posicionSemaforo + (0.0f,0.0f, -0.75f), posicionSemaforo + (0,0,0.75f) };
std::vector<glm::vec3> SemaforoVerde = { posicionSemaforo + (0.0f, 0.0f, 2.25f) };

bool enableSemaforo = true, iniciaSemaforo = false, Verde = false, RenderSemaforo = true;
int contRoja = 0;
int tiempoSemaforo = 0, tiempoSemaforoAcumulado = 0;

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"lambo", glm::vec3(-28.12, 0.0, -62.0)},
		{"fountain", glm::vec3(64.84, 0, 63.28)},
		{"fire", glm::vec3(-5.0, 0.0, -40.0)},
		{"topito", posicionTopito[0]},
		{"nitro", glm::vec3(0.0, 0.0, 0.0)}
};

double deltaTime;
double currTime, lastTime;

// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 1.0, particleLifetime = 3.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;

// Definition for the particle system nitro
GLuint initVelNitro, startTimeNitro;
GLuint VAOParticlesNitro;
GLuint nParticlesNitro = 2000;
GLuint posBufNitro[2], velBufNitro[2], ageNitro[2];
GLuint particleArrayNitro[2];
GLuint feedbackNitro[2];
GLuint drawBufNitro = 1;
float particleSizeNitro = 0.5, particleLifetimeNitro = 3.0;
double currTimeParticlesAnimationNitro, lastTimeParticlesAnimationNitro;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;
std::map<std::string, std::tuple<AbstractModel::CBB, glm::mat4, glm::mat4> > collidersCBB;

// Framesbuffers
GLuint depthMap, depthMapFBO;

//Rotacion del carro
glm::vec3 VectorRotacion = glm::vec3(0.0f, 1.0f, 0.0f);
float AnguloAnterior = 0.0f;

/**********************
 * OpenAL config
 */

 // OpenAL Defines
#define NUM_BUFFERS 31
#define NUM_SOURCES 31
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2 Acelerar
ALfloat source2Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Source 3, Perder
ALfloat source3Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };
// Source 4, Ganar
ALfloat source4Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source4Vel[] = { 0.0, 0.0, 0.0 };
// Source 5, Creditos
ALfloat source5Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source5Vel[] = { 0.0, 0.0, 0.0 };
// Source 6, Chocar
ALfloat source6Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source6Vel[] = { 0.0, 0.0, 0.0 };
// Source 7, Coin
ALfloat source7Pos[TAMCOIN][3] = { 0.0, 0.0, 0.0 };
ALfloat source7Vel[TAMCOIN][3] = { 0.0, 0.0, 0.0 };

// Source 8, Coin
ALfloat source8Pos[TAMGEMA][3] = { 0.0, 0.0, 0.0 };
ALfloat source8Vel[TAMGEMA][3] = { 0.0, 0.0, 0.0 };

// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = { true, true, true, true, true, true, true };
std::vector<bool> sourcesPlayCoin = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
std::vector<bool> sourcesPlayGema = { true, true, true, true, true, true, true };

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void initParticleBuffersTopito();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);
void RenderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);
float Angulo(glm::vec3 P, glm::vec3 Q);

float Angulo(glm::vec3 P, glm::vec3 Q) {
	float Angulo = glm::dot(P, Q);

	if (Angulo > 1.0) {
		Angulo = 1.0;
	}
	else if (Angulo < -1.0) {
		Angulo = -1.0;
	}
	else
		Angulo = Angulo;

	return acos(Angulo);
}

void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader.turnOn();
	shader.setVectorFloat3("textColor", color.x, color.y, color.z);
	//	glUniform3f(shader.getUniformLocation("textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

//Particulas para el topito
GLuint initVelTopito, startTimeTopito;
GLuint VAOParticlesTopito;
GLuint nParticlesTopito = 10000;
double currTimeParticlesAnimationTopito, lastTimeParticlesAnimationTopito;

void initParticleBuffersTopito() {
	// Generate the buffers
	glGenBuffers(1, &initVelTopito);   // Initial velocity buffer
	glGenBuffers(1, &startTimeTopito); // Start time buffer

	// Allocate space for all buffers
	int size = nParticlesTopito * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVelTopito);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTimeTopito);
	glBufferData(GL_ARRAY_BUFFER, nParticlesTopito * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticlesTopito * 3];
	for (unsigned int i = 0; i < nParticlesTopito; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVelTopito);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticlesTopito];
	float time = 0.0f;
	float rate = 0.0009f;
	for (unsigned int i = 0; i < nParticlesTopito; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTimeTopito);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticlesTopito * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticlesTopito);
	glBindVertexArray(VAOParticlesTopito);
	glBindBuffer(GL_ARRAY_BUFFER, initVelTopito);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTimeTopito);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for (unsigned int i = 0; i < nParticlesFire; i++) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine{};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

//Nitro
void initParticleBuffersNitro() {
	// Generate the buffers
	glGenBuffers(2, posBufNitro);    // position buffers
	glGenBuffers(2, velBufNitro);    // velocity buffers
	glGenBuffers(2, ageNitro);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesNitro * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBufNitro[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBufNitro[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBufNitro[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBufNitro[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, ageNitro[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, ageNitro[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesNitro);
	float rate = particleLifetime / nParticlesNitro;
	for (unsigned int i = 0; i < nParticlesNitro; i++) {
		int index = i - nParticlesNitro;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine{};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, ageNitro[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArrayNitro);

	// Set up particle array 0
	glBindVertexArray(particleArrayNitro[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBufNitro[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBufNitro[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, ageNitro[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArrayNitro[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBufNitro[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBufNitro[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, ageNitro[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedbackNitro);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedbackNitro[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBufNitro[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBufNitro[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, ageNitro[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedbackNitro[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBufNitro[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBufNitro[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, ageNitro[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkyboxDia.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderSkyboxNoche.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", { "Position", "Velocity", "Age" });
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	shaderText.initialize("../Shaders/text.vs", "../Shaders/text.fs");
	shaderSkyBox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderParticlesTopito.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderParticlesNitro.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", { "Position", "Velocity", "Age" });

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkyBox);
	skyboxSphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinderCollider.init();
	cylinderCollider.setShader(&shader);
	cylinderCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);

	boxInicio.init();
	boxInicio.setShader(&shaderMulLighting);
	boxFinal.init();
	boxFinal.setShader(&shaderMulLighting);

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	terrainLogic.init();
	terrainLogic.setPosition(glm::vec3(100, 0, 100));

	terrainLogicInicio.init();
	terrainLogicInicio.setPosition(glm::vec3(100, 0, 100));

	// Lamborginhi
	modelLambo.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	//Se inicializan las coins
	for (int i = 0; i < TAMCOIN; i++)
	{
		modelCoin[i].loadModel("../models/Proyecto/Coin/Bells.obj");
		modelCoin[i].setShader(&shaderMulLighting);
	}

	//Se inicializan las gemas
	for (int i = 0; i < TAMGEMA; i++)
	{
		modelGema[i].loadModel(gemaModelo[i]);
		modelGema[i].setShader(&shaderMulLighting);
	}

	//Lamp models
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.loadModel("../models/Street_Light/LampPost.obj");
	modelLampPost2.setShader(&shaderMulLighting);

	//Grass
	modelGrass.loadModel("../models/grass/grassModel.obj");
	modelGrass.setShader(&shaderMulLighting);

	//Fountain
	modelFountain.loadModel("../models/fountain/fountain.obj");
	modelFountain.setShader(&shaderMulLighting);

	//Mayow
	mayowModelAnimate.loadModel("../models/mayow/personaje2.fbx");
	mayowModelAnimate.setShader(&shaderMulLighting);

	//Fabrica
	fabricaModel.loadModel("../models/Proyecto/Fabrica/farm_build_10.obj");
	fabricaModel.setShader(&shaderMulLighting);
	//Gym
	gymModel.loadModel("../models/Proyecto/Gym/farm_build_01.obj");
	gymModel.setShader(&shaderMulLighting);
	//Casa
	houseModel.loadModel("../models/Proyecto/House/farm_build_07.obj");
	houseModel.setShader(&shaderMulLighting);

	//Aku
	akuModel.loadModel("../models/Aku Aku/aku_aku.obj");
	akuModel.setShader(&shaderMulLighting);

	//Track1
	track1Model.loadModel("../models/Proyecto/ChaoRace/track8V2.obj");
	track1Model.setShader(&shaderMulLighting);
	//Harley
	harleyModel.loadModel("../models/Harley/Harley.fbx");
	harleyModel.setShader(&shaderMulLighting);

	//Flash
	flashModel.loadModel("../models/Flash/Flash4.fbx");
	flashModel.setShader(&shaderMulLighting);

	//Superman
	supermanModel.loadModel("../models/Proyecto/Superman/SupermanZombie.fbx");
	supermanModel.setShader(&shaderMulLighting);


	//Muros
	muro1Model.loadModel("../models/Proyecto/Bardas/Wall/Models and Textures/wall.obj");
	muro1Model.setShader(&shaderMulLighting);

	muro2Model.loadModel("../models/Proyecto/Bardas/Destroid Walls/DestroyableWalls2.obj");
	muro2Model.setShader(&shaderMulLighting);

	arcoModel.loadModel("../models/Proyecto/PropuestasPortones/Propuesta1.obj");
	arcoModel.setShader(&shaderMulLighting);

	//Ganar/Perder
	ganarModel.loadModel("../models/Proyecto/Ganaste Perdiste/Ganaste.obj");
	ganarModel.setShader(&shaderMulLighting);

	perderModel.loadModel("../models/Proyecto/Ganaste Perdiste/Perdiste.obj");
	perderModel.setShader(&shaderMulLighting);

	//Topo
	TopitoModel.loadModel("../models/Proyecto/Topo/Topo.obj");
	TopitoModel.setShader(&shaderMulLighting);

	//Semaforo 
	semaforoModel.loadModel("../models/Proyecto/Semaforo/Semaforo.obj");
	semaforoModel.setShader(&shaderMulLighting);

	//Hooh
	HoohModel.loadModel("../models/Ho-Oh/HoohV1.fbx");
	HoohModel.setShader(&shaderMulLighting);

	//Eclipse
	modelEclipse.loadModel("../models/Eclipse/2003eclipse_chasis.obj");
	modelEclipse.setShader(&shaderMulLighting);
	modelEclipseLLDelanteras.loadModel("../models/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseLLDelanteras.setShader(&shaderMulLighting);
	modelEclipseLLTraseras.loadModel("../models/Eclipse/2003eclipse_rear_wheels.obj");
	modelEclipseLLTraseras.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	FIBITMAP *bitmap2;
	unsigned char *data;


	// FreeType
	// --------
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight));
	shader.turnOn();
	shaderText.setMatrix4("projection", 1, GL_FALSE, glm::value_ptr(projection));

	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// load font as face
	if (FT_New_Face(ft, "C:/Windows/Fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load first 128 characters of ASCII set
	for (unsigned char c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);


	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//End freetype

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[0][i]);

		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

// Definiendo la textura a utilizar
	Texture textureTerrainBackground("../Textures/grassy2.png");
	bitmap = textureTerrainBackground.loadImage();
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
		imageHeight);
	glGenTextures(1, &textureTerrainBackgroundID);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainR("../Textures/highway3.jpg");
	bitmap = textureTerrainR.loadImage();
	data = textureTerrainR.convertToData(bitmap, imageWidth,
		imageHeight);
	glGenTextures(1, &textureTerrainRID);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainG("../Textures/inicio2.jpg");
	bitmap = textureTerrainG.loadImage();
	data = textureTerrainG.convertToData(bitmap, imageWidth,
		imageHeight);
	glGenTextures(1, &textureTerrainGID);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainB("../Textures/grass2.jpg");
	bitmap = textureTerrainB.loadImage();
	data = textureTerrainB.convertToData(bitmap, imageWidth,
		imageHeight);
	glGenTextures(1, &textureTerrainBID);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBlendMap("../Textures/blendMapProyecto3.png");
	bitmap = textureTerrainBlendMap.loadImage(true);
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
		imageHeight);
	glGenTextures(1, &textureTerrainBlendMapID);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureTerrainBlendMap.freeImage(bitmap);

	Texture textureParticlesFountain("../Textures/bluewater.png");
	bitmap = textureParticlesFountain.loadImage();
	data = textureParticlesFountain.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFountainID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticlesFountain.freeImage(bitmap);

	//Topito
	Texture textureParticlesTopito("../Textures/tierra.png");
	bitmap = textureParticlesTopito.loadImage();
	data = textureParticlesTopito.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleTopitoID);
	glBindTexture(GL_TEXTURE_2D, textureParticleTopitoID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticlesTopito.freeImage(bitmap);

	//Particulas fuego
	Texture textureParticleFire("../Textures/fire.png");
	bitmap = textureParticleFire.loadImage();
	data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 1.0f); //Ancho particula fuego
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for (int i = 0; i < randData.size(); i++) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f, 0.1f, 0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(1.5f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0, 1, 0);
	n = glm::cross(glm::vec3(1, 0, 0), v);
	if (glm::length(n) < 0.00001f) {
		n = glm::cross(glm::vec3(0, 1, 0), v);
	}
	u = glm::cross(v, n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	//Nitro
	Texture textureParticleNitro("../Textures/Nitro4.png");
	bitmap = textureParticleNitro.loadImage();
	data = textureParticleNitro.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleNitroID);
	glBindTexture(GL_TEXTURE_2D, textureParticleNitroID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleNitro.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01Nitro = std::uniform_real_distribution<float>(0.0f, 0.03f);
	std::mt19937 generatorNitro;
	std::random_device rdNitro;
	generatorNitro.seed(rdNitro());
	int sizeNitro = nParticlesNitro * 2;
	std::vector<GLfloat> randDataNitro(sizeNitro);
	for (int i = 0; i < randDataNitro.size(); i++) {
		randDataNitro[i] = distr01Nitro(generatorNitro);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, sizeNitro);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, sizeNitro, GL_RED, GL_FLOAT, randDataNitro.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesNitro.setInt("Pass", 1);
	shaderParticlesNitro.setInt("ParticleTex", 0);
	shaderParticlesNitro.setInt("RandomTex", 1);
	shaderParticlesNitro.setFloat("ParticleLifetime", particleLifetimeNitro);
	shaderParticlesNitro.setFloat("ParticleSize", particleSizeNitro);
	shaderParticlesNitro.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f, 0.1f, 0.0f)));
	shaderParticlesNitro.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basisNitro;
	glm::vec3 uNitro, vNitro, nNitro;
	vNitro = glm::vec3(0, 1, 0);
	nNitro = glm::cross(glm::vec3(1, 0, 0), vNitro);
	if (glm::length(nNitro) < 0.00001f) {
		nNitro = glm::cross(glm::vec3(0, 1, 0), vNitro);
	}
	uNitro = glm::cross(vNitro, nNitro);
	basisNitro[0] = glm::normalize(uNitro);
	basisNitro[1] = glm::normalize(vNitro);
	basisNitro[2] = glm::normalize(nNitro);
	shaderParticlesNitro.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basisNitro));

	/*******************************************
	 * Inicializacion de los buffers
	 *******************************************/
	initParticleBuffers();
	initParticleBuffersTopito();

	/*******************************************
	* Inicializacion de los buffers
	*******************************************/
	initParticleBuffers();

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffersFire();
	initParticleBuffersNitro();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Config source 0
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav"); //Fuente
	buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav"); // Fuego
	buffer[2] = alutCreateBufferFromFile("../sounds/carAcel.wav"); //Acelerar
	buffer[3] = alutCreateBufferFromFile("../sounds/nelson-aha.wav"); //Perder
	buffer[4] = alutCreateBufferFromFile("../sounds/youWin.wav"); //Ganar
	buffer[5] = alutCreateBufferFromFile("../sounds/gracias.wav"); //Creditos
	buffer[6] = alutCreateBufferFromFile("../sounds/chocar.wav"); //Chocar
	buffer[7] = alutCreateBufferFromFile("../sounds/coin.wav"); //Ganar
	buffer[8] = alutCreateBufferFromFile("../sounds/gema.wav"); //Ganar

	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR) {
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}


	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 2.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 2.0f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 2000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 1.0f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 500);

	alSourcef(source[3], AL_PITCH, 1.0f); //Que tan rapido se reproduce el sonido
	alSourcef(source[3], AL_GAIN, 3.0f); //Ganacia, que tan fuerte se escucha
	alSourcefv(source[3], AL_POSITION, source3Pos); //Posicion
	alSourcefv(source[3], AL_VELOCITY, source3Vel); //Velocidad
	alSourcei(source[3], AL_BUFFER, buffer[3]); //Buffer asociado a esa fuente
	alSourcei(source[3], AL_LOOPING, AL_FALSE); //Una vez que ya se reprodujo una vez el sonido volverlo hacer o no
	alSourcef(source[3], AL_MAX_DISTANCE, 500); //Umbral para saber cuanto se tiene que alejar para que se escuche menos el sonido

	alSourcef(source[4], AL_PITCH, 1.0f);
	alSourcef(source[4], AL_GAIN, 3.0f); 
	alSourcefv(source[4], AL_POSITION, source4Pos); 
	alSourcefv(source[4], AL_VELOCITY, source4Vel); 
	alSourcei(source[4], AL_BUFFER, buffer[4]); 
	alSourcei(source[4], AL_LOOPING, AL_FALSE); 
	alSourcef(source[4], AL_MAX_DISTANCE, 500); 

	alSourcef(source[5], AL_PITCH, 1.0f);
	alSourcef(source[5], AL_GAIN, 3.0f);
	alSourcefv(source[5], AL_POSITION, source5Pos);
	alSourcefv(source[5], AL_VELOCITY, source5Vel);
	alSourcei(source[5], AL_BUFFER, buffer[5]);
	alSourcei(source[5], AL_LOOPING, AL_FALSE);
	alSourcef(source[5], AL_MAX_DISTANCE, 500);

	alSourcef(source[6], AL_PITCH, 1.0f);
	alSourcef(source[6], AL_GAIN, 3.0f);
	alSourcefv(source[6], AL_POSITION, source6Pos);
	alSourcefv(source[6], AL_VELOCITY, source6Vel);
	alSourcei(source[6], AL_BUFFER, buffer[6]);
	alSourcei(source[6], AL_LOOPING, AL_FALSE);
	alSourcef(source[6], AL_MAX_DISTANCE, 500);

	for (int i = 0; i < TAMCOIN; i++)
	{
		alSourcef(source[7 + i], AL_PITCH, 1.0f);
		alSourcef(source[7 + i], AL_GAIN, 3.0f);
		alSourcefv(source[7 + i], AL_POSITION, source7Pos[i]);
		alSourcefv(source[7 + i], AL_VELOCITY, source7Vel[i]);
		alSourcei(source[7 + i], AL_BUFFER, buffer[7]);
		alSourcei(source[7 + i], AL_LOOPING, AL_FALSE);
		alSourcef(source[7 + i], AL_MAX_DISTANCE, 500);
	}

	for (int i = 0; i < TAMGEMA; i++)
	{
		alSourcef(source[23 + i], AL_PITCH, 1.0f);
		alSourcef(source[23 + i], AL_GAIN, 3.0f);
		alSourcefv(source[23 + i], AL_POSITION, source8Pos[i]);
		alSourcefv(source[23 + i], AL_VELOCITY, source8Vel[i]);
		alSourcei(source[23 + i], AL_BUFFER, buffer[8]);
		alSourcei(source[23 + i], AL_LOOPING, AL_FALSE);
		alSourcef(source[23 + i], AL_MAX_DISTANCE, 500);
	}

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkyBox.destroy();
	shaderSkyboxDia.destroy();
	shaderSkyboxNoche.destroy();
	shaderTerrain.destroy();
	shaderParticlesFountain.destroy();
	shaderParticlesFire.destroy();
	shaderParticlesTopito.destroy();
	shaderParticlesNitro.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	cylinderCollider.destroy();
	boxViewDepth.destroy();
	boxInicio.destroy();
	boxFinal.destroy();

	// Terrains objects Delete
	terrain.destroy();
	terrainLogic.destroy();
	terrainLogicInicio.destroy();

	//Destroy coins
	for (int i = 0; i < TAMCOIN; i++)
	{
		modelCoin[i].destroy();
	}

	//Destroy Gemas
	for (int i = 0; i < TAMGEMA; i++)
	{
		modelGema[i].destroy();
	}

	// Custom objects Delete
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelLamp1.destroy();
	modelLamp2.destroy();
	modelLampPost2.destroy();
	modelGrass.destroy();
	modelFountain.destroy();

	// Custom objects animate
	mayowModelAnimate.destroy();

	//Objects escenografia
	fabricaModel.destroy();
	gymModel.destroy();
	houseModel.destroy();
	track1Model.destroy();

	akuModel.destroy();

	harleyModel.destroy();
	flashModel.destroy();
	supermanModel.destroy();

	//Muros
	muro1Model.destroy();
	muro2Model.destroy();

	//Semaforo
	semaforoModel.destroy();

	//Arco
	arcoModel.destroy();

	//Ganar/Perder
	ganarModel.destroy();
	perderModel.destroy();

	//Obstaculos
	TopitoModel.destroy();
	modelEclipse.destroy();
	modelEclipseLLDelanteras.destroy();
	modelEclipseLLTraseras.destroy();

	//Recorrido
	HoohModel.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);
	glDeleteTextures(1, &textureParticleTopitoID);
	glDeleteTextures(1, &textureParticleNitroID);

	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fountain particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);

	// Remove the buffer of the nitro particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBufNitro);
	glDeleteBuffers(2, velBufNitro);
	glDeleteBuffers(2, ageNitro);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedbackNitro);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesNitro);

	//Remove topito
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVelTopito);
	glDeleteBuffers(1, &startTimeTopito);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesTopito);

}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		enableCountSelected = false;
		modelSelected++;
		if (modelSelected > 1)
			modelSelected = 0;
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	//Mayow animation
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(1.0f), glm::vec3(0, 1, 0));
		animationIndex = 0;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-1.0f), glm::vec3(0, 1, 0));
		animationIndex = 0;
	}if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, 0.05));
		animationIndex = 0;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, -0.05));
		animationIndex = 0;
	}

	//Joystick
	if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
		//std::cout << "Joystick conected" << std::endl;

		int axesCount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		//std::cout << "Number of axes available: " << axesCount << std::endl;

		int buttonCount;
		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		if (GLFW_PRESS == buttons[0] && presionaTecla == false) {
			presionaTecla = true; //Para desactivar la pantalla de inicio
			ActivaRecorrido = true;
			modelSelected = 0;
		}
		if (GLFW_PRESS == buttons[3] && modelSelected == 0) {
			ActivaRecorrido = false;
			modelSelected = 1;
			record = true;
			cambiarCamara++;
		}
		//Cambia la distancia de la camara en tercera persona
		if (enableCamaraSelected && modelSelected == 1 && (GLFW_PRESS == buttons[3]) && vida > 0 && ganar == false) {
			enableCamaraSelected = false;
			cambiarCamara++;
			if (cambiarCamara > 1)
				cambiarCamara = 0;
			if (cambiarCamara == 0)
				distanceFromTarget = 15;
			else if (cambiarCamara == 1)
				distanceFromTarget = 25;

			camera->setDistanceFromTarget(distanceFromTarget);
		}
		else if (GLFW_RELEASE == buttons[3])
			enableCamaraSelected = true;

		//Gira la camara
		if (enableGirarCamaraSelected && modelSelected == 1 && (GLFW_PRESS == buttons[1]) && vida > 0 && ganar == false) {
			enableGirarCamaraSelected = false;
			girarCamara = 0;
			std::cout << "Gira 1" << std::endl;

		}
		else if (GLFW_RELEASE == buttons[1] && girarCamara == 0) {
			girarCamara = 1;
			enableGirarCamaraSelected = true;
			std::cout << "Gira 2" << std::endl;

		}
		//Permite activar el nitro
		if (modelSelected == 1 && (GLFW_PRESS == buttons[0]) && nitro >= 100 && vida > 0 && ganar == false) {
			avanza = true;
			velocidad = 1.0;
			usarNitro = true;
			frenado1 = false;
			nitro = 0;
		}

		//Permite frenar
		if (modelSelected == 1 && (GLFW_PRESS == buttons[2]) && vida > 0 && ganar == false) {
			frenar = true;
		}

		//Permite girar
		if (modelSelected == 1 && (axes[0] < 0) && (avanza || retrocede)) {
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(-0.83827, 0.17224, 1.47627));
			modelMatrixLambo = glm::rotate(modelMatrixLambo, glm::radians(2.0f), glm::vec3(0, 1, 0));
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.83827, -0.17224, -1.47627));
			//std::cout << "Right Stick Y Axis Gira izquierda: " << axes[0] << std::endl; // tested with PS4 controller connected via micro USB cable
		}
		else if (modelSelected == 1 && (axes[0] > 0) && (avanza || retrocede)) {
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.83827, 0.17224, 1.47627));
			modelMatrixLambo = glm::rotate(modelMatrixLambo, -glm::radians(2.0f), glm::vec3(0, 1, 0));
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(-0.83827, -0.17224, -1.47627));
			//std::cout << "Right Stick Y Axis Gira derecha: " << axes[0] << std::endl; // tested with PS4 controller connected via micro USB cable
		}

		if (modelSelected == 1 && (axes[5] > 0) && vida > 0 && iniciaCarrera && ganar == false) {
			avanza = true;
			if (velocidad <= 0.5) {
				frenado1 = FALSE;
				velocidad += 0.05;
			}
			//std::cout << "Right Trigger/R2: " << axes[5] << std::endl; // tested with PS4 controller connected via micro USB cable
		}
		else if (modelSelected == 1 && (axes[4] > 0) && vida > 0 && iniciaCarrera && ganar == false) {
			retrocede = true;
			if (velocidad >= -0.5) {
				frenado2 = FALSE;
				velocidad -= 0.05;
			}
			//std::cout << "Left Trigger/L2: " << axes[4] << std::endl; // tested with PS4 controller connected via micro USB cable
		}
		else {
			if (velocidad > 0 && axes[5] <= 0)
				frenado1 = TRUE;
			if (velocidad < 0 && axes[4] <= 0)
				frenado2 = TRUE;
		}

	}
	else {
		//Lambo animation TECLADO
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && presionaTecla == false) {
			presionaTecla = true; //Para desactivar la pantalla de inicio
			ActivaRecorrido = true;
			modelSelected = 0;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && modelSelected == 0) {
			ActivaRecorrido = false;
			modelSelected = 1;
			record = true;
			cambiarCamara++;
		}

		//Cambia la distancia de la camara en tercera persona
		if (enableCamaraSelected && modelSelected == 1 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && vida > 0 && ganar == false) {
			enableCamaraSelected = false;
			cambiarCamara++;
			if (cambiarCamara > 1)
				cambiarCamara = 0;
			if (cambiarCamara == 0)
				distanceFromTarget = 15;
			else if (cambiarCamara == 1)
				distanceFromTarget = 25;

			camera->setDistanceFromTarget(distanceFromTarget);
		}
		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
			enableCamaraSelected = true;

		//Gira la camara
		if (enableGirarCamaraSelected && modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && vida > 0 && ganar == false) {
			enableGirarCamaraSelected = false;
			girarCamara = 0;
			std::cout << "Gira 1" << std::endl;

		}
		else if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) && girarCamara == 0) {
			girarCamara = 1;
			enableGirarCamaraSelected = true;
			std::cout << "Gira 2" << std::endl;
		}

		//Permite activar el nitro
		if (modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && nitro >= 100 && vida > 0 && ganar == false) {
			avanza = true;
			velocidad = 1.0;
			usarNitro = true;
			frenado1 = false;
			nitro = 0;
		}

		//Permite frenar
		if (modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && vida > 0 && ganar == false) {
			frenar = true;
		}

		//Permite girar
		if (modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && (avanza || retrocede)) {
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(-0.83827, 0.17224, 1.47627));
			modelMatrixLambo = glm::rotate(modelMatrixLambo, glm::radians(2.0f), glm::vec3(0, 1, 0));
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.83827, -0.17224, -1.47627));
		}
		else if (modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && (avanza || retrocede)) {
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.83827, 0.17224, 1.47627));
			modelMatrixLambo = glm::rotate(modelMatrixLambo, -glm::radians(2.0f), glm::vec3(0, 1, 0));
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(-0.83827, -0.17224, -1.47627));
		}

		if (modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && vida > 0 && iniciaCarrera && ganar == false) {
			avanza = true;
			if (velocidad <= 0.5) {
				frenado1 = FALSE;
				velocidad += 0.05;
			}

		}
		else if (modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && vida > 0 && iniciaCarrera && ganar == false) {
			retrocede = true;
			if (velocidad >= -0.5) {
				frenado2 = FALSE;
				velocidad -= 0.05;
			}
		}
		else {
			if (velocidad > 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
				frenado1 = TRUE;
			if (velocidad < 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
				frenado2 = TRUE;
		}

	}

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	//Se le coloca la posicion a cada coin
	for (int i = 0; i < TAMCOIN; i++)
		matrixModelCoin[i] = glm::translate(matrixModelCoin[i], glm::vec3(coinPosition[i].x, 0.0, coinPosition[i].z));

	for (int i = 0; i < TAMGEMA; i++)
	{
		matrixModelGemas[i] = glm::translate(matrixModelGemas[i], glm::vec3(gemaPosition[i].x, 0.0, gemaPosition[i].z));
	}

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(-28.12, 0, -62));
	modelMatrixLambo = glm::rotate(modelMatrixLambo, glm::radians(90.0f), glm::vec3(0, 1, 0));

	modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(-28.12, 0, -68));
	modelMatrixEclipse[3][1] = terrain.getHeightTerrain(modelMatrixEclipse[3][0], modelMatrixEclipse[3][2]);

	modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(13.0f, 0.05f, 20.95f));
	modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	//posicion para escenografia
	modelMatrixFabrica = glm::translate(modelMatrixFabrica, glm::vec3(-83.59f, 0.0f, -90.03f));
	modelMatrixFabrica = glm::rotate(modelMatrixFabrica, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixGym = glm::translate(modelMatrixGym, glm::vec3(-57.89f, 0.0f, -90.03f));
	modelMatrixGym = glm::rotate(modelMatrixGym, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixHouse = glm::translate(modelMatrixHouse, glm::vec3(-26.19f, 0.0f, -90.03f));
	modelMatrixHouse = glm::rotate(modelMatrixHouse, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixTrack1 = glm::translate(modelMatrixTrack1, glm::vec3(-33.98, 0, 7.03));
	modelMatrixTrack1 = glm::scale(modelMatrixTrack1, glm::vec3(1.5, 1.5, 1.5));

	modelMatrixAku = glm::translate(modelMatrixAku, glm::vec3(-9.92, 1, 15.7));
	modelMatrixAku = glm::rotate(modelMatrixAku, glm::radians(180.0f), glm::vec3(0, 1, 0));


	modelMatrixCamaraFinal = glm::translate(modelMatrixCamaraFinal, glm::vec3(-9.92, 0, 20.7));

	modelMatrixHarley = glm::translate(modelMatrixHarley, glm::vec3(-19.14, 0, -56.08));
	modelMatrixHarley = glm::rotate(modelMatrixHarley, glm::radians(180.0f), glm::vec3(0, 1, 0));

	modelMatrixFlash = glm::translate(modelMatrixFlash, glm::vec3(-19.14, 0, -56.08));
	modelMatrixFlash = glm::rotate(modelMatrixFlash, glm::radians(180.0f), glm::vec3(0, 1, 0));

	modelMatrixSuperman = glm::translate(modelMatrixSuperman, glm::vec3(-19.14, 0, -49.08));
	modelMatrixSuperman = glm::rotate(modelMatrixSuperman, glm::radians(180.0f), glm::vec3(0, 1, 0));

	//Semaforo
	modelMatrixSemaforo = glm::translate(modelMatrixSemaforo, posicionSemaforo);

	//posicion para Muros
	modelMatrixMuro1 = glm::translate(modelMatrixMuro1, glm::vec3(-76.0, 0, 16.8));
	modelMatrixMuro1 = glm::rotate(modelMatrixMuro1, glm::radians(90.0f), glm::vec3(0, 1, 0));

	modelMatrixMuro2 = glm::translate(modelMatrixMuro2, glm::vec3(-12.5, 0, 73.78));

	modelMatrixArco = glm::translate(modelMatrixArco, glm::vec3(arcoPosition[contadorArco].x, 0, arcoPosition[contadorArco].z));

	//Ganar / Perder
	modelMatrixGanar = glm::translate(modelMatrixGanar, glm::vec3(-16.02, 0, -64.45));

	modelMatrixPerder = glm::translate(modelMatrixPerder, glm::vec3(-28.12, 0, -64.84));

	modelMatrixFountain = glm::translate(modelMatrixFountain, glm::vec3(64.84, 0, 63.28));
	modelMatrixFountain[3][1] = terrain.getHeightTerrain(modelMatrixFountain[3][0], modelMatrixFountain[3][2]) + 0.2;
	modelMatrixFountain = glm::scale(modelMatrixFountain, glm::vec3(10.0f, 10.0f, 10.0f));

	//Topo
	modelMatrixTopito = glm::translate(modelMatrixTopito, glm::vec3(posicionTopito[contadorArco].x, 0, posicionTopito[contadorArco].z));
	modelMatrixTopito[3][1] = terrain.getHeightTerrain(modelMatrixTopito[3][0], modelMatrixTopito[3][2]) - 5.2;


	//Recorido
	modelMatrixHooh = glm::translate(modelMatrixHooh, glm::vec3(-28.12, 0, -64.84));
	modelMatrixHooh = glm::rotate(modelMatrixHooh, glm::radians(90.0f), glm::vec3(0, 1, 0));

	//Frames Hooh
	keyFramesHooh = getKeyFrames("../animaciones/animation_recorrido.txt");
	keyFramesEclipse = getKeyFrames("../animaciones/animation_recorrido_carro.txt");

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	currTimeParticlesAnimationNitro = lastTime;
	lastTimeParticlesAnimationNitro = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	//Posicion light, apuntador a la camara, distancia maximo de alcance, plano cerca y el FOV (Filed of view)
	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1, 45);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667) {
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		if (modelSelected == 0) {
			axis = glm::axis(glm::quat_cast(modelMatrixHooh));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixHooh));
			target = modelMatrixHooh[3];
		}
		else if (modelSelected == 1) {
			axis = glm::axis(glm::quat_cast(modelMatrixLambo));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixLambo));
			target = modelMatrixLambo[3];
		}
		if (creditos) {
			camera->setDistanceFromTarget(8);
			axis = glm::axis(glm::quat_cast(modelMatrixCamaraFinal));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixCamaraFinal));
			target = modelMatrixCamaraFinal[3];
		}

		if (std::isnan(angleTarget))
			angleTarget = 0.0;
		if (axis.y < 0)
			angleTarget = -angleTarget;
		if (girarCamara == 0)
			angleTarget -= glm::radians(180.0f); 
		if (girarCamara == 1)
		{
			angleTarget -= glm::radians(180.0f); 
			girarCamara = 2;
		}
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		view = camera->getViewMatrix();

		//Se actializa la caja usando las dimensiones de viewport
		shadowBox->update(screenWidth, screenHeight);
		shadowBox->getCenter();

		//Se crea matriz de forma manual la caja para la matriz de proyeccion
		glm::mat4 lightProjection = glm::mat4(1.0), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0 / shadowBox->getWidth();
		lightProjection[1][1] = 2.0 / shadowBox->getHeight();
		lightProjection[2][2] = -2.0 / shadowBox->getLength();
		lightProjection[3][3] = 1.0;
		lightView = glm::lookAt(shadowBox->getCenter(), shadowBox->getCenter() + glm::normalize(-lightPos), glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		if (vueltaCont <= 1 || creditos)
		{
			shaderSkyBox = shaderSkyboxDia;
			skyboxSphere.setShader(&shaderSkyBox);
		}
		else
		{
			shaderSkyBox = shaderSkyboxNoche;
			skyboxSphere.setShader(&shaderSkyBox);
		}

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkyBox.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderSkyBox.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
			glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
			glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vis  ta y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el Topito
		shaderParticlesTopito.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderParticlesTopito.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader para el nitro
		shaderParticlesNitro.setInt("Pass", 2);
		shaderParticlesNitro.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesNitro.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Propiedades de neblina
		*******************************************/
		if (vueltaCont <= 1 || creditos)
			SelectorDeDia = 0;
		else if (vueltaCont <= 3)
			SelectorDeDia = vueltaCont - 1;

		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(ColoresFog[SelectorDeDia])); //0.5, 0.5, 0.4)
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(ColoresFog[SelectorDeDia]));
		shaderSkyBox.setVectorFloat3("fogColor", glm::value_ptr(ColoresFog[SelectorDeDia]));

		/*******************************************
		 * Propiedades Luz direccional
		*******************************************/

		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(Ambiental[SelectorDeDia]));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(Difusa[SelectorDeDia]));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(Especular[SelectorDeDia]));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(Ambiental[SelectorDeDia]));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(Difusa[SelectorDeDia]));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(Especular[SelectorDeDia]));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		if (SelectorDeDia == 2) {
			glm::vec3 spotPositionIzquierda = glm::vec3(modelMatrixLambo * glm::vec4(1.0, 0.0, 4.0, 1.0));
			glm::vec3 spotPositionDerecha = glm::vec3(modelMatrixLambo * glm::vec4(-1.0, 0.0, 4.0, 1.0));
			glm::vec3 LuzRotacion = glm::vec3(modelMatrixLambo[2][0], modelMatrixLambo[2][1], modelMatrixLambo[2][2]);

			shaderMulLighting.setInt("spotLightCount", 2);
			shaderTerrain.setInt("spotLightCount", 2);

			shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
			shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
			shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
			shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPositionIzquierda));
			//shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -0.75, 1)));
			shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(normalize(LuzRotacion)));
			shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
			shaderMulLighting.setFloat("spotLights[0].linear", 1.0);
			shaderMulLighting.setFloat("spotLights[0].quadratic", 0.1);
			shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(30.0f)));
			shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(35.0f)));
			shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
			shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
			shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
			shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPositionIzquierda));
			shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(normalize(LuzRotacion)));
			//shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -0.75, 1)));
			shaderTerrain.setFloat("spotLights[0].constant", 1.0);
			shaderTerrain.setFloat("spotLights[0].linear", 1.0);
			shaderTerrain.setFloat("spotLights[0].quadratic", 0.1);
			shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(30.0f)));
			shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(35.0f)));

			shaderMulLighting.setVectorFloat3("spotLights[1].light.ambient", glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
			shaderMulLighting.setVectorFloat3("spotLights[1].light.diffuse", glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
			shaderMulLighting.setVectorFloat3("spotLights[1].light.specular", glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
			shaderMulLighting.setVectorFloat3("spotLights[1].position", glm::value_ptr(spotPositionDerecha));
			shaderMulLighting.setVectorFloat3("spotLights[1].direction", glm::value_ptr(normalize(LuzRotacion)));
			//shaderMulLighting.setVectorFloat3("spotLights[1].direction", glm::value_ptr(glm::vec3(0, -0.75, 1)));
			shaderMulLighting.setFloat("spotLights[1].constant", 1.0);
			shaderMulLighting.setFloat("spotLights[1].linear", 1.0);
			shaderMulLighting.setFloat("spotLights[1].quadratic", 0.1);
			shaderMulLighting.setFloat("spotLights[1].cutOff", cos(glm::radians(20.0f)));
			shaderMulLighting.setFloat("spotLights[1].outerCutOff", cos(glm::radians(25.0f)));
			shaderTerrain.setVectorFloat3("spotLights[1].light.ambient", glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
			shaderTerrain.setVectorFloat3("spotLights[1].light.diffuse", glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
			shaderTerrain.setVectorFloat3("spotLights[1].light.specular", glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
			shaderTerrain.setVectorFloat3("spotLights[1].position", glm::value_ptr(spotPositionDerecha));
			shaderTerrain.setVectorFloat3("spotLights[1].direction", glm::value_ptr(normalize(LuzRotacion)));
			//shaderMulLighting.setVectorFloat3("spotLights[1].direction", glm::value_ptr(glm::vec3(0, -0.75, 1)));
			shaderTerrain.setFloat("spotLights[1].constant", 1.0);
			shaderTerrain.setFloat("spotLights[1].linear", 1.0);
			shaderTerrain.setFloat("spotLights[1].quadratic", 0.1);
			shaderTerrain.setFloat("spotLights[1].cutOff", cos(glm::radians(30.0f)));
			shaderTerrain.setFloat("spotLights[1].outerCutOff", cos(glm::radians(35.0f)));
		}
		/*******************************************
		  * Propiedades PointLights
		  *******************************************/
		shaderMulLighting.setInt("pointLightCount", lamp1Position.size() + lamp2Orientation.size());
		shaderTerrain.setInt("pointLightCount", lamp1Position.size() + lamp2Orientation.size());
		for (int i = 0; i < lamp1Position.size(); i++) {
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp1Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0, 10.3585, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}
		for (int i = 0; i < lamp2Position.size(); i++) {
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.5, 1.5, 1.5));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
		}
		if (SelectorDeDia == 2) {
			for (int i = 0; i < 1; i++) {
				glm::mat4 matrixLuzCarro = modelMatrixLambo;
				matrixLuzCarro = glm::scale(matrixLuzCarro, glm::vec3(1.1, 1.1, 1.1));
				glm::vec3 lampPosition = glm::vec3(matrixLuzCarro[3]);
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
			}
		}
		if (Verde == false) {
			for (int i = 0; i < contRoja; i++) {
				glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
				matrixAdjustLamp = glm::translate(matrixAdjustLamp, SemaforoRojo[i]);
				matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.1, 1.1, 1.1));
				glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.5, 0.01, 0.01)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.01, 0.01)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.9, 0.01, 0.01)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.5, 0.01, 0.01)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.01, 0.01)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.01, 0.01)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
			}
		}
		if (Verde == true) {
			for (int i = 0; i < SemaforoVerde.size(); i++) {
				glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
				matrixAdjustLamp = glm::translate(matrixAdjustLamp, SemaforoVerde[i]);
				matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.1, 1.1, 1.1));
				glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.0, 1.0, 0.0)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.0, 1.0, 0.0)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.0, 1.0, 0.0)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
			}
		}
		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene(false);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		 // reset viewport
		 /*glViewport(0, 0, screenWidth, screenHeight);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 // render Depth map to quad for visual debugging
		 shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		 shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, depthMap);
		 boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		 boxViewDepth.render();*/


		 /*******************************************
		  * 2.- We render the normal objects
		  *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();


		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);

		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkyBox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		renderScene();
		/*******************************************
		 * Debug to box light box
		 *******************************************/
		 /*glm::vec3 front = glm::normalize(-lightPos);
		 glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), front));
		 glm::vec3 up = glm::normalize(glm::cross(front, right));
		 glDisable(GL_CULL_FACE);
		 glm::mat4 boxViewTransform = glm::mat4(1.0f);
		 boxViewTransform = glm::translate(boxViewTransform, centerBox);
		 boxViewTransform[0] = glm::vec4(right, 0.0);
		 boxViewTransform[1] = glm::vec4(up, 0.0);
		 boxViewTransform[2] = glm::vec4(front, 0.0);
		 boxViewTransform = glm::scale(boxViewTransform, glm::vec3(shadowBox->getWidth(), shadowBox->getHeight(), shadowBox->getLength()));
		 boxLightViewBox.enableWireMode();
		 boxLightViewBox.render(boxViewTransform);
		 glEnable(GL_CULL_FACE);*/

		 /*******************************************
		  * Creacion de colliders
		  * IMPORTANT do this before interpolations
		  *******************************************/
		  //Collider de coin
		AbstractModel::SBB coinCollider[TAMCOIN];
		for (int i = 0; i < TAMCOIN; i++) {
			if (isCollisionCoin[i] == false) {
				glm::mat4 modelMatrixColliderCoin = glm::mat4(matrixModelCoin[i]);
				modelMatrixColliderCoin[3][1] = terrain.getHeightTerrain(modelMatrixColliderCoin[3][0], modelMatrixColliderCoin[3][2]);
				modelMatrixColliderCoin = glm::scale(modelMatrixColliderCoin, glm::vec3(2.0, 2.0, 2.0));
				modelMatrixColliderCoin = glm::translate(modelMatrixColliderCoin, modelCoin[i].getSbb().c);
				coinCollider[i].c = glm::vec3(modelMatrixColliderCoin[3]);
				coinCollider[i].ratio = modelCoin[i].getSbb().ratio * 2.0;
				addOrUpdateColliders(collidersSBB, coinValor[i], coinCollider[i], matrixModelCoin[i]);
			}
			else {
				coinCollider[i].c = glm::vec3(i + 1, -100.0f, 0.0f);
				coinCollider[i].ratio = 0;
				addOrUpdateColliders(collidersSBB, coinValor[i], coinCollider[i], matrixModelCoin[i]);
			}
		}

		//Collider de gemas
		AbstractModel::SBB gemasCollider[TAMGEMA];
		for (int i = 0; i < TAMGEMA; i++) {
			if (isCollisionGema[i] == false) {
				glm::mat4 modelMatrixColliderGemas = glm::mat4(matrixModelGemas[i]);
				modelMatrixColliderGemas[3][1] = terrain.getHeightTerrain(modelMatrixColliderGemas[3][0], modelMatrixColliderGemas[3][2]) + 0.5;
				modelMatrixColliderGemas = glm::scale(modelMatrixColliderGemas, glm::vec3(0.015, 0.015, 0.015));
				modelMatrixColliderGemas = glm::translate(modelMatrixColliderGemas, modelGema[i].getSbb().c);
				gemasCollider[i].c = glm::vec3(modelMatrixColliderGemas[3]);
				gemasCollider[i].ratio = modelGema[i].getSbb().ratio * 0.015;
				addOrUpdateColliders(collidersSBB, gemaValor[i], gemasCollider[i], matrixModelGemas[i]);
			}
			else {
				gemasCollider[i].c = glm::vec3(0.0f, -100.0f, i + 1);
				gemasCollider[i].ratio = 0;
				addOrUpdateColliders(collidersSBB, gemaValor[i], gemasCollider[i], matrixModelGemas[i]);
			}
		}

		// Lamps1 colliders
		for (int i = 0; i < lamp1Position.size(); i++) {
			AbstractModel::OBB lampCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp1Position[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp1Orientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
			// Set the orientation of collider before doing the scale
			lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.0, 1.0, 1.0));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp1.getObb().c);
			lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
			lampCollider.e = modelLamp1.getObb().e * glm::vec3(1.0, 1.0, 1.0);
			std::get<0>(collidersOBB.find("lamp1-" + std::to_string(i))->second) = lampCollider;
		}

		// Lamps2 colliders
		for (int i = 0; i < lamp2Position.size(); i++) {
			AbstractModel::OBB lampCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp2Position[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp2Orientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "lamp2-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
			// Set the orientation of collider before doing the scale
			lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.5, 1.5, 1.5));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLampPost2.getObb().c);
			lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
			lampCollider.e = modelLampPost2.getObb().e * glm::vec3(1.5, 1.5, 1.5);
			std::get<0>(collidersOBB.find("lamp2-" + std::to_string(i))->second) = lampCollider;
		}

		//Collider del lambo
		//El objetivo es crear un objeto de tipo collider AbstractModel::OBB, el cual tiene las propiedades siguientes:
		// c: Centro de la caja, e: Medias dimensiones de las aristas, u: Orientacion en forma de quaternion
		AbstractModel::OBB lamboCollider;
		glm::mat4 modelMatrixColliderLambo = glm::mat4(modelMatrixLambo);
		lamboCollider.u = glm::quat_cast(modelMatrixLambo);
		modelMatrixColliderLambo = glm::scale(modelMatrixColliderLambo, glm::vec3(1.3, 1.3, 1.3));
		modelMatrixColliderLambo = glm::translate(modelMatrixColliderLambo, modelLambo.getObb().c);
		lamboCollider.c = glm::vec3(modelMatrixColliderLambo[3]);
		lamboCollider.e = modelLambo.getObb().e * glm::vec3(1.3, 1.3, 1.3); //se multiplica por el escalamiento
		//1.- Arreglo de colliders, 2.- Etiqueta, 3.- Collider que creamos, 4.- Matriz Transformacion Original
		addOrUpdateColliders(collidersOBB, "lambo", lamboCollider, modelMatrixLambo);

		AbstractModel::OBB EclipseCollider;
		glm::mat4 modelMatrixColliderEclipse = glm::mat4(modelMatrixEclipse);
		EclipseCollider.u = glm::quat_cast(modelMatrixEclipse);
		modelMatrixColliderEclipse = glm::scale(modelMatrixColliderEclipse, glm::vec3(1.3, 1.3, 1.3));
		modelMatrixColliderEclipse = glm::translate(modelMatrixColliderEclipse, modelEclipse.getObb().c);
		EclipseCollider.c = glm::vec3(modelMatrixColliderEclipse[3]);
		EclipseCollider.e = modelEclipse.getObb().e * glm::vec3(1.3, 1.3, 1.3); //se multiplica por el escalamiento
		//1.- Arreglo de colliders, 2.- Etiqueta, 3.- Collider que creamos, 4.- Matriz Transformacion Original
		addOrUpdateColliders(collidersOBB, "eclipse", EclipseCollider, modelMatrixEclipse);

		// Collider de mayow
		AbstractModel::CBB mayowCollider;
		glm::mat4 modelMatrixColliderMayow = glm::mat4(modelMatrixMayow);
		modelMatrixColliderMayow = glm::rotate(modelMatrixColliderMayow,
			glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelMatrixColliderMayow = glm::scale(modelMatrixColliderMayow, glm::vec3(0, 0, 0));
		modelMatrixColliderMayow = glm::translate(modelMatrixColliderMayow, mayowModelAnimate.getCbb().c);
		mayowCollider.c = glm::vec3(modelMatrixColliderMayow[3]);
		mayowCollider.ratio = mayowModelAnimate.getCbb().ratio * 0;
		mayowCollider.heigth = mayowModelAnimate.getCbb().heigth * 0;
		addOrUpdateColliders(collidersCBB, "mayow", mayowCollider, modelMatrixMayow);
		
		// Collider de Fabrica
		AbstractModel::OBB fabricaCollider;
		glm::mat4 modelMatrixColliderFabrica = glm::mat4(modelMatrixFabrica);
		fabricaCollider.u = glm::quat_cast(modelMatrixColliderFabrica);
		modelMatrixColliderFabrica = glm::scale(modelMatrixColliderFabrica, glm::vec3(2.0, 2.0, 2.0));
		modelMatrixColliderFabrica = glm::translate(modelMatrixColliderFabrica, fabricaModel.getObb().c);
		fabricaCollider.e = fabricaModel.getObb().e * glm::vec3(2.0, 2.0, 2.0);
		fabricaCollider.c = glm::vec3(modelMatrixColliderFabrica[3]);
		addOrUpdateColliders(collidersOBB, "fabrica", fabricaCollider, modelMatrixFabrica);

		// Collider de Gym
		AbstractModel::OBB gymCollider;
		glm::mat4 modelMatrixColliderGym = glm::mat4(modelMatrixGym);
		gymCollider.u = glm::quat_cast(modelMatrixColliderGym);
		modelMatrixColliderGym = glm::scale(modelMatrixColliderGym, glm::vec3(2.0, 2.0, 2.0));
		modelMatrixColliderGym = glm::translate(modelMatrixColliderGym, gymModel.getObb().c);
		gymCollider.e = gymModel.getObb().e * glm::vec3(2.0, 2.0, 2.0);
		gymCollider.c = glm::vec3(modelMatrixColliderGym[3]);
		addOrUpdateColliders(collidersOBB, "gym", gymCollider, modelMatrixGym);

		// Collider de House
		AbstractModel::OBB houseCollider;
		glm::mat4 modelMatrixColliderHouse = glm::mat4(modelMatrixHouse);
		houseCollider.u = glm::quat_cast(modelMatrixColliderHouse);
		modelMatrixColliderHouse = glm::scale(modelMatrixColliderHouse, glm::vec3(2.0, 2.0, 2.0));
		modelMatrixColliderHouse = glm::translate(modelMatrixColliderHouse, houseModel.getObb().c);
		houseCollider.e = houseModel.getObb().e * glm::vec3(2.0, 2.0, 2.0);
		houseCollider.c = glm::vec3(modelMatrixColliderHouse[3]);
		addOrUpdateColliders(collidersOBB, "house", houseCollider, modelMatrixHouse);

		//Collider de Harley
		AbstractModel::OBB harleyCollider;
		if (contadorArco < 1 && vueltaCont <= 1 && harley) {
			glm::mat4 modelmatrixColliderHarley = glm::mat4(modelMatrixHarley);
			// Set the orientation of collider before doing the scale
			harleyCollider.u = glm::quat_cast(modelMatrixHarley);
			modelmatrixColliderHarley = glm::scale(modelmatrixColliderHarley, glm::vec3(100.0f, 100.0f, 100.0f) *
				glm::vec3(0.015, 0.015, 0.015));
			modelmatrixColliderHarley = glm::translate(modelmatrixColliderHarley, harleyModel.getObb().c);
			harleyCollider.e = harleyModel.getObb().e * glm::vec3(100.0f, 100.0f, 100.0f) *
				glm::vec3(0.015, 0.015, 0.015);
			harleyCollider.c = glm::vec3(modelmatrixColliderHarley[3]);
			addOrUpdateColliders(collidersOBB, "harley", harleyCollider, modelMatrixHarley);
		}
		else
		{
			harleyCollider.u = glm::quat_cast(glm::mat4(1.0));
			harleyCollider.c = glm::vec3(-50.0f, -50.0f, -50);
			harleyCollider.e = glm::vec3(0.0f, 0.0f, 0.0f);
			addOrUpdateColliders(collidersOBB, "harley", harleyCollider, modelMatrixHarley);
		}
		//Collider flash
		AbstractModel::OBB flashCollider;
		if (contadorArco < 1 && vueltaCont >= 1 && harley == false && flash) {
			glm::mat4 modelMatrixColliderFlash = glm::mat4(modelMatrixFlash);
			flashCollider.u = glm::quat_cast(modelMatrixFlash);
			modelMatrixColliderFlash = glm::scale(modelMatrixColliderFlash, glm::vec3(100.0f, 100.0f, 100.0f) *
				glm::vec3(0.0125, 0.0125, 0.0125));
			modelMatrixColliderFlash = glm::translate(modelMatrixColliderFlash, flashModel.getObb().c);
			flashCollider.c = glm::vec3(modelMatrixColliderFlash[3]);
			flashCollider.e = flashModel.getObb().e * glm::vec3(100.0f, 100.0f, 100.0f) *
				glm::vec3(0.0125, 0.0125, 0.0125); //se multiplica por el escalamiento
			addOrUpdateColliders(collidersOBB, "flash", flashCollider, modelMatrixFlash);
		}
		else
		{
			flashCollider.u = glm::quat_cast(glm::mat4(1.0));
			flashCollider.c = glm::vec3(2.0f, -50.0f, 2.0f);
			flashCollider.e = glm::vec3(0.0f, 0.0f, 0);
			addOrUpdateColliders(collidersOBB, "flash", flashCollider, modelMatrixFlash);
		}

		AbstractModel::OBB supermanCollider;
		if ((contadorArco < 1 && vueltaCont >= 2 && harley == false && flash == false)) {
			glm::mat4 modelMatrixColliderSuperman = glm::mat4(modelMatrixSuperman);
			supermanCollider.u = glm::quat_cast(modelMatrixSuperman);
			modelMatrixColliderSuperman = glm::translate(modelMatrixColliderSuperman, glm::vec3(0, 0, 3.5));
			modelMatrixColliderSuperman = glm::scale(modelMatrixColliderSuperman, glm::vec3(100.0f, 100.0f, 100.0f) *
				glm::vec3(0.02, 0.015, 0.155));
			modelMatrixColliderSuperman = glm::translate(modelMatrixColliderSuperman, supermanModel.getObb().c);
			supermanCollider.c = glm::vec3(modelMatrixColliderSuperman[3]);
			supermanCollider.e = supermanModel.getObb().e * glm::vec3(100.0f, 100.0f, 100.0f) *
				glm::vec3(0.02, 0.015, 0.155); //se multiplica por el escalamiento
			addOrUpdateColliders(collidersOBB, "superman", supermanCollider, modelMatrixSuperman);
		}
		
		// Collider de muro1
		AbstractModel::OBB muro1Collider;
		glm::mat4 modelMatrixColliderMuro1 = glm::mat4(modelMatrixMuro1);
		muro1Collider.u = glm::quat_cast(modelMatrixColliderMuro1);
		modelMatrixColliderMuro1 = glm::scale(modelMatrixColliderMuro1, glm::vec3(6.0, 2.0, 1.0));
		modelMatrixColliderMuro1 = glm::translate(modelMatrixColliderMuro1, muro1Model.getObb().c);
		muro1Collider.e = muro1Model.getObb().e * glm::vec3(6.0, 2.0, 1.0);
		muro1Collider.c = glm::vec3(modelMatrixColliderMuro1[3]);
		addOrUpdateColliders(collidersOBB, "muro1", muro1Collider, modelMatrixMuro1);

		// Collider de muro2
		AbstractModel::OBB muro2Collider;
		glm::mat4 modelMatrixColliderMuro2 = glm::mat4(modelMatrixMuro2);
		muro2Collider.u = glm::quat_cast(modelMatrixColliderMuro2);
		modelMatrixColliderMuro2 = glm::scale(modelMatrixColliderMuro2, glm::vec3(2.3, 1.0, 1.0));
		modelMatrixColliderMuro2 = glm::translate(modelMatrixColliderMuro2, muro2Model.getObb().c);
		muro2Collider.e = muro2Model.getObb().e * glm::vec3(2.3, 1.0, 1.0);
		muro2Collider.c = glm::vec3(modelMatrixColliderMuro2[3]);
		addOrUpdateColliders(collidersOBB, "muro2", muro2Collider, modelMatrixMuro2);

		// Collider de Arco
		AbstractModel::OBB arcoCollider;
		glm::mat4 modelMatrixColliderArco = glm::mat4(modelMatrixArco);
		arcoCollider.u = glm::quat_cast(modelMatrixColliderArco);
		modelMatrixColliderArco = glm::scale(modelMatrixColliderArco, glm::vec3(0.75, 0.75, 0.4));
		modelMatrixColliderArco = glm::translate(modelMatrixColliderArco, arcoModel.getObb().c);
		arcoCollider.e = arcoModel.getObb().e * glm::vec3(0.75, 0.75, 0.4);
		arcoCollider.c = glm::vec3(modelMatrixColliderArco[3]);
		addOrUpdateColliders(collidersOBB, "arco", arcoCollider, modelMatrixArco);

		AbstractModel::OBB topitoCollider;
		glm::mat4 modelMatrixColliderTopito = glm::mat4(modelMatrixTopito);
		topitoCollider.u = glm::quat_cast(modelMatrixColliderTopito);
		modelMatrixColliderTopito = glm::scale(modelMatrixColliderTopito, glm::vec3(0.75, 0.85, 0.4));
		modelMatrixColliderTopito = glm::translate(modelMatrixColliderTopito, TopitoModel.getObb().c);
		modelMatrixColliderTopito = glm::translate(modelMatrixColliderTopito, glm::vec3(0.0, MovimientoenYTopito, 0.0));
		topitoCollider.e = TopitoModel.getObb().e * glm::vec3(0.75, 0.75, 0.4);
		topitoCollider.c = glm::vec3(modelMatrixColliderTopito[3]);
		addOrUpdateColliders(collidersOBB, "topo", topitoCollider, modelMatrixTopito);

		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			//boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			//sphereCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::CBB, glm::mat4, glm::mat4> >::iterator it =
			collidersCBB.begin(); it != collidersCBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio, 1, std::get<0>(it->second).ratio));
			matrixCollider = glm::scale(matrixCollider, glm::vec3(1, std::get<0>(it->second).heigth, 1));
			cylinderCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			cylinderCollider.enableWireMode();
			cylinderCollider.render(matrixCollider);
		}

		/*******************************************
		 * Test Colisions
		 *******************************************/
		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt
					&& testOBBOBB(std::get<0>(it->second),
						std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					if (it->first == "lambo" || jt->first == "lambo")
					{
						if (it->first != "arco" && jt->first != "arco")
						{
							vida -= abs(velocidad) * 10;
							velocidad = 0;
							avanza = false;
							retrocede = false;
							frenado1 = false;
							frenado2 = false;
							chocar = true;
						}
						else
						{
							detectarArco = true;
						}
					}
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt
					&& testSphereSphereIntersection(std::get<0>(it->second),
						std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second),
					std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					isCollision = true;
					for (int i = 0; i < TAMCOIN; i++)
					{
						if (it->first == coinValor[i] && jt->first == "lambo")
						{
							isCollisionCoin[i] = true;
							nitro += 10;
						}
					}
					for (int i = 0; i < TAMGEMA; i++)
					{
						if (it->first == gemaValor[i] && jt->first == "lambo")
						{
							isCollisionGema[i] = true;
							nitro += 25;
						}
					}
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::CBB, glm::mat4, glm::mat4> >::iterator it =
			collidersCBB.begin(); it != collidersCBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testCylinderOBox(std::get<0>(it->second),
					std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::CBB, glm::mat4, glm::mat4> >::iterator it =
			collidersCBB.begin(); it != collidersCBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (testCylinderSphere(std::get<0>(it->second),
					std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end();
			colIt++) {
			std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.find(colIt->first);
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.find(colIt->first);
			std::map<std::string,
				std::tuple<AbstractModel::CBB, glm::mat4, glm::mat4> >::iterator kt =
				collidersCBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("mayow") == 0)
						modelMatrixMayow = std::get<1>(jt->second);
					if (jt->first.compare("lambo") == 0)//Sirve para que no atraviese
						modelMatrixLambo = std::get<1>(jt->second);
				}
			}
			if (kt != collidersCBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersCBB, kt->first);
			}
		}
		if (record) {
			rotacionLLantas += 0.5;
			if (rotacionLLantas == 360.0) {
				rotacionLLantas = 0;
			}
		}
		/*******************************************
		 * Interpolation key frames with disconect objects
		 *******************************************/
		if (keyFramesHooh.size() > 0 && ActivaRecorrido) {
			// Para reproducir el frame
			interpolationHooh = numPasosHooh / (float)maxNumPasosHooh;
			numPasosHooh++;
			if (interpolationHooh > 1.0) {
				numPasosHooh = 0;
				interpolationHooh = 0;
				indexFrameHooh = indexFrameHoohNext;
				indexFrameHoohNext++;
			}
			if (indexFrameHoohNext > keyFramesHooh.size() - 1) {
				indexFrameHoohNext = 0;
				std::cout << "keyFramesHooh.size(): " << keyFramesHooh.size() << std::endl;
				modelSelected = 1;
				record = true;
				ActivaRecorrido = false;
			}
			modelMatrixHooh = interpolate(keyFramesHooh, indexFrameHooh, indexFrameHoohNext, 0, interpolationHooh);
		}

		if (keyFramesEclipse.size() > 0 && record) {
			// Para reproducir el frame
			interpolationEclipse = numPasosEclipse / (float)maxNumPasosEclipse;
			numPasosEclipse++;
			if (interpolationEclipse > 1.0) {
				numPasosEclipse = 0;
				interpolationEclipse = 0;
				indexFrameEclipse = indexFrameEclipseNext;
				indexFrameEclipseNext++;
			}
			if (indexFrameEclipseNext > keyFramesEclipse.size() - 1)
				indexFrameEclipseNext = 0;
			modelMatrixEclipse = interpolate(keyFramesEclipse, indexFrameEclipse, indexFrameEclipseNext, 0, interpolationEclipse);
		}

		// Constantes de animaciones
		animationIndex = 1;
		rotCoin += 0.1;

		//Nitro del auto
		if (contadorNitro <= 2 && usarNitro == true && frenar == false) {
			avanza = true;
			frenado1 = false;
			contadorNitro += 0.05;
			velocidad = 1.0;
		}
		else if (usarNitro == true) {
			usarNitro = false;
			contadorNitro = 0;
			frenado1 = true;
		}

		//Decrementar la velocidad del auto
		if (velocidad > 0.01 && frenado1 && usarNitro == false)
			velocidad -= 0.01;
		else if (frenado1) {
			velocidad = 0;
			frenado1 = false;
			avanza = false;
		}
		if (velocidad < -0.01 && frenado2) {
			velocidad += 0.01;
		}
		else if (frenado2) {
			velocidad = 0;
			frenado2 = false;
			retrocede = false;
		}

		if (velocidad > 0.1 && frenar)
			velocidad -= 0.1;
		else if (velocidad < -0.1 && frenar) {
			velocidad += 0.1;
		}
		else if (frenar) {
			velocidad = 0;
			avanza = false;
			retrocede = false;
			frenar = false;
		}

		/*******************************************
		 * State machines
		 *******************************************/
		if (creditos == false && (ganar || vida <= 0)) {
			contCreditos += 1;
			if (contCreditos == 100)
				creditos = true;
		}

		// State machine for the lambo car
		if (creditos)
		{
			switch (stateAku) {
			case 0:
				moverAku += 0.01;
				if (moverAku > 1)
					stateAku = 1;
				break;
			case 1:
				moverAku -= 0.01;
				if (moverAku < -1) {
					stateAku = 0;
				}
				break;
			}
		}

		/****************************
			Topito
		*****************************/
		if (ActivaTopito == 1) {
			if (subetopito == true) {
				MovimientoenYTopito += 0.05;
				if (MovimientoenYTopito >= 3.5f) {
					subetopito = false;
				}
			}
			else if (subetopito == false) {
				MovimientoenYTopito -= 0.05;
				if (MovimientoenYTopito <= 0.0f) {
					subetopito = true;
					ActivaTopito = 2;
				}
			}
			if (ActivaTopito == 2) {
				ActivaTopito = 0;
			}
		}

		if (contLetras <= 1)
		{
			colorLetras = glm::vec3(0.5, 0.5, 0.0);
			contLetras += 0.1;
		}
		else if (contLetras <= 2)
		{
			colorLetras = glm::vec3(0.0, 0.0, 0.0);
			contLetras += 0.1;
		}
		else
			contLetras = 0;

		//Freetype

		if (presionaTecla && creditos == false && modelSelected == 1)
		{
			std::string mostrarCont = "Time: 00:00";
			std::string mostrarVuelta = "Vuelta: " + std::to_string(vueltaCont) + "/3";
			std::string mostrarVida = "";
			std::string mostrarNitro = "";

			if (ganar == false) {

				/*****************
				Control Semaforo
				******************/
				if (iniciaCarrera == false) {
					tiempoSemaforo += 1;
					if (tiempoSemaforo == 25) {
						contRoja += 1;
						tiempoSemaforoAcumulado += 1;
						tiempoSemaforo = 0;
						if (contRoja == 4) {
							contRoja = 3;
							Verde = true;

						}
					}
					if (tiempoSemaforoAcumulado == 5) {
						tiempoSemaforo = 0;
						contRoja = 0;
						Verde = false;
						tiempoSemaforoAcumulado = 0;
						RenderSemaforo = false;
						iniciaCarrera = true;
						timecount = 1;
						segundos = 20;
					}
				}
				else if (segundos >= 0)
				{
					if (vida > 0)
						timecount -= 1;

					if (timecount == 0 && vida > 0)
					{
						timecount = 59;
						segundos -= 1;
					}

					mostrarCont = "Time: 00:";
					if (segundos < 10)
						mostrarCont += "0" + std::to_string(segundos) + ":";
					else
						mostrarCont += std::to_string(segundos) + ":";

					if (timecount < 10)
						mostrarCont += "0" + std::to_string(timecount);
					else
						mostrarCont += std::to_string(timecount);
				}
				else {
					vida = 0;
					std::cout << "Vida: " << vida << ", segundos: " << segundos << ", milisegundos: " << timecount << std::endl;
					mostrarCont = "Time: 00:00:00";
				}
			}
			else
				mostrarVuelta = "Vuelta: 3/3";

			if (vida >= 99.9)
				mostrarVida = "**********";
			else if (vida >= 89.9)
				mostrarVida = "*********";
			else if (vida >= 79.9)
				mostrarVida = "********";
			else if (vida >= 69.9)
				mostrarVida = "*******";
			else if (vida >= 59.9)
				mostrarVida = "******";
			else if (vida >= 49.9)
				mostrarVida = "*****";
			else if (vida >= 39.9)
				mostrarVida = "****";
			else if (vida >= 29.9)
				mostrarVida = "***";
			else if (vida >= 19.9)
				mostrarVida = "**";
			else if (vida >= 9.9)
				mostrarVida = "*";
			else if (vida <= 0)
				mostrarVida = "";

			if (nitro > 19 && nitro < 39)
				mostrarNitro = "\n";
			else if (nitro > 39 && nitro < 59)
				mostrarNitro = "\n\n";
			else if (nitro > 59 && nitro < 79)
				mostrarNitro = "\n\n\n";
			else if (nitro > 79 && nitro < 99)
				mostrarNitro = "\n\n\n\n";
			else if (nitro > 99)
				mostrarNitro = "\n\n\n\n\n";

			RenderText(shaderText, mostrarCont, 620.0f, 640.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(shaderText, mostrarVuelta, 620.0f, 670.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

			RenderText(shaderText, mostrarVida, 25.0f, 650.0f, 1.0f, glm::vec3((100 - vida)*0.01, vida*0.01, 0.0));

			RenderText(shaderText, "[", 10.0f, 620.0f, 1.0f, glm::vec3(0.8f, 0.0f, 0.8f)); // 100
			RenderText(shaderText, "]", 204.0f, 620.0f, 1.0f, glm::vec3(0.8f, 0.0f, 0.8f)); //294
			RenderText(shaderText, mostrarNitro, 25.0f, 620.0f, 1.0f, glm::vec3(0.8f, 0.0f, 0.8f)); //110
		}
		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		if (presionaTecla)
		{
			source0Pos[0] = modelMatrixFountain[3].x;
			source0Pos[1] = modelMatrixFountain[3].y;
			source0Pos[2] = modelMatrixFountain[3].z;
			alSourcefv(source[0], AL_POSITION, source0Pos);

			source2Pos[0] = modelMatrixLambo[3].x;
			source2Pos[1] = modelMatrixLambo[3].y;
			source2Pos[2] = modelMatrixLambo[3].z;
			alSourcefv(source[2], AL_POSITION, source2Pos);

			//Sonido para el perdedor
			source3Pos[0] = modelMatrixPerder[3].x;
			source3Pos[1] = modelMatrixPerder[3].y;
			source3Pos[2] = modelMatrixPerder[3].z;
			alSourcefv(source[3], AL_POSITION, source3Pos);

			//Sonido para el ganador
			source4Pos[0] = modelMatrixGanar[3].x;
			source4Pos[1] = modelMatrixGanar[3].y;
			source4Pos[2] = modelMatrixGanar[3].z;
			alSourcefv(source[4], AL_POSITION, source4Pos);

			//Sonido para creditos
			source5Pos[0] = modelMatrixAku[3].x;
			source5Pos[1] = modelMatrixAku[3].y;
			source5Pos[2] = modelMatrixAku[3].z;
			alSourcefv(source[5], AL_POSITION, source5Pos);

			//Sonido para frenar
			source6Pos[0] = modelMatrixLambo[3].x;
			source6Pos[1] = modelMatrixLambo[3].y;
			source6Pos[2] = modelMatrixLambo[3].z;
			alSourcefv(source[6], AL_POSITION, source6Pos);

			//Sonido para coin
			for (int i = 0; i < TAMCOIN; i++)
			{
				source7Pos[i][0] = matrixModelCoin[i][3].x;
				source7Pos[i][1] = matrixModelCoin[i][3].y;
				source7Pos[i][2] = matrixModelCoin[i][3].z;
				alSourcefv(source[7 + i], AL_POSITION, source7Pos[i]);
			}

			//Sonido para gema
			for (int i = 0; i < TAMGEMA; i++)
			{
				source8Pos[i][0] = matrixModelCoin[i][3].x;
				source8Pos[i][1] = matrixModelCoin[i][3].y;
				source8Pos[i][2] = matrixModelCoin[i][3].z;
				alSourcefv(source[23 + i], AL_POSITION, source8Pos[i]);
			}


			// Listener for the Thris person camera
			listenerPos[0] = modelMatrixLambo[3].x;
			listenerPos[1] = modelMatrixLambo[3].y;
			listenerPos[2] = modelMatrixLambo[3].z;
			alListenerfv(AL_POSITION, listenerPos);

			glm::vec3 upModel = glm::normalize(modelMatrixLambo[1]);
			glm::vec3 frontModel = glm::normalize(modelMatrixLambo[2]);

			//En tercera persona 
			listenerOri[0] = frontModel.x;
			listenerOri[1] = frontModel.y;
			listenerOri[2] = frontModel.z;
			listenerOri[3] = upModel.x;
			listenerOri[4] = upModel.y;
			listenerOri[5] = upModel.z;

			alListenerfv(AL_ORIENTATION, listenerOri);

			//Indica como reproducir el sonido
			for (unsigned int i = 0; i < 2; i++) {
				if (sourcesPlay[i]) {
					sourcesPlay[i] = false;
					alSourcePlay(source[i]);
				}
			}

			if ((avanza || retrocede) && sourcesPlay[2])
			{
				sourcesPlay[2] = false;
				alSourcePlay(source[2]);
			}
			else if (avanza == false && retrocede == false) {
				sourcesPlay[2] = true;
				alSourceStop(source[2]);
			}

			if (vida <= 0 && sourcesPlay[3])
			{
				sourcesPlay[3] = false;
				alSourcePlay(source[3]);
			}
			if (ganar && sourcesPlay[4])
			{
				sourcesPlay[4] = false;
				alSourcePlay(source[4]);
			}

			if (chocar && sourcesPlay[6])
			{
				sourcesPlay[6] = false;
				alSourcePlay(source[6]);
			}
			else {
				sourcesPlay[6] = true;
				chocar = false;
			}

			for (int i = 0; i < TAMCOIN; i++)
				if (isCollisionCoin[i] && sourcesPlayCoin[i])
				{
					sourcesPlayCoin[i] = false;
					alSourcePlay(source[7 + i]);
				}

			for (int i = 0; i < TAMGEMA; i++)
				if (isCollisionGema[i] && sourcesPlayGema[i])
				{
					sourcesPlayGema[i] = false;
					alSourcePlay(source[23 + i]);
				}
		}
		if (creditos)
		{

			for (unsigned int i = 0; i < 2; i++)
				if (sourcesPlay[i] == false)
					alSourceStop(source[i]);


			if (sourcesPlay[5])
			{
				sourcesPlay[5] = false;
				alSourcePlay(source[5]);
			}
		}
	}


}

void prepareScene() {

	skyboxSphere.setShader(&shaderSkyBox);

	terrain.setShader(&shaderTerrain);

	// Lamborginhi
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	modelEclipse.setShader(&shaderMulLighting);

	//Lamp models
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.setShader(&shaderMulLighting);

	//Coin models
	for (int i = 0; i < TAMCOIN; i++)
	{
		modelCoin[i].setShader(&shaderMulLighting);
	}

	//Gema models
	for (int i = 0; i < TAMGEMA; i++)
	{
		modelGema[i].setShader(&shaderMulLighting);
	}

	//Grass
	modelGrass.setShader(&shaderMulLighting);

	//Mayow
	mayowModelAnimate.setShader(&shaderMulLighting);

	//Fabrica
	fabricaModel.setShader(&shaderMulLighting);
	//Gym
	gymModel.setShader(&shaderMulLighting);
	//House
	houseModel.setShader(&shaderMulLighting);
	//Aku
	akuModel.setShader(&shaderMulLighting);
	//Track1
	track1Model.setShader(&shaderMulLighting);

	//Harley
	harleyModel.setShader(&shaderMulLighting);

	//Flash
	flashModel.setShader(&shaderMulLighting);

	//Superman
	supermanModel.setShader(&shaderMulLighting);

	muro1Model.setShader(&shaderMulLighting);
	muro2Model.setShader(&shaderMulLighting);

	//Arco
	arcoModel.setShader(&shaderMulLighting);

	//Ganar Perder
	ganarModel.setShader(&shaderMulLighting);
	perderModel.setShader(&shaderMulLighting);
	//Topo
	TopitoModel.setShader(&shaderMulLighting);

	HoohModel.setShader(&shaderMulLighting);
}

void prepareDepthScene() {

	skyboxSphere.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	// Lamborginhi
	modelLambo.setShader(&shaderDepth);
	modelLamboLeftDor.setShader(&shaderDepth);
	modelLamboRightDor.setShader(&shaderDepth);
	modelLamboFrontLeftWheel.setShader(&shaderDepth);
	modelLamboFrontRightWheel.setShader(&shaderDepth);
	modelLamboRearLeftWheel.setShader(&shaderDepth);
	modelLamboRearRightWheel.setShader(&shaderDepth);

	modelEclipse.setShader(&shaderDepth);

	//Coins
	for (int i = 0; i < TAMCOIN; i++)
	{
		modelCoin[i].setShader(&shaderDepth);
	}

	//Gema
	for (int i = 0; i < TAMGEMA; i++)
	{
		modelGema[i].setShader(&shaderDepth);
	}

	//Lamp models
	modelLamp1.setShader(&shaderDepth);
	modelLamp2.setShader(&shaderDepth);
	modelLampPost2.setShader(&shaderDepth);

	//Grass
	modelGrass.setShader(&shaderDepth);

	//Mayow
	mayowModelAnimate.setShader(&shaderDepth);

	//Fabrica
	fabricaModel.setShader(&shaderDepth);

	//Gym
	gymModel.setShader(&shaderDepth);

	//Aku
	akuModel.setShader(&shaderDepth);

	//House
	houseModel.setShader(&shaderDepth);

	//Track1
	track1Model.setShader(&shaderDepth);

	//harley
	harleyModel.setShader(&shaderDepth);

	//flash
	flashModel.setShader(&shaderDepth);

	//Superman
	supermanModel.setShader(&shaderDepth);

	//Muros
	muro1Model.setShader(&shaderDepth);
	muro2Model.setShader(&shaderDepth);

	//Arco
	arcoModel.setShader(&shaderDepth);

	//Ganar / Perder
	ganarModel.setShader(&shaderDepth);
	perderModel.setShader(&shaderDepth);

	//Obstaculos
	TopitoModel.setShader(&shaderDepth);

	//Hooh
	HoohModel.setShader(&shaderDepth);
}

void renderScene(bool renderParticles) {

	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	glm::mat4 modelCesped = glm::mat4(1.0);
	modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
	modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
	// Se activa la textura del background
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de tierra
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de hierba
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura del camino
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	// Se activa la textura del blend map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));

	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/
	
	 // Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	// Render the lamps
	for (int i = 0; i < lamp1Position.size(); i++) {
		lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x, lamp1Position[i].z);
		modelLamp1.setPosition(lamp1Position[i]);
		modelLamp1.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
		modelLamp1.render();
	}

	for (int i = 0; i < lamp2Position.size(); i++) {
		lamp2Position[i].y = terrain.getHeightTerrain(lamp2Position[i].x, lamp2Position[i].z);
		modelLamp2.setPosition(lamp2Position[i]);
		modelLamp2.setScale(glm::vec3(1.5, 1.5, 1.5));
		modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		modelLamp2.render();
		modelLampPost2.setPosition(lamp2Position[i]);
		modelLampPost2.setScale(glm::vec3(1.5, 1.5, 1.5));
		modelLampPost2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		modelLampPost2.render();
	}

	//Coin render
	for (int i = 0; i < TAMCOIN; i++) {
		glm::mat4 matrixModelCoinBody = glm::mat4(matrixModelCoin[i]);
		matrixModelCoinBody[3][1] = terrain.getHeightTerrain(matrixModelCoinBody[3][0], matrixModelCoinBody[3][2]);
		matrixModelCoinBody = glm::scale(matrixModelCoinBody, glm::vec3(2.0, 2.0, 2.0));
		matrixModelCoinBody = glm::rotate(matrixModelCoinBody, rotCoin, glm::vec3(0, 1, 0));
		if (isCollisionCoin[i] == false)
		{
			modelCoin[i].render(matrixModelCoinBody);
		}
	}

	//Gema render
	for (int i = 0; i < TAMGEMA; i++) {
		glm::mat4 matrixModelGemasBody = glm::mat4(matrixModelGemas[i]);
		matrixModelGemasBody[3][1] = terrain.getHeightTerrain(matrixModelGemasBody[3][0], matrixModelGemasBody[3][2]) + 0.5;
		matrixModelGemasBody = glm::scale(matrixModelGemasBody, glm::vec3(0.015, 0.015, 0.015));
		matrixModelGemasBody = glm::rotate(matrixModelGemasBody, rotCoin, glm::vec3(0, 1, 0));
		if (isCollisionGema[i] == false)
		{
			modelGema[i].render(matrixModelGemasBody);
		}
	}

	// Grass
	glDisable(GL_CULL_FACE);
	glm::vec3 grassPosition = glm::vec3(12.5, 0, 34.38);
	grassPosition.y = terrain.getHeightTerrain(grassPosition.x, grassPosition.z);
	modelGrass.setPosition(grassPosition);
	modelGrass.render();
	glEnable(GL_CULL_FACE);

	// Fountain
	glDisable(GL_CULL_FACE);
	modelFountain.render(modelMatrixFountain);
	glEnable(GL_CULL_FACE);



	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/
	//Renderizado de escenografia
	modelMatrixFabrica[3][1] = terrain.getHeightTerrain(modelMatrixFabrica[3][0], modelMatrixFabrica[3][2]);
	glm::mat4 modelMatrixFabricaBody = glm::mat4(modelMatrixFabrica);
	modelMatrixFabricaBody = glm::scale(modelMatrixFabricaBody, glm::vec3(2.0, 2.0, 2.0));
	fabricaModel.render(modelMatrixFabricaBody);

	modelMatrixGym[3][1] = terrain.getHeightTerrain(modelMatrixGym[3][0], modelMatrixGym[3][2]);
	glm::mat4 modelMatrixGymBody = glm::mat4(modelMatrixGym);
	modelMatrixGymBody = glm::scale(modelMatrixGymBody, glm::vec3(2.0, 2.0, 2.0));
	gymModel.render(modelMatrixGymBody);

	modelMatrixHouse[3][1] = terrain.getHeightTerrain(modelMatrixHouse[3][0], modelMatrixHouse[3][2]);
	glm::mat4 modelMatrixHouseBody = glm::mat4(modelMatrixHouse);
	modelMatrixHouseBody = glm::scale(modelMatrixHouseBody, glm::vec3(2.0, 2.0, 2.0));
	houseModel.render(modelMatrixHouseBody);

	modelMatrixTrack1[3][1] = terrain.getHeightTerrain(modelMatrixTrack1[3][0], modelMatrixTrack1[3][2]);
	glm::mat4 modelMatrixTrack1Body = glm::mat4(modelMatrixTrack1);
	track1Model.render(modelMatrixTrack1Body);

	/* Custom Anim objects obj Harley */
	if (contadorArco < 1 && vueltaCont <= 1 && harley)
	{
		modelMatrixHarley[3][1] = terrain.getHeightTerrain(modelMatrixHarley[3][0], modelMatrixHarley[3][2]);
		glm::mat4 modelMatrixHarleyBody = glm::mat4(modelMatrixHarley);
		modelMatrixHarleyBody = glm::scale(modelMatrixHarleyBody, glm::vec3(0.015, 0.015, 0.015));
		harleyModel.render(modelMatrixHarleyBody);
	}

	/* Custom Anim objects obj Flash */
	if (contadorArco < 1 && vueltaCont >= 1 && harley == false && flash) {
		modelMatrixFlash[3][1] = terrain.getHeightTerrain(modelMatrixFlash[3][0], modelMatrixFlash[3][2]);
		glm::mat4 modelMatrixFlashBody = glm::mat4(modelMatrixFlash);
		modelMatrixFlashBody = glm::scale(modelMatrixFlashBody, glm::vec3(0.015, 0.015, 0.015));
		flashModel.setAnimationIndex(1);
		flashModel.render(modelMatrixFlashBody);
	}
	if (contadorArco < 1 && vueltaCont >= 2 && harley == false && flash == false) {
		modelMatrixSuperman[3][1] = terrain.getHeightTerrain(modelMatrixSuperman[3][0], modelMatrixSuperman[3][2]);
		glm::mat4 modelMatrixSupermanBody = glm::mat4(modelMatrixSuperman);
		modelMatrixSupermanBody = glm::scale(modelMatrixSupermanBody, glm::vec3(0.015, 0.015, 0.015));
		supermanModel.setAnimationIndex(0);
		supermanModel.render(modelMatrixSupermanBody);
	}

	//Hooh	
	if (ActivaRecorrido)
	{
		modelMatrixHooh[3][1] = terrain.getHeightTerrain(modelMatrixHooh[3][0], modelMatrixHooh[3][2]) + 10.0f;
		glm::mat4 modelMatrixHoohBody = glm::mat4(modelMatrixHooh);
		modelMatrixHoohBody = glm::scale(modelMatrixHoohBody, glm::vec3(0.021, 0.021, 0.021));
		HoohModel.setAnimationIndex(0);
		HoohModel.render(modelMatrixHoohBody);
	}

	//Eclipse
	modelMatrixEclipse[3][1] = terrain.getHeightTerrain(modelMatrixEclipse[3][0], modelMatrixEclipse[3][2]);
	glm::mat4 modelMatrixEclipseCarro = glm::mat4(modelMatrixEclipse);
	modelMatrixEclipseCarro = glm::scale(modelMatrixEclipseCarro, glm::vec3(1.3, 1.3, 1.3));
	modelEclipse.render(modelMatrixEclipseCarro);

	glm::mat4 modelMatrixLLDelanteras = glm::mat4(modelMatrixEclipseCarro);
	modelMatrixLLDelanteras = glm::translate(modelMatrixLLDelanteras, glm::vec3(1.4629, 0.3699, 0.0));
	modelMatrixLLDelanteras = glm::rotate(modelMatrixLLDelanteras, -glm::radians(rotacionLLantas), glm::vec3(0, 0, 1));
	modelMatrixLLDelanteras = glm::translate(modelMatrixLLDelanteras, glm::vec3(-1.4629, -0.3699, 0.0));
	modelEclipseLLDelanteras.render(modelMatrixLLDelanteras);

	glm::mat4 modelMatrixLLTraseras = glm::mat4(modelMatrixEclipseCarro);
	modelMatrixLLTraseras = glm::translate(modelMatrixLLTraseras, glm::vec3(-1.5692, 0.3699, 0.0));
	modelMatrixLLTraseras = glm::rotate(modelMatrixLLTraseras, -glm::radians(rotacionLLantas), glm::vec3(0, 0, 1));
	modelMatrixLLTraseras = glm::translate(modelMatrixLLTraseras, glm::vec3(1.5692, -0.3699, 0.0));
	modelEclipseLLTraseras.render(modelMatrixLLTraseras);

	//Renderizado de muros

	modelMatrixMuro1[3][1] = terrain.getHeightTerrain(modelMatrixMuro1[3][0], modelMatrixMuro1[3][2]);
	glm::mat4 modelMatrixMuro1Body = glm::mat4(modelMatrixMuro1);
	modelMatrixMuro1Body = glm::scale(modelMatrixMuro1Body, glm::vec3(6.0, 2.0, 1.0));
	muro1Model.render(modelMatrixMuro1Body);

	modelMatrixMuro2[3][1] = terrain.getHeightTerrain(modelMatrixMuro2[3][0], modelMatrixMuro2[3][2]);
	glm::mat4 modelMatrixMuro2Body = glm::mat4(modelMatrixMuro2);
	modelMatrixMuro2Body = glm::scale(modelMatrixMuro2Body, glm::vec3(2.3, 1.0, 1.0));
	muro2Model.render(modelMatrixMuro2Body);

	//Ganar / perder
	if (ganar)
	{
		modelMatrixGanar[3][1] = terrain.getHeightTerrain(modelMatrixGanar[3][0], modelMatrixGanar[3][2]) + 3;
		glm::mat4 modelMatrixGanarBody = glm::mat4(modelMatrixLambo);
		modelMatrixGanarBody = glm::translate(modelMatrixGanarBody, glm::vec3(0, 2, 4));
		modelMatrixGanarBody = glm::rotate(modelMatrixGanarBody, glm::radians(180.0f), glm::vec3(0, 1, 0));
		modelMatrixGanarBody = glm::scale(modelMatrixGanarBody, glm::vec3(3.0, 3.0, 3.0));
		ganarModel.render(modelMatrixGanarBody);
	}

	if (vida <= 0)
	{
		glm::mat4 modelMatrixPerderBody = glm::mat4(modelMatrixLambo);
		modelMatrixPerderBody = glm::translate(modelMatrixPerderBody, glm::vec3(0, 2, 2));
		modelMatrixPerderBody = glm::rotate(modelMatrixPerderBody, glm::radians(180.0f), glm::vec3(0, 1, 0));
		modelMatrixPerderBody = glm::scale(modelMatrixPerderBody, glm::vec3(3.0, 3.0, 3.0));
		perderModel.render(modelMatrixPerderBody);
	}

	if (creditos) {
		glm::mat4 modelMatrixAkuBody = glm::mat4(modelMatrixAku);
		modelMatrixAkuBody = glm::translate(modelMatrixAkuBody, glm::vec3(moverAku, 0, 0));
		modelMatrixAkuBody = glm::scale(modelMatrixAkuBody, glm::vec3(0.3, 0.3, 0.3));
		akuModel.render(modelMatrixAkuBody);
	}

	//Semaforo
	if (RenderSemaforo)
		semaforoModel.render(modelMatrixSemaforo);

	//Arco
	if (detectarArco == false) {
		modelMatrixArco[3][1] = terrain.getHeightTerrain(modelMatrixArco[3][0], modelMatrixArco[3][2]);
		glm::mat4 modelMatrixArcoBody = glm::mat4(modelMatrixArco);
		modelMatrixArcoBody = glm::scale(modelMatrixArcoBody, glm::vec3(1.0f, 0.7f, 1.0f));
		arcoModel.render(modelMatrixArcoBody);

		glm::mat4 modelMatrixTopitoBody = glm::mat4(modelMatrixTopito);
		modelMatrixTopitoBody = glm::translate(modelMatrixTopitoBody, glm::vec3(0, MovimientoenYTopito, 0));
		TopitoModel.render(modelMatrixTopitoBody);

	}
	else {
		contadorArco += 1;
		segundos += 5;

		if (contadorArco == 2 || contadorArco == 3 || contadorArco == 4)
			modelMatrixArco = glm::rotate(modelMatrixArco, glm::radians(-90.0f), glm::vec3(0, 1, 0));

		contadorArco -= 1;

		modelMatrixArco = glm::translate(modelMatrixArco, glm::vec3(-arcoPosition[contadorArco].x, 0, -arcoPosition[contadorArco].z));
		modelMatrixTopito = glm::translate(modelMatrixTopito, glm::vec3(-posicionTopito[contadorArco].x, 0, -posicionTopito[contadorArco].z));

		contadorArco += 1;
		if (contadorArco > 4) {
			contadorArco = 0;
			vuelta = false;
			harley = false;
			if (vueltaCont == 2)
				flash = false;
		}
		modelMatrixArco = glm::translate(modelMatrixArco, glm::vec3(arcoPosition[contadorArco].x, 0, arcoPosition[contadorArco].z));
		modelMatrixTopito = glm::translate(modelMatrixTopito, glm::vec3(posicionTopito[contadorArco].x, 0, posicionTopito[contadorArco].z));

		if (contadorArco == 1 || contadorArco == 2 || contadorArco == 3)
			modelMatrixArco = glm::rotate(modelMatrixArco, glm::radians(90.0f), glm::vec3(0, 1, 0));

		detectarArco = false;
	}
	/**********
	 * Update the position with alpha objects
	 */
	 // Update the lambo
	blendingUnsorted.find("lambo")->second = glm::vec3(modelMatrixLambo[3]);
	
	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++) {
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	if (presionaTecla == false)
	{

		boxInicio.setPosition(glm::vec3(modelMatrixLambo[3].x - 10, modelMatrixLambo[3].y + 4, modelMatrixLambo[3].z));
		boxInicio.setOrientation(glm::vec3(0, 0, -30));
		boxInicio.setScale(glm::vec3(0.005, 4.5, 10));
		boxInicio.render();

		RenderText(shaderText, "Ingresa una moneda", 50.0f, 410.0f, 1.5f, colorLetras);
	}

	if (creditos)
	{

		boxFinal.setPosition(glm::vec3(-9.92, 8, 20.7));
		boxFinal.setScale(glm::vec3(15, 15, 15));
		boxFinal.render();

		RenderText(shaderText, "Universidad Nacional Autonoma de Mexico", 150.0f, 610.0f, 0.5f, glm::vec3(0.957, 0.467, 0.635));
		RenderText(shaderText, "Facultad de Ingenieria", 150.0f, 580.0f, 0.5f, glm::vec3(0.957, 0.467, 0.635));
		RenderText(shaderText, "Division de Ingenieria Electrica", 150.0f, 550.0f, 0.5f, glm::vec3(0.957, 0.467, 0.635));
		RenderText(shaderText, "Computacion Grafica Avanzada", 150.0f, 520.0f, 0.5f, glm::vec3(0.957, 0.467, 0.635));
		RenderText(shaderText, "Asesor: Avila Martell Reynaldo", 150.0f, 400.0f, 0.5f, glm::vec3(0.643, 0.521, 0.945));
		RenderText(shaderText, "Creadores:", 150.0f, 280.0f, 0.5f, glm::vec3(0.0784, 0.851, 0.816));
		RenderText(shaderText, "Cuevas Salgado Carlos", 250.0f, 250.0f, 0.5f, glm::vec3(0.0784, 0.851, 0.816));
		RenderText(shaderText, "Galindo Chavez Lisset America", 250.0f, 220.0f, 0.5f, glm::vec3(0.0784, 0.851, 0.816));

	}

	for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {
		if (it->second.first.compare("lambo") == 0) {
			// Lambo car

			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0, 0.0, 0.738135));
			glm::vec3 normal1 = glm::normalize(terrain.getNormalTerrain(modelMatrixLambo[3][0], modelMatrixLambo[3][2]));
			glm::vec3 axisxtem = glm::vec3(modelMatrixLambo[0]);
			glm::vec3 axisz = glm::normalize(glm::cross(axisxtem, normal1));
			glm::vec3 axisx = glm::normalize(glm::cross(normal1, axisz));
			glm::vec3 pospivote1 = glm::vec3(modelMatrixLambo[3]);
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0, 0.0, -0.738135));

			modelMatrixLambo[3][1] = terrain.getHeightTerrain(modelMatrixLambo[3][0], modelMatrixLambo[3][2]);
			float region = terrainLogic.getHeightTerrain(modelMatrixLambo[3][0], modelMatrixLambo[3][2]);
			float terrenoInicio = terrainLogicInicio.getHeightTerrain(modelMatrixLambo[3][0], modelMatrixLambo[3][2]);

			modelMatrixLambo[2] = glm::vec4(axisz, 0.0);
			modelMatrixLambo[1] = glm::vec4(normal1, 0.0);

			//Izquierda enfrente
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(-0.83827, 0.17224, 1.47627));
			float region2 = terrainLogic.getHeightTerrain(modelMatrixLambo[3][0], modelMatrixLambo[3][2]);
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.83827, -0.17224, -1.47627));

			//Derecha enfrente
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.83827, 0.17224, 1.47627));
			float region3 = terrainLogic.getHeightTerrain(modelMatrixLambo[3][0], modelMatrixLambo[3][2]);
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(-0.83827, -0.17224, -1.47627));

			//Izquierda Atras
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(-0.83827, 0.17224, -1.47627));
			float region4 = terrainLogic.getHeightTerrain(modelMatrixLambo[3][0], modelMatrixLambo[3][2]);
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.83827, -0.17224, 1.47627));

			//Derecha atras
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.83827, 0.17224, -1.47627));
			float region5 = terrainLogic.getHeightTerrain(modelMatrixLambo[3][0], modelMatrixLambo[3][2]);
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(-0.83827, 0.17224, 1.47627));



			if (region <= 0 || region2 <= 0 || region3 <= 0 || region4 <= 0 || region5 <= 0)
			{

				if (velocidad > 0 && deteccionAtras == false)
				{
					deteccionEnfrente = true;
					velocidad = 0;
					retrocede = true;
					velocidad -= 0.01;
				}
				if (velocidad < 0 && deteccionEnfrente == false)
				{
					deteccionAtras = true;
					velocidad = 0;
					avanza = true;
					velocidad += 0.01;
				}

			}
			else
			{
				deteccionEnfrente = false;
				deteccionAtras = false;
			}

			if (region > 0.9 && region < 10)
			{
				vida -= region * 0.02;
				std::cout << "region: " << region << ", vida: " << vida << std::endl;
			}

			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0, 0.17224, 1.47627));
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0, 0, velocidad));
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0, -0.17224, -1.47627));

			if (terrenoInicio > 0 && vuelta == false) {
				vuelta = true;
				vueltaCont += 1;
				if (vueltaCont > 1)
					segundos += 5;

				for (int i = 0; i < TAMGEMA; i++) {
					isCollisionGema[i] = false;
					sourcesPlayGema[i] = true;
				}
				for (int i = 0; i < TAMCOIN; i++) {
					isCollisionCoin[i] = false;
					sourcesPlayCoin[i] = true;
				}
				if (vueltaCont > 3) {
					ganar = true;
					velocidad -= 0.1;
				}
			}

			glm::mat4 modelMatrixLamboChasis = glm::mat4(modelMatrixLambo);
			modelMatrixLamboChasis = glm::scale(modelMatrixLamboChasis, glm::vec3(1.3, 1.3, 1.3));
			modelLambo.render(modelMatrixLamboChasis);
			glActiveTexture(GL_TEXTURE0);

			modelLamboLeftDor.render(modelMatrixLamboChasis);
			modelLamboRightDor.render(modelMatrixLamboChasis);
			modelLamboFrontLeftWheel.render(modelMatrixLamboChasis);
			modelLamboFrontRightWheel.render(modelMatrixLamboChasis);
			modelLamboRearLeftWheel.render(modelMatrixLamboChasis);
			modelLamboRearRightWheel.render(modelMatrixLamboChasis);
			// Se regresa el cull faces IMPORTANTE para las puertas
		}
		else if (renderParticles && it->second.first.compare("fountain") == 0) {
			/**********
			 * Init Render particles systems
			 */
			glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0);
			modelMatrixParticlesFountain = glm::translate(modelMatrixParticlesFountain, it->second.second);
			modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(modelMatrixParticlesFountain[3][0], modelMatrixParticlesFountain[3][2]) + 0.36 * 10.0;
			modelMatrixParticlesFountain = glm::scale(modelMatrixParticlesFountain, glm::vec3(3.0, 3.0, 3.0));
			currTimeParticlesAnimation = TimeManager::Instance().GetTime();
			if (currTimeParticlesAnimation - lastTimeParticlesAnimation > 10.0)
				lastTimeParticlesAnimation = currTimeParticlesAnimation;
			//glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			// Set the point size
			glPointSize(10.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
			shaderParticlesFountain.turnOn();
			shaderParticlesFountain.setFloat("Time", float(currTimeParticlesAnimation - lastTimeParticlesAnimation));
			shaderParticlesFountain.setFloat("ParticleLifetime", 3.5f);
			shaderParticlesFountain.setInt("ParticleTex", 0);
			shaderParticlesFountain.setVectorFloat3("Gravity", glm::value_ptr(glm::vec3(0.0f, -0.3f, 0.0f)));
			shaderParticlesFountain.setMatrix4("model", 1, false, glm::value_ptr(modelMatrixParticlesFountain));
			glBindVertexArray(VAOParticles);
			glDrawArrays(GL_POINTS, 0, nParticles);
			glDepthMask(GL_TRUE);
			//glEnable(GL_DEPTH_TEST);
			shaderParticlesFountain.turnOff();
			/**********
			 * End Render particles systems
			 */
		}
		else if (renderParticles && it->second.first.compare("topito") == 0 && ActivaTopito == 0) {
			glm::mat4 modelMatrixParticlesTopito = modelMatrixTopito;
			modelMatrixParticlesTopito[3][1] = terrain.getHeightTerrain(modelMatrixParticlesTopito[3][0], modelMatrixParticlesTopito[3][2]);
			modelMatrixParticlesTopito = glm::scale(modelMatrixParticlesTopito, glm::vec3(10.0, 10.0, 10.0));
			currTimeParticlesAnimationTopito = TimeManager::Instance().GetTime();
			if (currTimeParticlesAnimationTopito - lastTimeParticlesAnimationTopito > 10.0) {
				lastTimeParticlesAnimationTopito = currTimeParticlesAnimationTopito;
				ActivaTopito = 1;
			}
			//glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			// Set the point size
			glPointSize(12.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleTopitoID);
			shaderParticlesTopito.turnOn();
			shaderParticlesTopito.setFloat("Time", float(currTimeParticlesAnimationTopito - lastTimeParticlesAnimationTopito));
			shaderParticlesTopito.setFloat("ParticleLifetime", 3.5f);
			shaderParticlesTopito.setInt("ParticleTex", 0);
			shaderParticlesTopito.setVectorFloat3("Gravity", glm::value_ptr(glm::vec3(0.0f, -0.3f, 0.0f)));
			shaderParticlesTopito.setMatrix4("model", 1, false, glm::value_ptr(modelMatrixParticlesTopito));
			glBindVertexArray(VAOParticlesTopito);
			glDrawArrays(GL_POINTS, 0, nParticlesTopito);
			glDepthMask(GL_TRUE);
			//glEnable(GL_DEPTH_TEST);
			shaderParticlesTopito.turnOff();

		}
		else if (renderParticles && it->second.first.compare("fire") == 0 && vida <= 0) {
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(modelMatrixLambo);
			modelFireParticles = glm::translate(modelFireParticles, glm::vec3(7.0, 0, 0));
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(16, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

			/****************************+
			 * Open AL sound data
			 */
			source1Pos[0] = modelFireParticles[3].x;
			source1Pos[1] = modelFireParticles[3].y;
			source1Pos[2] = modelFireParticles[3].z;
			alSourcefv(source[1], AL_POSITION, source1Pos);

			/**********
			 * End Render particles systems
			 */
		}
		else if (it->second.first.compare("nitro") == 0 && usarNitro) {
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationNitro = currTimeParticlesAnimationNitro;
			currTimeParticlesAnimationNitro = TimeManager::Instance().GetTime();

			shaderParticlesNitro.setInt("Pass", 1);
			shaderParticlesNitro.setFloat("Time", currTimeParticlesAnimationNitro);
			shaderParticlesNitro.setFloat("DeltaT", currTimeParticlesAnimationNitro - lastTimeParticlesAnimationNitro);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedbackNitro[drawBufNitro]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArrayNitro[1 - drawBufNitro]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesNitro);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesNitro.setInt("Pass", 2);
			
			glm::mat4 modelNitroParticles = modelMatrixLambo;
			modelNitroParticles = glm::translate(modelNitroParticles, glm::vec3(2.01120, 0.54554, -3.5159));
			modelNitroParticles = glm::rotate(modelNitroParticles, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
			shaderParticlesNitro.setMatrix4("model", 1, false, glm::value_ptr(modelNitroParticles));

			shaderParticlesNitro.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleNitroID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArrayNitro[drawBufNitro]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesNitro);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBufNitro = 1 - drawBufNitro;
			shaderParticlesNitro.turnOff();

			/**********
			 * End Render particles systems
			 */
		}
	}
	glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv) {

	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
