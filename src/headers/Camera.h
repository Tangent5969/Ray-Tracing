#ifndef CAMERA_H
#define CAMERA_H

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>


class Camera {
public:
	float focus;
	glm::mat4 model;
	glm::vec3 pos;

	Camera(int width, int height, float fov);
	void updateRes(int width, int height);
	void updateFov(float fov);
	void input(GLFWwindow* window);
	void mouseInput(GLFWwindow* window, float dx, float dy);
	void updateDT(float dt);

private:
	float dt;
	float fov;
	int width;
	int height;
	glm::vec3 side;
	glm::vec3 up;
	glm::vec3 rotation;
	glm::vec3 direction;

};

#endif
