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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

	// resize window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	float currentTime, dt;
	float prevTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {

		// input first
		currentTime = glfwGetTime();
		dt = currentTime - prevTime;
		prevTime = currentTime;
		input(window, dt);
		cam.updateModel();

		// position debug
		std::cout << cam.pos.x << " " << cam.pos.y << " " << cam.pos.z << std::endl; //"			 dir " << cam.direction.x << " " << cam.direction.y << " " << cam.direction.z << std::endl;


		// render stuff
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();


		// uniforms
		int camPosLocation = glGetUniformLocation(shader.program, "camPos");
		glUniform3fv(camPosLocation, 1, glm::value_ptr(cam.pos));
		int modelLocation = glGetUniformLocation(shader.program, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam.model));
		int resLocation = glGetUniformLocation(shader.program, "res");
		glUniform2f(resLocation, width, height);
		int focusLocation = glGetUniformLocation(shader.program, "focus");
		glUniform1f(focusLocation, cam.focus);
	

		VAO.bind();

		glDrawElements(GL_TRIANGLES, sizeof(indicies) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	VAO.deleteVAO();
	VBO.deleteVBO();
	EBO.deleteEBO();
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

	cam.mouseInput(window, dx, dy);

}

// keyboard
void input(GLFWwindow* window, float dt) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
		glfwSetWindowShouldClose(window, true);
	cam.input(window, dt);
}

