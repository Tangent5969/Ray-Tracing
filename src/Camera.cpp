#include "headers/Camera.h"

Camera::Camera(int width, int height, float fov) {

	this->dt = 0;
	this->fov = fov;
	this->width = width;
	this->height = height;


	focus = width / (2 * glm::tan(fov / 2));
	pos = glm::vec3(0, 0, -1);
	direction = glm::vec3(0.0f, 0.0f, 1.0f);
	up = glm::vec3(0, 1, 0);
	side = glm::normalize(glm::cross(direction, up));
	rotation = glm::vec3(0);
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
	rotation.x -= dx * dt * sensitivity;
	rotation.y -= dy * dt * sensitivity;

	glm::mat3 rotateX = glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), up);
	glm::mat3 rotateY = glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), side);
	glm::mat3 finalRotate = rotateX * rotateY;

	model = finalRotate * glm::mat3(side, up, direction);
}


void Camera::updateDT(float dt) {
	this->dt = dt;
}