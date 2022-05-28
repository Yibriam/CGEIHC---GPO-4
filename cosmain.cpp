/*PROYECTO FINAL - CGEIHC
Nombre: Yibriam Galván Velázquez
Numero de cuenta: 313112267
*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(-160.0f, 30.0f, 145.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, -10.0f);	
float MovementSpeed = 0.5f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Ciclo de día
float reloj = 0.0f,
	  a= 0.0f,
	  b= -20000000.0f,
	  c= 0.0f,
	  d= 1.0f,
	  e= 1.0f;

//Luces apagadas y encendidas

float estadoluz = 0.0f;
float estadoluz2 = 0.0f;
int lucesautomaticas = 0.0f;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);	//oRIENTACIÓN DEL VECTOR DE LUZ ba de arriba a abajo y de enfrente hacia atrás


//Posiciones Avatar

float movXavatar = 0.0f,
	  movZavatar = 0.0f;

float rotavatar = 180.0f,
	  rotbrazoDer=0.0f,
	  rotbrazoIzq=0.0f,
	  rotpiernaDer=0.0f,
	  rotpiernaIzq=0.0f;

float velocidadAvatar = 1.2f;

float avatarcamina = false;

int estadopatada = 1;

//For Keyboard
float	movX = 0.0f,
		movY = 10.0f,
		movZ = 0.0f;

float	rotX = 0.0f,
		rotY = 0.0f,
		rotZ = 0.0f;


// Animación autos variables
float	movAuto1_x = 0.0f,
		movAuto1_z = 0.0f,
		movAuto2_x = 0.0f,
		movAuto2_z = 0.0f,
		rotAuto1 = 0.0f,
		rotAuto2 = 0.0f,
		rotz = 0.0f,
		radio = 5.0f,
		radio3 = 5.0f,
		caidaArbol = 0.0f,
		pochita_x = 0.0f,
		pochita_y = 0.0f,
		pochita_z = 0.0f,
		rotpochita = 0.0f,
		zim_x = 0.0f,
		zim_z = 0.0f,
		rotzim = 0.0f,
		gir_y = 0.0f,
		rotgir = 0.0f,
		orienta = 0.0f;
bool	animacion = false;
float   recorrido = 1.0f,
		recorrido1 = 1.0f,
		recorrido2 = 1.0f,
		recorrido3 = 1.0f,
		recorrido4 = 1.0f;
float   miVariable = 0.0f;
float   miVariable2 = 0.0f;
float   miVariable3 = 0.0f;

//Animación barco variables
float   P_barco_x = -40.0f,
		P_barco_y = -23.75f,
		P_barco_z = 400.0f,
		radio1 = 5.0f,
		radio2 = 5.0f;
float	O_barco = -90.0f;
int		r_barco = 1;

//Animación avión variables
float   P_avion_x = 490.0f,
		P_avion_y = 200.0f,
		P_avion_z = 470.0f;
float	O_avion = -135.0f;
float	O_avion2 = 0.0f;
float	O_avion3 = 0.0f;
int		r_avion = 1;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;

float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}

void animate(void)
{
	//Animando barco
	
	if (r_barco == 1) //Para atrás
	{
		radio1 += 0.05f;
		O_barco = -90.0f;
		P_barco_x -= 0.3f;
		P_barco_y += 0.05f * cos(radio1);
		if (P_barco_x < -440)
			r_barco = 2;
	}
	
	if (r_barco == 2) 
	{
		radio1 += 0.05f;
		radio2 += 5.0f;
		O_barco += 0.77f;
		O_barco += 0.05f * cos(radio2);
		P_barco_z += 0.3f;
		P_barco_y += 0.05f * cos(radio1);
		if (P_barco_z > 470)
			r_barco = 3;
	}
	
	if (r_barco == 3) //Arriba avanza
	{
		radio1 += 0.05f;
		O_barco = 90.0f;
		P_barco_x += 0.3f;
		P_barco_y += 0.05f * cos(radio1);
		if (P_barco_x >= -40)
			r_barco = 4;
	}
	
	if (r_barco == 4) //Baja
	{
		radio1 += 0.05f;
		O_barco += 0.72f;
		O_barco += 0.05f * cos(radio2);
		P_barco_z -= 0.3f;
		P_barco_y += 0.05f * cos(radio1);
		if (P_barco_z <= 400)
			r_barco = 5;
	}


	if (r_barco == 5) //Baja
	{
		
		P_barco_y += 0.05f;
		if (P_barco_y > 10)
			r_barco = 6;
	}

	if (r_barco == 6) //Baja
	{

		P_barco_y -= 0.05f;
		if (P_barco_y <= -25.75)
			r_barco = 1;
	}

	//Animando avion

	if (r_avion == 1) //Para atrás
	{
		O_avion = -135.0f;
		O_avion2 += 0.5f;
		P_avion_x -= 0.9f;
		P_avion_z -= 0.9f;
		if (P_avion_x <= -440)
			r_avion = 2;
	}
	if (r_avion == 2)
	{
		O_avion = 90.0f;
		O_avion2 = 0.0f;
		O_avion3 -= 0.355f;
		P_avion_x += 0.9f;

		if (P_avion_x >= 400)
			r_avion = 3;
	}
	if (r_avion == 3) //Arriba avanza
	{
		O_avion = 0.0f;
		O_avion2 += 0.5f;
		O_avion3 = 0.0f;
		P_avion_z += 0.9f;

		if (P_avion_z >= 470)
			r_avion = 4;
	}
	if (r_avion == 4) //Arriba avanza
	{
		O_avion = -90.0f;
		O_avion2 = 0.0f;
		P_avion_x -= 0.9f;

		if (P_avion_x <= -440)
			r_avion = 5;
	}
	if (r_avion == 5) //Arriba avanza
	{
		O_avion = -180.0f;
		P_avion_z -= 0.9f;

		if (P_avion_z <= -470)
			r_avion = 6;
	}

	if (r_avion == 6) //Arriba avanza
	{
		O_avion = 0.0f;
		P_avion_z += 0.9f;

		if (P_avion_z >= 470)
			r_avion = 7;
	}
	if (r_avion == 7) //Arriba avanza
	{
		O_avion = 90.0f;
		P_avion_x += 0.9f;

		if (P_avion_x >= 400)
			r_avion = 1;
	}


	//ECUACIONES PARAMÉTRICAS EN ESTE CASO ES UN CÍRCULO PARA LA LUZ AL REDEDOR DEL ESPECTACULAR
	lightPosition.x = 30.0f * cos(miVariable2)-70;
	lightPosition.z = 30.0f * sin(miVariable2)-170;
	miVariable2 += 0.05f;

	//ECUACIONES PARAMÉTRICAS EN ESTE CASO ES UN CÍRCULO PARA LA LUZ ALREDEDOR DEL MAPA (CICLO DE DÍA)
	lightDirection.x = 10.0f * cos(miVariable);
	lightDirection.y = 10.0f * sin(miVariable);
	//lightDirection.y = 50.0f * cos(miVariable);
	miVariable -= 0.001f;
	
	if (lucesautomaticas == 0.0f) 
	{
		miVariable3 -= 0.001f;
		if (miVariable3 <= -3.0f)
		{
			estadoluz2 = 1.0f;
			lucesautomaticas = 1.0f;
		}
			
	}
	if (lucesautomaticas == 1.0f)
	{
		miVariable3 += 0.001f;
		if (miVariable3 >= 0.0f)
		{
			estadoluz2 = 0.0f;
			lucesautomaticas = 0.0f;
			miVariable = 0.0f;
		}

	}
		
	if (estadoluz == 1)
	{
		//Cambio color RGB luces
		if (c < 1) {
			c += 0.01f;
		}

		if (c >= 1) {
			d -= 0.01f;
		}
		if (c >= 1 && d <= 0) {
			e -= 0.01;
		}
		if (c >= 1 && d <= 0 && e <= 0)
		{
			c = 0.0;
			d = 1.0;
			e = 1.0f;
		}
	}
	
	if (estadoluz == 0)
	{
		c = 0.0f;
		d = 0.0f;
		e = 0.0f;
	}
	
	
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	//Vehículo carromotor
	if (animacion)
	{
		if (recorrido == 1)
		{
			movAuto1_x += 1.0f;
			if (movAuto1_x > 130)
				recorrido = 2;
		}
		if (recorrido == 2)
		{
			rotAuto1 = -90.0f;
			movAuto1_z += 1.0f;
			if (movAuto1_z > 70)
				recorrido = 3;
		}
		if (recorrido == 3)
		{
			rotAuto1 = 0.0f;
			movAuto1_x += 1.5f;
			if (movAuto1_x > 400)
				recorrido = 4;
		}
		if (recorrido == 4)
		{
			rotAuto1 = 90.0f;
			movAuto1_z -= 2.0f;
			if (movAuto1_z < -400)
				recorrido = 5;
		}
		if (recorrido == 5)
		{
			rotAuto1 = -180.0f;
			movAuto1_x -= 1.18f;
			if (movAuto1_x < 150)
				recorrido = 6;
		}
		if (recorrido == 6)
		{
			rotAuto1 += 5.0f;
			rotz += 0.1f;
			movAuto1_x -= 0.3f;
			movAuto1_x += 0.05f * cos(radio);
			movAuto1_z += 1.0f;
			if (movAuto1_z > -355)
				recorrido = 7;
		}
	}
	
	//Vehículo camión
	if (animacion)
	{
		if (recorrido1 == 1)
		{
			movAuto2_z -= 0.8f;
			if (movAuto2_z < -250)
				recorrido1 = 2;
		}
		if (recorrido1 == 2)
		{
			rotAuto2 += 0.2f;
			movAuto2_x -= 0.5f;
			movAuto2_z -= 1.0f;
			if (movAuto2_z < -360)
				recorrido1 = 3;
		}
		if (recorrido1 == 3)
		{
			caidaArbol += 0.5f;
			rotAuto2 -= 0.66f;
			movAuto2_x += 0.05f * cos(radio);
			movAuto2_x += 0.5f;
			movAuto2_z -= 0.28f;
			if (movAuto2_x > 20)
				recorrido1 = 4;
		}
		if (recorrido1 == 4)
		{
			if (rotAuto2 >= -90.0)
				rotAuto2 -= 0.8f;
			movAuto2_x += 0.5f;
			if (movAuto2_x > 150)
				recorrido1 = 5;
		}
		if (recorrido1 == 5)
		{
			movAuto2_x += 0.6f;
			rotAuto2 += 0.6f;
			movAuto2_x += 0.05f * cos(radio);
			if (movAuto2_x > 165)
				recorrido1 = 6;
		}
	}

	//Personaje Pochita
	if (animacion)
	{
		if (recorrido2 == 1)
		{
			radio += 0.05f;
			pochita_z += 0.05f;
			pochita_y += 0.05f * cos(radio);
			printf("pochita_z, %f\n", pochita_z);
			printf("pochita_y, %f\n", pochita_y);
			if (pochita_z > 40)
				recorrido2 = 2;
		}
		if (recorrido2 == 2)
		{
			radio += 0.05f;
			pochita_z += 0.05f;
			pochita_x -= 0.075f;
			pochita_y += 0.05f * cos(radio);
			rotpochita -= 0.65f;
			if (rotpochita < -180)
				recorrido2 = 3;
		}
		if (recorrido2 == 3)
		{
			radio += 0.05f;
			pochita_z -= 0.05f;
			pochita_y += 0.05f * cos(radio);
			printf("pochita_z, %f\n", pochita_z);
			printf("pochita_y, %f\n", pochita_y);
			if (pochita_z < 41)
				recorrido2 = 4;
		}
		if (recorrido2 == 4)
		{
			radio += 0.05f;
			pochita_z -= 0.015f;
			pochita_y += 0.05f;
			pochita_y += 0.05f * cos(radio);
			printf("pochita_z, %f\n", pochita_z);
			printf("pochita_y, %f\n", pochita_y);
			if (pochita_y > 27.3)
				recorrido2 = 5;
		}
		if (recorrido2 == 5)
		{
			pochita_z -= 0.1f;
			printf("pochita_z, %f\n", pochita_z);
			if (pochita_z < 21)
				recorrido2 = 6;
		}
		if (recorrido2 == 6)
		{
			pochita_z -= 0.1f;
			pochita_y -= 0.1f;
			printf("pochita_z, %f\n", pochita_z);
			if (pochita_z < -3)
				recorrido2 = 7;
		}
	}
	// Personaje Zim

		if (recorrido3 == 1)
		{
			zim_z += 0.1f;
			if (zim_z > 30)
				recorrido3 = 2;
		}
		if (recorrido3 == 2)
		{
			rotzim = -90.0f;
			zim_x -= 0.1f;
			if (zim_x < -100)
				recorrido3 = 3;
		}
		if (recorrido3 == 3)
		{
			rotzim = -180.0f;
			zim_z -= 0.1f;
			if (zim_z < -15)
				recorrido3 = 4;
		}
		if (recorrido3 == 4)
		{
			rotzim = -90.0f;
			zim_x -= 0.1f;
			if (zim_x < -145)
				recorrido3 = 5;
		}
		if (recorrido3 == 5)
		{
			rotzim = 0.0f;
			zim_z += 0.1f;
			if (zim_z > 40)
				recorrido3 = 6;
		}
		if (recorrido3 == 6)
		{
			rotzim = 90.0f;
			zim_x += 0.1f;
			if (zim_x > -100)
				recorrido3 = 7;
		}
		if (recorrido3 == 7)
		{
			rotzim = 180.0f;
			zim_z -= 0.1f;
			if (zim_z < 30)
				recorrido3 = 8;
		}
		if (recorrido3 == 8)
		{
			rotzim = 90.0f;
			zim_x += 0.1f;
			if (zim_x > 0)
				recorrido3 = 9;
		}
		if (recorrido3 == 9)
		{
			rotzim = 180.0f;
			zim_z -= 0.1f;
			if (zim_z < 0)
				recorrido3 = 10;
		}
		if (recorrido3 == 10)
		{
			rotzim = 0.0f;
			recorrido3 = 1;
		}

	// Personaje Gir

		if (recorrido4 == 1)
		{
			radio3 += 0.05f;
			gir_y += 0.1f * cos(radio3);
			rotgir += 2.0f;
			if (gir_y < 10)
				recorrido4 = 1;
		}
		

}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void avatar_camina()
{
	if (avatarcamina==true) 
	{
		if (estadopatada ==1)
		{
			rotpiernaDer += 1.2f;
			rotpiernaIzq -= 1.2f;

			if (rotpiernaDer >= 90.0f)
				estadopatada = 2;
		}
		if (estadopatada == 2)
		{
			rotpiernaDer -= 1.2f;
			rotpiernaIzq += 1.2f;

			if (rotpiernaDer <= -90.0f)
				estadopatada = 1;
		}
	}
}

int main()
{
	glfwInit();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Entrada mouse
	glfwSetCursorPosCallback(window, mouse_callback);
	
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------

	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");



	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models

	//---------CHINATOWN
	Model piso("resources/obj/piso/piso-0-1.obj");
	Model carromotor("resources/obj/carromotor/carromotor.obj");
	Model camionazul("resources/obj/camionazul/camionazul.obj");
	Model policia("resources/obj/policia/policia.obj");
	Model camioneta1("resources/obj/camioneta/camioneta1.obj");
	Model venado("resources/obj/venado/venado.obj");
	Model semaforo("resources/obj/semaforo/semaforo.obj");
	Model semaforo2("resources/obj/semaforo2/semaforo2.obj");
	Model farola("resources/obj/farola/farola.obj");
	Model banca("resources/obj/banca/banca.obj");
	Model montana("resources/obj/montana/montana.obj");
	Model espectacular("resources/obj/espectacular/espectacular.obj");
	Model muralla("resources/obj/muralla/muralla.obj");
	Model murallaP("resources/obj/murallaP/murallaP.obj");
	Model chopper("resources/obj/chopper/chopper.obj");
	Model thousandSunny("resources/obj/thousandSunny/thousandSunny.obj");
	Model zoro("resources/obj/zoro/zoro.obj");
	Model arbol1("resources/obj/arbol1/arbol1.obj");
	Model denji("resources/obj/denji/denji.obj");
	Model legoPlane("resources/obj/legoPlane/legoPlane.obj");
	Model slide("resources/obj/slide/slide.obj");
	Model pochita("resources/obj/pochita/pochita.obj");
	Model Tree("resources/obj/Tree/Tree1.obj");
	Model Tree3("resources/obj/Tree3/Tree3.obj");
	Model zonajuegos("resources/obj/zonajuegos/zonajuegos.obj");
	Model zim("resources/obj/zim/zim.obj");
	Model gir("resources/obj/gir/gir.obj");
	//edificios
	Model edificio1("resources/obj/edificio1/edificio1.obj");
	Model edificio2("resources/obj/edificio2/edificio2.obj");
	Model edificio3("resources/obj/edificio3/edificio3.obj");
	Model edificio4("resources/obj/edificio4/edificio4.obj");
	Model edificio5("resources/obj/edificio5/edificio5.obj");
	Model edificio6("resources/obj/edificio6/edificio6.obj");
	Model base("resources/obj/base/base.obj");
	//Shinobu
	Model shinobu("resources/obj/shinobu/head.obj");
	//Inosuke
	Model inosukecabeza("resources/obj/inosuke/head.obj");
	Model inosukepecho("resources/obj/inosuke/pecho.obj");
	Model inosukebrazoDer("resources/obj/inosuke/brazo.obj");
	Model inosukebrazoIzq("resources/obj/inosuke/brazo.obj");
	Model inosukepiernaDer("resources/obj/inosuke/pierna.obj");
	Model inosukepiernaIzq("resources/obj/inosuke/pierna.obj");


	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// don't forget to enable shader before setting uniforms
		staticShader.use();

		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.65f, 0.65f, 0.65f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(c, d, e));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 1.0f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.01f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-40.0f, 0.0f, -140.0f));	//Esta luz a diferencia de la ota tiene una posición fija
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(e, c, d));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.01f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(-40.0f, 0.0f, -200.0f));	//Esta luz a diferencia de la ota tiene una posición fija
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(d, e, c));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.01f);

		staticShader.setVec3("pointLight[3].position", glm::vec3(-40.0f, 0.0f, -170.0f));	//Esta luz a diferencia de la ota tiene una posición fija
		staticShader.setVec3("pointLight[3].ambient", glm::vec3(d, d, d));
		staticShader.setVec3("pointLight[3].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[3].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[3].constant", 1.0f);
		staticShader.setFloat("pointLight[3].linear", 0.009f);
		staticShader.setFloat("pointLight[3].quadratic", 0.01f);

		staticShader.setVec3("pointLight[4].position", glm::vec3(145.0f, 0.0f, -170.0f));	//Esta luz a diferencia de la ota tiene una posición fija
		staticShader.setVec3("pointLight[4].ambient", glm::vec3(estadoluz2, estadoluz2, estadoluz2));
		staticShader.setVec3("pointLight[4].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[4].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[4].constant", 1.0f);
		staticShader.setFloat("pointLight[4].linear", 0.009f);
		staticShader.setFloat("pointLight[4].quadratic", 0.01f);

		staticShader.setVec3("pointLight[5].position", glm::vec3(145.0f, 0.0f, -320.0f));	//Esta luz a diferencia de la ota tiene una posición fija
		staticShader.setVec3("pointLight[5].ambient", glm::vec3(estadoluz2, estadoluz2, estadoluz2));
		staticShader.setVec3("pointLight[5].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[5].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[5].constant", 1.0f);
		staticShader.setFloat("pointLight[5].linear", 0.009f);
		staticShader.setFloat("pointLight[5].quadratic", 0.01f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);


		
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-75.0f, -1.75f, -110.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(movAuto1_x -90.0f, -1.75f, movAuto1_z -60.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotAuto1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotz), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		carromotor.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(movAuto2_x -130.0f, -1.75f, movAuto2_z-60.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		model = glm::rotate(model, glm::radians(rotAuto2), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		camionazul.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, -1.75f, 170.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		policia.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, -1.75f, 70.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		camioneta1.Draw(staticShader);

		/*model = glm::translate(glm::mat4(1.0f), glm::vec3(-208.0f, 31.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		venado.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-240.0f, 58.0f, 90.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		venado.Draw(staticShader);*/

		model = glm::translate(glm::mat4(1.0f), glm::vec3(320.0f, -1.75f, 20.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		semaforo.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, -1.75f, -71.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		semaforo2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(155.0f, -1.75f, -175.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		farola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(155.0f, -1.75f, -325.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		farola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(270.0f, -1.75f, -80.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(270.0f, -1.75f, -230.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(270.0f, -1.75f, -380.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		banca.Draw(staticShader);

		/*model = glm::translate(glm::mat4(1.0f), glm::vec3(-330.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f,65.0f,71.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		montana.Draw(staticShader);*/

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-70.0f, -1.75f, -170.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		espectacular.Draw(staticShader);

		/*model = glm::translate(glm::mat4(1.0f), glm::vec3(495.0f, -1.75f, 174.0f));
		model = glm::scale(model, glm::vec3(5.0f, 25.0f, 25.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		muralla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(495.0f, -1.75f, 274.0f));
		model = glm::scale(model, glm::vec3(5.0f, 25.0f, 25.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		muralla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(495.0f, -1.75f, -476.0f));
		model = glm::scale(model, glm::vec3(5.0f, 25.0f, 25.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		muralla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(230.0f, -1.75f, -746.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		muralla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-280.0f, -1.75f, -746.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		muralla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-555.0f, -1.75f, -476.0f));
		model = glm::scale(model, glm::vec3(5.0f, 25.0f, 25.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		muralla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-555.0f, -1.75f, 20.0f));
		model = glm::scale(model, glm::vec3(5.0f, 25.0f, 25.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		muralla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-555.0f, -1.75f, 270.0f));
		model = glm::scale(model, glm::vec3(5.0f, 25.0f, 25.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		muralla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-295.0f, -1.75f, 530.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		muralla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(230.0f, -1.75f, 530.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		muralla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(495.0f, -1.75f, -176.0f));
		model = glm::scale(model, glm::vec3(5.0f, 25.0f, 25.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		murallaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 29.75f, 130.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(O_barco), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		chopper.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(135.0f, 15.75f, 90.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(O_avion2-180), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		zoro.Draw(staticShader);*/

		model = glm::translate(glm::mat4(1.0f), glm::vec3(zim_x -200.0f, 0.0f, zim_z + 110.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(rotzim), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		zim.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-210.0f, gir_y + 5.0f, 168.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(rotgir), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		gir.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-190.0f, -1.75f, -450.0f));
		model = glm::rotate(model, glm::radians(caidaArbol), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(P_avion_x, P_avion_y, P_avion_z));
		model = glm::scale(model, glm::vec3(7.0f));
		model = glm::rotate(model, glm::radians(O_avion), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(O_avion2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(O_avion3), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		legoPlane.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(P_barco_x, P_barco_y - 2.0f, P_barco_z));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(O_barco), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		thousandSunny.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pochita_x + 70.0f, pochita_y - 1.63, pochita_z + 310.0f));
		model = glm::rotate(model, glm::radians(rotpochita), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		pochita.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.75f, 330.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		slide.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(115.0f, -1.75f, 330.0f));
		model = glm::scale(model, glm::vec3(21.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		zonajuegos.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(180.0f, -1.75f, 400.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		Tree.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(380.0f, -1.75f, 135.0f));
		model = glm::scale(model, glm::vec3(38.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		edificio1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, -1.75f, -580.0f));
		model = glm::scale(model, glm::vec3(38.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		edificio2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(390.0f, -1.75f, -390.0f));
		model = glm::scale(model, glm::vec3(38.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		edificio3.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.75f, -300.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		edificio4.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(380.0f, -1.75f, -280.0f));
		model = glm::scale(model, glm::vec3(18.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		edificio5.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(380.0f, -1.75f, -60.0f));
		model = glm::scale(model, glm::vec3(25.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		edificio6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-250.0f, -1.75f, 130.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		base.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------

		//AVATAR INOSUKE

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(movXavatar - 3.0f, -0.24f, movZavatar));
		////model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//tmp = model = glm::rotate(model, glm::radians(rotavatar), glm::vec3(0.0f, 1.0f, 0.0));
		//staticShader.setMat4("model", model);
		//inosukepecho.Draw(staticShader);

		////Pierna Der
		//model = glm::translate(tmp, glm::vec3(-0.8f, -2.5f, 0.0f));
		////model = glm::rotate(model, glm::radians(rotpiernaDer), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//inosukepiernaDer.Draw(staticShader);

		////Pierna Izq
		//model = glm::translate(tmp, glm::vec3(0.8f, -2.5f, -0.0f));
		////model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		////model = glm::rotate(model, glm::radians(rotpiernaIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//inosukepiernaIzq.Draw(staticShader);

		////Brazo derecho
		////model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::translate(tmp, glm::vec3(-1.5f, 1.0f, 0.6f));
		////model = glm::rotate(model, glm::radians(rotbrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//inosukebrazoDer.Draw(staticShader);

		////Brazo izquierdo
		////model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::translate(tmp, glm::vec3(1.5f, 1.0f, 0.6f));
		////model = glm::rotate(model, glm::radians(rotbrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//inosukebrazoIzq.Draw(staticShader);

		////Cabeza
		//model = glm::translate(tmp, glm::vec3(1.0f, -5.6f, 5.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		////model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		//model = glm::scale(model, glm::vec3(0.8f));
		//staticShader.setMat4("model", model);
		//inosukecabeza.Draw(staticShader);

		//avatar_camina();


		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		glBindVertexArray(0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Cámara
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
		movZavatar -= velocidadAvatar;
		rotavatar = 180.0f;
		avatarcamina = true;
	}
		
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
		movZavatar += velocidadAvatar;
		rotavatar = 0.0f;
		avatarcamina = true;
	}
		
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
		movXavatar -= velocidadAvatar;
		rotavatar = -90.0f;
		avatarcamina = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
		movXavatar += velocidadAvatar;
		rotavatar = 90.0f;
		avatarcamina = true;
	}
		else
		{
			avatarcamina = false;
		}

	//Luces Manuales
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		if (estadoluz ==0.0f)
			estadoluz = 1.0f;
		
		else
			estadoluz = 0.0f;
	}
	
	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	if (key == GLFW_KEY_SPACE && recorrido == 7 && recorrido1 == 6)
	{
		recorrido = 1;
		recorrido1 = 1;
		movAuto1_x = 0.0f;
		movAuto1_z = 0.0f;
		movAuto2_x = 0.0f;
		movAuto2_z = 0.0f;
		caidaArbol = 0.0f;
		rotAuto1 = 0.0f;
		rotAuto2 = 0.0f;
		rotz = 0.0f;
		animacion ^= true;
	}

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



