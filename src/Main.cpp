#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "headers/Shader.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/Camera.h"
#include "headers/Object.h"
#include "headers/UBO.h"
#include "headers/Uniforms.h"
#include "headers/FBO.h"

// globals
const int INITIAL_WIDTH = 1280;
const int INITIAL_HEIGHT = 720;
int width = INITIAL_WIDTH;
int height = INITIAL_HEIGHT;
float prevX = 0.0f;
float prevY = 0.0f;
bool resize = false;
Camera cam(width, height, 45.0f);


// declare functions before use
void input(GLFWwindow* window, float dt);
void mouse_position_callback(GLFWwindow* window, double xPos, double yPos);
void framebuffer_size_callback(GLFWwindow* window, int x, int y);


int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLfloat vertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};


	GLFWwindow* window = glfwCreateWindow(width, height, "program", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	//glfwSwapInterval(0); // disable Vsync

	// call backs
	glfwSetCursorPosCallback(window, mouse_position_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// mouse mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, width, height);


	// create shaders
	Shader rayShader("./src/shaders/Default.vert", "./src/shaders/Ray.frag");
	Shader FBOShader("./src/shaders/Default.vert", "./src/shaders/FBO.frag");


	VAO VAO;
	VBO VBO(vertices, sizeof(vertices));

	VAO.bind();
	VAO.linkVBO(VBO, 0);
	VAO.linkVBO(VBO, 1);

	FBO FBO(width, height);
	FBOShader.use();
	glUniform1i(glGetUniformLocation(FBOShader.program, "text"), 0);

	UBO UBO;

	VAO.unBind();
	VBO.unBind();
	FBO.unBind();
	UBO.unBind();

	Uniforms uni;


	// materials
	Material materials[5];
	materials[0].color = glm::vec3(0.5, 0.2, 0.8);
	materials[0].lightStrength = 0.8;
	materials[0].lightColor = glm::vec3(0.5, 0.2, 0.8);

	materials[1].color = glm::vec3(0.1, 0.8, 0.85);
	materials[1].lightStrength = 2;
	materials[1].lightColor = glm::vec3(1);

	materials[2].color = glm::vec3(0.5, 0.7, 0.2);
	materials[2].lightStrength = 0;
	materials[2].lightColor = glm::vec3(0);

	materials[3].color = glm::vec3(0.8, 0.25, 0.25);
	materials[3].lightStrength = 0;
	materials[3].lightColor = glm::vec3(0);

	// star
	materials[4].color = glm::vec3(0);
	materials[4].lightStrength = 10;
	materials[4].lightColor = glm::vec3(1, 0.9, 1);


	// sphere objects
	const int spheresLength = 5;
	Sphere spheres[spheresLength];
	spheres[0].pos = glm::vec3(0, 0, 3);
	spheres[0].radius = 0.6;
	spheres[0].mat = materials[0];

	spheres[1].pos = glm::vec3(-3, 3, 1);
	spheres[1].radius = 0.4;
	spheres[1].mat = materials[1];

	spheres[2].pos = glm::vec3(-2, 1.2, 0);
	spheres[2].radius = 0.8;
	spheres[2].mat = materials[2];

	spheres[3].pos = glm::vec3(0, -10, 0);
	spheres[3].radius = 9;
	spheres[3].mat = materials[3];

	// star
	spheres[4].pos = glm::vec3(300, 400, -180);
	spheres[4].radius = 50;
	spheres[4].mat = materials[4];


	// delta time setup
	float currentTime, dt;
	float prevTime = glfwGetTime();

	// main loop
	while (!glfwWindowShouldClose(window)) {

		// input first
		currentTime = glfwGetTime();
		dt = currentTime - prevTime;
		prevTime = currentTime;
		cam.updateDT(dt);
		input(window, dt);

		// FBO resize
		if (resize) {
			resize = false;
			FBO.bind();
			FBO.resize(width, height);
			FBO.unBind();
		}


		// position debug
		//std::cout << cam.pos.x << " " << cam.pos.y << " " << cam.pos.z << std::endl; //"			 dir " << cam.direction.x << " " << cam.direction.y << " " << cam.direction.z << std::endl;
		
		// fps
		std::cout << int (1 / dt) << std::endl;


		// pass 1
		FBO.bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		rayShader.use();

		// uniforms
		uni.init(rayShader.program);
		uni.update(cam.pos, cam.model, width, height, cam.focus, spheresLength);
		UBO.bind();
		UBO.build(rayShader.program, spheres, spheresLength);

		VAO.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// pass 2
		FBO.unBind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		FBOShader.use();

		FBO.bindTexture();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO.deleteVAO();
	VBO.deleteVBO();
	FBO.deleteFBO();
	UBO.deleteUBO();
	rayShader.deleteShader();
	FBOShader.deleteShader();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


// resize window
void framebuffer_size_callback(GLFWwindow* window, int x, int y) {
	glViewport(0, 0, x, y);
	cam.updateRes(x, y);
	width = x;
	height = y;
	resize = true;
}

// mouse movement
void mouse_position_callback(GLFWwindow* window, double xPos, double yPos) {
	float x = static_cast<float>(xPos);
	float y = static_cast<float>(yPos);

	float dx = x - prevX;
	float dy = y - prevY; 
	prevX = x;
	prevY = y;

	cam.mouseInput(dx, dy, true);
}

// keyboard
void input(GLFWwindow* window, float dt) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
		glfwSetWindowShouldClose(window, true);
	cam.input(window);
}

