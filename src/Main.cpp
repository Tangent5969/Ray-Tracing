#include<iostream>

// opengl headers
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "headers/GUI.h"
#include "headers/Shader.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/Camera.h"
#include "headers/Object.h"
#include "headers/UBO.h"
#include "headers/Uniforms.h"
#include "headers/FBO.h"

// globals
int windowWidth = 1408;
int windowHeight = 792;
int width = 1280;
int height = 720;
int oldWidth = width;
int oldHeight = height;
float prevX = 0.0f;
float prevY = 0.0f;
bool moved = false;
bool lockedMovement = false;
Camera cam(width, height, 65.0f);

// detects for tab key just pressed
bool tabHeld = false;


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


	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "program", NULL, NULL);
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

	rayShader.use();
	glUniform1i(glGetUniformLocation(rayShader.program, "text"), 0);
	FBOShader.use();
	glUniform1i(glGetUniformLocation(FBOShader.program, "text"), 0);

	VAO VAO;
	VBO VBO(vertices, sizeof(vertices));

	VAO.bind();
	VAO.linkVBO(VBO, 0);
	VAO.linkVBO(VBO, 1);

	FBO FBO(width, height);
	FBO.unBind();

	UBO UBO;

	VAO.unBind();
	VBO.unBind();
	UBO.unBind();

	Uniforms uni;
	uni.init(rayShader.program);


	// materials
	Material materials[7];
	materials[0].color = glm::vec3(0.5, 0.2, 0.8);
	materials[0].lightStrength = 1;
	materials[0].lightColor = glm::vec3(0.5, 0.2, 0.8);
	materials[0].smoothness = 0;

	materials[1].color = glm::vec3(0.1, 0.8, 0.85);
	materials[1].lightStrength = 0;
	materials[1].lightColor = glm::vec3(0);
	materials[1].smoothness = 0.2;

	materials[2].color = glm::vec3(0.5, 0.7, 0.2);
	materials[2].lightStrength = 0;
	materials[2].lightColor = glm::vec3(0);
	materials[2].smoothness = 0.95;
	materials[2].gloss = 0.6;

	materials[3].color = glm::vec3(0.8, 0.25, 0.25);
	materials[3].lightStrength = 0;
	materials[3].lightColor = glm::vec3(0);
	materials[3].smoothness = 0;

	// star
	materials[4].color = glm::vec3(1, 0.9, 1);
	materials[4].lightStrength = 50;
	materials[4].lightColor = glm::vec3(1, 0.9, 1);
	materials[4].smoothness = 0;

	materials[5].smoothness = 0.8;
	materials[5].gloss = 1;

	materials[6].glass = true;
	materials[6].smoothness =0.9995f;
	materials[6].refraction = 1.01f;

	// sphere objects
	const int spheresLength = 6;
	Sphere spheres[spheresLength];
	spheres[0].pos = glm::vec3(0, 0, 3);
	spheres[0].radius = 0.6;
	spheres[0].mat = materials[0];

	spheres[1].pos = glm::vec3(10, -1.1, 20);
	spheres[1].radius = 5;
	spheres[1].mat = materials[5];

	spheres[2].pos = glm::vec3(2, -1, 0);
	spheres[2].radius = 0.8;
	spheres[2].mat = materials[2];

	spheres[3].pos = glm::vec3(0, -10, 0);
	spheres[3].radius = 9;
	spheres[3].mat = materials[3];

	// star
	spheres[4].pos = glm::vec3(350, 100, -200);
	spheres[4].radius = 50;
	spheres[4].mat = materials[4];

	spheres[5].pos = glm::vec3(-1, -0.4, 0);
	spheres[5].mat = materials[6];


	// initialize loop variables
	float currentTime, dt;
	float prevTime = glfwGetTime();
	int accumulationFrame = 0;

	FBO.bindTexture();

	// imgui
    GUI gui(window);


	// main loop
	while (!glfwWindowShouldClose(window)) {
		// input first
		currentTime = glfwGetTime();
		dt = currentTime - prevTime;
		prevTime = currentTime;
		cam.updateDT(dt);
		input(window, dt);


		if (width != oldWidth || height != oldHeight) {
			oldWidth = width;
			oldHeight = height;
			cam.updateRes(width, height);

			// resize FBO and texture
			FBO.bind();
			FBO.resize(width, height);
			FBO.unBind();
			FBO.bindTexture();

			// resize viewport
			glViewport(0, 0, width, height);

			accumulationFrame = 1;
		}

		if (moved) {
			moved = false;
			accumulationFrame = 1;
		}
		else {
			accumulationFrame++;
		}


		// position debug
		//std::cout << cam.pos.x << " " << cam.pos.y << " " << cam.pos.z << std::endl; //"			 dir " << cam.direction.x << " " << cam.direction.y << " " << cam.direction.z << std::endl;
		
		// fps debug
		std::cout << int (1 / dt) << std::endl;

		// viewport size debug
		//std::cout << width << " h " << height << std::endl;


		// render scene to FBO
		FBO.bind();
		rayShader.use();

		// uniforms
		uni.update(cam.pos, cam.model, width, height, cam.focus, spheresLength, accumulationFrame);
		UBO.bind();
		UBO.build(rayShader.program, spheres, spheresLength);

		VAO.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// gui logic and image
		gui.mainLoop(FBO.texture, width, height, lockedMovement, cam, dt);

		FBO.unBind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// imgui
		gui.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO.deleteVAO();
	VBO.deleteVBO();
	FBO.deleteFBO();
	UBO.deleteUBO();
	rayShader.deleteShader();
	FBOShader.deleteShader();

	// imgui
	gui.deleteGUI();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


// resize window
void framebuffer_size_callback(GLFWwindow* window, int x, int y) {
	windowWidth = x;
	windowHeight = y;
}

// mouse movement
void mouse_position_callback(GLFWwindow* window, double xPos, double yPos) {
	float x = static_cast<float>(xPos);
	float y = static_cast<float>(yPos);

	float dx = x - prevX;
	float dy = y - prevY; 
	prevX = x;
	prevY = y;

	if (!lockedMovement) {
		cam.mouseInput(dx, dy, true);
		moved = true;
	}
}

// keyboard
void input(GLFWwindow* window, float dt) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
		glfwSetWindowShouldClose(window, true);

	// emable or disable movement
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		if (!tabHeld) {
			lockedMovement = !lockedMovement;
			if (lockedMovement) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		tabHeld = true;
	} 
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) tabHeld = false;

	if (!lockedMovement && cam.input(window)) {
		moved = true;
	}
}
