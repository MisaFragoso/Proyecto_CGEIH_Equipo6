#include <iostream>
#include <cmath>

// GLEW y GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 lightPos2(0.0f, 0.0f, 0.0f);
bool active;
bool active2;

// Variables para las animaciones
float tiempo;
float rot = 0.0f;
float rot2 = 0.0f;
bool anim = false;
bool anim2 = false;
float speed = 1.0f;

// Variables para la animacion simple 1 de la puerta Izquierda
float angle_offset = 0.0f;
float angular_speed = 1.0f;
// Variables para la animacion simple 2 de la puerta Derecha
float angle_offset2 = 0.0f;
float angular_speed2 = 1.0f;
// Variables para la animacion simple 3 de la camara de seguridad
float angle_offset3 = 0.0f;
float angular_speed3 = 1.0f;
// Variables para la animacion compleja
float angle_offset4 = 0.0f;
float angular_speed4 = 1.0f;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(2.0f, 17.0f, -34.5f),  //0 Luces superiores
	glm::vec3(2.0f, 17.0f, -18.7f),  //1
	glm::vec3(2.0f, 17.0f,  -2.4f),  //2
	glm::vec3(2.0f, 5.615f, 6.36f),  //3
	glm::vec3(2.0f,  7.5f, -38.5f),  //4 Luces inferiores
	glm::vec3(2.0f,  7.5f, -30.5f),  //5
	glm::vec3(2.0f,  7.5f, -22.5f),  //6
	glm::vec3(2.0f,  7.5f, -14.5f)   //7
};

float vertices[] = {
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(0);
glm::vec3 Light5 = glm::vec3(0);
glm::vec3 Light6 = glm::vec3(0);
glm::vec3 Light7 = glm::vec3(0);
glm::vec3 Light8 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto FInal - CGeIHC -  Equipo 6 - Gpo 1", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	//Carga de los Shaders
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	//Carga de modelos (Misael)
	Model asadero((char*)"Models/misa/asadero/asadero.obj");
	Model ATM((char*)"Models/misa/ATM/ATM.obj");
	Model barandilla((char*)"Models/misa/barandilla/barandilla.obj");
	Model bench((char*)"Models/misa/bench/bench.obj");
	Model bench2((char*)"Models/misa/bench/bench2.obj");
	Model bocina((char*)"Models/misa/bocina/bocina.obj");
	Model box((char*)"Models/misa/box/cuerpoBox.obj");
	Model box2((char*)"Models/misa/box/pera.obj");
	Model cabina((char*)"Models/misa/cabina/cabina.obj");
	Model caja((char*)"Models/misa/caja/caja.obj");
	Model cine((char*)"Models/misa/cine/estrucP.obj");
	Model escaleras((char*)"Models/misa/escaleras/escaleras.obj");
	Model estrucint((char*)"Models/misa/estrucint/estrucint.obj");
	Model extintor((char*)"Models/misa/extintor/extintor.obj");

	Model helado((char*)"Models/misa/helado/helado.obj");
	Model hotdog((char*)"Models/misa/hotdog/cajahotdog.obj");
	Model hotdog2((char*)"Models/misa/hotdog/hotdog.obj");
	Model hotdog3((char*)"Models/misa/hotdog/salchicha.obj");
	Model hotdog4((char*)"Models/misa/hotdog/salTub1.obj");
	Model hotdog5((char*)"Models/misa/hotdog/salTub2.obj");
	Model hotdog6((char*)"Models/misa/hotdog/salTub3.obj");

	//Model kiosko((char*)"Models/misa/kiosko/kiosko.obj");
	Model lamp1((char*)"Models/misa/lamp1/lamp1.obj");
	Model lamp2((char*)"Models/misa/lamp1/lamp2.obj");
	Model letras((char*)"Models/misa/letras/letras.obj");
	Model monitor((char*)"Models/misa/monitor/monitor.obj");
	Model palom((char*)"Models/misa/palom/palom.obj");
	Model palom2((char*)"Models/misa/palom/tapalom.obj");
	Model pantalla((char*)"Models/misa/pantalla/pantalla.obj");
	Model personas((char*)"Models/misa/personas/personas.obj");
	Model plantas((char*)"Models/misa/plantas/plantas.obj");
	//Model posters((char*)"Models/misa/posters/posters.obj");
	//Model publi((char*)"Models/misa/publi/publi.obj");
	Model puerta1((char*)"Models/misa/puertas/puerta1.obj");
	Model puerta2((char*)"Models/misa/puertas/puerta2.obj");
	Model queso((char*)"Models/misa/queso/queso.obj");
	Model sillon((char*)"Models/misa/sillon/sillon.obj");
	Model slush((char*)"Models/misa/slush/slush.obj");



	//Carga de modelos (JONY)
	Model Fachada((char*)"Models/jony/Fachada/exterior.obj");
	Model Ventanales((char*)"Models/Fachada/ventanal.obj");       //Transparencia y canal alpha
	//Model CamBas((char*)"Models/2_Camara/bascam.obj");
	//Model Cam((char*)"Models/2_Camara/cam.obj");

	
	

	//Carga de modelos WEB free y de Inteligencia artificial por Luma AI
	//Model Dron((char*)"ModelsWebIA/Dron/dron.obj");

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right_day.tga");
	faces.push_back("SkyBox/left_day.tga");
	faces.push_back("SkyBox/up_day.tga");
	faces.push_back("SkyBox/down_day.tga");
	faces.push_back("SkyBox/back_day.tga");
	faces.push_back("SkyBox/front_day.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        //glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f); //  -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.5f, 0.5f, 0.5f);        //Deja los mismos valores para 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);       //ambient y diffuse
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);       //Para la intensidad

		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= Light1.x;//abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= Light1.y;//abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= Light1.z;//sin(glfwGetTime() *Light1.z);		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.2f, 0.2f, 0.2f); //1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.7f);//0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 1.0f); //0.075f);
		
		// Point light 2
		glm::vec3 lightColor2;
		lightColor2.x = Light2.x;
		lightColor2.y = Light2.y;
		lightColor2.z = Light2.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 1.0f);

		// Point light 3
		glm::vec3 lightColor3;
		lightColor3.x = Light3.x;
		lightColor3.y = Light3.y;
		lightColor3.z = Light3.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 1.0f);

		// Point light 4
		glm::vec3 lightColor4;
		lightColor4.x = Light4.x;
		lightColor4.y = Light4.y;
		lightColor4.z = Light4.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.9f, 0.9f, 0.9f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.35f);       //0.7
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.44f);    //1.0

		// Point light 5
		glm::vec3 lightColor5;
		lightColor5.x = Light5.x;
		lightColor5.y = Light5.y;
		lightColor5.z = Light5.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), lightColor5.x, lightColor5.y, lightColor5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), lightColor5.x, lightColor5.y, lightColor5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 0.2f, 2.0f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.44f);

		// Point light 6
		glm::vec3 lightColor6;
		lightColor6.x = Light6.x;
		lightColor6.y = Light6.y;
		lightColor6.z = Light6.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), lightColor6.x, lightColor6.y, lightColor6.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), lightColor6.x, lightColor6.y, lightColor6.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 1.0f);
				
		// Point light 7
		glm::vec3 lightColor7;
		lightColor7.x = Light7.x;
		lightColor7.y = Light7.y;
		lightColor7.z = Light7.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), lightColor7.x, lightColor7.y, lightColor7.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), lightColor7.x, lightColor7.y, lightColor7.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), 1.0f);

		// Point light 8
		glm::vec3 lightColor8;
		lightColor8.x = Light8.x;
		lightColor8.y = Light8.y;
		lightColor8.z = Light8.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].position"), pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].ambient"), lightColor8.x, lightColor8.y, lightColor8.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].diffuse"), lightColor8.x, lightColor8.y, lightColor8.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].quadratic"), 1.0f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);	

		//+++++++++++++++++++++++++++++++++++++++Carga de modelos de la fachada (SIN TRANSPARENCIA)++++++++++++++
        view = camera.GetViewMatrix();	

		//CARGA DE MODELOS DE MUSICA
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Fachada.Draw(lightingShader);
		//Ventanales.Draw(lightingShader);

		//CARGA DE MODELOS DEL CINE Y CENTRO COMERCIAL
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		asadero.Draw(lightingShader);
		ATM.Draw(lightingShader);
		barandilla.Draw(lightingShader);
		bench.Draw(lightingShader);
		bench2.Draw(lightingShader);
		bocina.Draw(lightingShader);
		box.Draw(lightingShader);
		box2.Draw(lightingShader);
		cabina.Draw(lightingShader);
		caja.Draw(lightingShader);
		cine.Draw(lightingShader);
		escaleras.Draw(lightingShader);
		estrucint.Draw(lightingShader);
		extintor.Draw(lightingShader);
		helado.Draw(lightingShader);
		hotdog.Draw(lightingShader);
		hotdog2.Draw(lightingShader);
		hotdog4.Draw(lightingShader);
		hotdog5.Draw(lightingShader);
		hotdog6.Draw(lightingShader);
		hotdog3.Draw(lightingShader);
		//kiosko.Draw(lightingShader);
		lamp1.Draw(lightingShader);
		lamp2.Draw(lightingShader);
		letras.Draw(lightingShader);
		monitor.Draw(lightingShader);
		palom.Draw(lightingShader);
		palom2.Draw(lightingShader);
		pantalla.Draw(lightingShader);
		personas.Draw(lightingShader);
		plantas.Draw(lightingShader);
		//posters.Draw(lightingShader);
		//publi.Draw(lightingShader);
		puerta1.Draw(lightingShader);
		puerta2.Draw(lightingShader);
		queso.Draw(lightingShader);
		sillon.Draw(lightingShader);
		slush.Draw(lightingShader);


		//**************************************************************ANIMACION SIMPLE ************************

		float oscillation_angle3 = sin(glfwGetTime() * angular_speed3) * 45.0f; 
		float rotation_angle3 = oscillation_angle3 + angle_offset;
		rotation_angle3 = glm::clamp(rotation_angle3, -45.0f, 45.0f);

		if (rotation_angle3 >= 45.0f || rotation_angle3 <= -45.0f) {
			angle_offset3 = -angle_offset3; 
		}

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.046f, 5.469f, 2.462f));
		model = glm::rotate(model, glm::radians(oscillation_angle3), glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		//Cam.Draw(lightingShader);


		// ++++++++++++++++++++++++++++++++++Carga de los modelos principales (SIN TRANSPARENCIA)+++++++++++++
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		
		//Atril.Draw(lightingShader);
		

			
		//---------------------------------------Carga de los modelos principales (CON TRANSPARENCIA)-------------

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.5f);  //Color de la transparencia y valor de transparencia de alpha
		
		//Carga de objetos principales con transparencia
		//Atril2.Draw(lightingShader);

		glDisable(GL_BLEND);  //Desactiva el canal alfa 

		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);  //Para reiniciar el color alpha

		glBindVertexArray(0);

	    // ---------------------------------------------------------------------------------------Iluminacion-----------------

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 8; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		//----------------------------------------------------------------------------Animaci�n COMPLEJA------------
		Anim.Use();
		model = glm::mat4(1);
		tiempo = glfwGetTime() * speed;
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		float oscillation_angle4 = sin(glfwGetTime() * angular_speed4) * 45.0f;
		float rotation_angle4 = oscillation_angle4 + angle_offset;
		rotation_angle4 = glm::clamp(rotation_angle4, -270.0f, 270.0f);

		if (rotation_angle4 >= 270.0f || rotation_angle4 <= -270.0f) {
			angle_offset4 = -angle_offset4;
		}

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 17.0f, 0.0f));
		model = glm::rotate(model, glm::radians(oscillation_angle4), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		//Dron.Draw(Anim);
		glBindVertexArray(0);


		// ------------------------------------------------------------------------------------------------------------Skybox-------

		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Moves/alters the camera positions based on user input     ---------  Tu animacion no se detiene hasta finalizar el programa
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{	camera.ProcessKeyboard(FORWARD, deltaTime);	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{	camera.ProcessKeyboard(BACKWARD, deltaTime);}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{	camera.ProcessKeyboard(LEFT, deltaTime);	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{	camera.ProcessKeyboard(RIGHT, deltaTime);	}

	// Para mover la luz
	/*if (keys[GLFW_KEY_T])
	{	pointLightPositions[0].x += 0.1f;	}

	if (keys[GLFW_KEY_G])
	{	pointLightPositions[0].x -= 0.1f;	}

	if (keys[GLFW_KEY_Y])
	{	pointLightPositions[0].y += 0.01f;	}

	if (keys[GLFW_KEY_H])
	{	pointLightPositions[0].y -= 0.01f;	}

	if (keys[GLFW_KEY_U])
	{	pointLightPositions[0].z -= 0.01f;	}

	if (keys[GLFW_KEY_J])
	{	pointLightPositions[0].z += 0.01f;	}*/


	//Animacion de la puerta Izq
	if (anim) {
		if (rot < 120.0f) {
			rot -= 4.0f;
		}		
	}

	//Animacion de la puerta Der
	if (anim2) {
		if (rot2 > 0.0f) {
			rot2 -= 4.0f;
		}
	}

}

// Is called whenever a key is pressed/released via GLFW  ---     Tu animacion se puede detener con una tecla
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	// ACTIVACION DE LAS POINTLIGTHS
	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
			Light2 = glm::vec3(1.0f, 1.0f, 1.0f);
			Light3 = glm::vec3(1.0f, 1.0f, 1.0f);
			Light4 = glm::vec3(1.0f, 1.0f, 1.0f);	

			Light5 = glm::vec3(1.0f, 0.0f, 1.0f);
			Light6 = glm::vec3(1.0f, 1.0f, 0.0f);
			Light7 = glm::vec3(1.0f, 1.0f, 1.0f);
			Light8 = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
			Light2 = glm::vec3(0);
			Light3 = glm::vec3(0);
			Light4 = glm::vec3(0);		

			Light5 = glm::vec3(0);
			Light6 = glm::vec3(0);
			Light7 = glm::vec3(0);
			Light8 = glm::vec3(0);
		}
	}


	//Animacion puerta izquierda
	if (keys[GLFW_KEY_M]) {
		anim = !anim;
		if (anim) {
			rot = 0.0f;
		}
		else { rot = 120.0f; }
	}

	//Animacion Puerta DEr
	if (keys[GLFW_KEY_N]) {
		anim2 = !anim2;
		if (anim2) {
			rot2 = -90.0f;
		}

		else { rot2 = 0.0f; }
	}

}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}