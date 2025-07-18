#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "headers/Shader.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/EBO.h"
#include "headers/Camera.h"
#include "headers/Object.h"
#include "headers/UBO.h"
#include "headers/Uniforms.h"

// globals
const int INITIAL_WIDTH = 1280;
const int INITIAL_HEIGHT = 720;
int width = INITIAL_WIDTH;
int height = INITIAL_HEIGHT;
float prevX = 0.0f;
float prevY = 0.0f;
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
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	GLuint indicies[] = {
		0, 1, 2,
		1, 2, 3
	};

	GLFWwindow* window = glfwCreateWindow(width, height, "program", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_position_callback);

	// mouse mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	glViewport(0, 0, width, height);


	Shader shader("./src/shaders/Default.vert", "./src/shaders/Default.frag");


	VAO VAO;
	VAO.bind();

	VBO VBO(vertices, sizeof(vertices));
	EBO EBO(indicies, sizeof(indicies));

	VAO.linkVBO(VBO, 0);

	VAO.unBind();
	VBO.unBind();
	EBO.unBind();

	UBO UBO;
	Uniforms uni;


	// resize window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// materials
	const int materialsLength = 3;
	Material materials[materialsLength];
	materials[0].color = glm::vec3(0.5, 0.2, 0.8);
	materials[1].color = glm::vec3(0.8, 0.8, 0.8);
	materials[2].color = glm::vec3(0.5, 0.7, 0.2);


	// sphere objects
	const int spheresLength = 3;
	Sphere spheres[spheresLength];
	spheres[0].pos = glm::vec3(0, 0, 3);
	spheres[0].radius = 0.6;
	spheres[0].color = glm::vec3(0.5, 0.2, 0.8);

	
	spheres[1].pos = glm::vec3(1.5, 3, 1);
	spheres[1].radius = 0.4;
	spheres[1].color = glm::vec3(0.8, 0.8, 0.8);


	spheres[2].pos = glm::vec3(-2, 1.2, 0);
	spheres[2].radius = 0.8;
	spheres[2].color = glm::vec3(0.5, 0.7, 0.2);



	// delta time setup
	float currentTime, dt;
	float prevTime = glfwGetTime();
	srand(time(NULL));


	// main loop
	while (!glfwWindowShouldClose(window)) {

		// input first
		currentTime = glfwGetTime();
		dt = currentTime - prevTime;
		prevTime = currentTime;
		cam.updateDT(dt);
		input(window, dt);


		// position debug
		//std::cout << cam.pos.x << " " << cam.pos.y << " " << cam.pos.z << std::endl; //"			 dir " << cam.direction.x << " " << cam.direction.y << " " << cam.direction.z << std::endl;


		// render stuff
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();


		// uniforms
		uni.init(shader.program);
		uni.update(cam.pos, cam.model, width, height, cam.focus, spheresLength, rand());
		UBO.build(shader.program, spheres, spheresLength);


		VAO.bind();

		glDrawElements(GL_TRIANGLES, sizeof(indicies) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	VAO.deleteVAO();
	VBO.deleteVBO();
	EBO.deleteEBO();
	UBO.deleteUBO();
	shader.deleteShader();

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
}

// mouse movement
void mouse_position_callback(GLFWwindow* window, double xPos, double yPos) {
	float x = static_cast<float>(xPos);
	float y = static_cast<float>(yPos);

	float dx = x - prevX;
	float dy = y - prevY; 
	prevX = x;
	prevY = y;

	cam.mouseInput(dx, dy);
}

// keyboard
void input(GLFWwindow* window, float dt) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
		glfwSetWindowShouldClose(window, true);
	cam.input(window);
}

