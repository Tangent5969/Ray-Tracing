#include "headers/Camera.h"

Camera::Camera(int width, int height, float fov) {
	this->dt = 0;
	this->fov = fov;
	this->width = width;
	this->height = height;

	focus = width / (2 * glm::tan(fov / 2));
	pos = glm::vec3(0, 0, -1);
	direction = originalDirection;
	up = originalUp;
	side = originalSide;
	model = glm::mat4(1);

}

void Camera::updateRes(int width, int height) {
	this->width = width;
	this->height = height;
	focus = width / (2 * glm::tan(fov / 2));
}

void Camera::updateFov(float fov) {
	this->fov = fov;
	focus = width / (2 * glm::tan(fov / 2));
}

void Camera::input(GLFWwindow* window) {
	float speed = 1.0f * dt;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pos += speed * direction;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pos -= speed * direction;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pos += speed * side;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pos -= speed * side;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		pos += speed * up;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		pos -= speed * up;
}

void Camera::mouseInput(float dx, float dy) {
	rotation.x += dx * dt * sensitivity;
	rotation.y += dy * dt * sensitivity;

	glm::mat3 rotateX = glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), originalUp);
	glm::mat3 rotateY = glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), originalSide);
	glm::mat3 finalRotate = rotateX * rotateY;

	model = finalRotate * glm::mat3(originalSide, originalUp, originalDirection);
	side = model[0];
	up = model[1];
	direction = model[2];
}


void Camera::updateDT(float dt) {
	this->dt = dt;
}