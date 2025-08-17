#ifndef OBJECT_H
#define OBJECT_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<vector>

#define MAX_SPHERES 100
#define MAX_MATERIALS 100
#define MAX_MODELS 100

const glm::vec3 originalSide = glm::vec3(1, 0, 0);
const glm::vec3 originalUp = glm::vec3(0, 1, 0);
const glm::vec3 originalDirection = glm::vec3(0, 0, 1);

struct Material {
	glm::vec3 color = glm::vec3(1); // 12 
	float lightStrength = 0.0f; //4
	glm::vec3 lightColor = glm::vec3(0); // 12
	float smoothness = 0.0f; // 4
	float gloss = 0.0f; // 4
	float glass = false; // 4
	float refraction = 1.0f; // 4
	float pad; // 4

};


struct Sphere {
	glm::vec3 pos = glm::vec3(0); // 12
	float radius = 0.5f; // 4
	int matIndex = 0; // 4
	float pad1; // 4
	float pad2; // 4
	float pad3; // 4
};


struct Triangle {
	glm::vec3 posA; // 12
	float pad1; // 4
	glm::vec3 posB; // 12
	float pad2; // 4
	glm::vec3 posC; // 12
	float pad3; // 4
	glm::vec3 normA; // 12
	float pad4; // 4
	glm::vec3 normB; // 12
	float pad5; // 4
	glm::vec3 normC; // 12
	float pad6; // 4
};


// transform = translate * rotate (x * y * z) * scale

struct ModelExtra {
	glm::vec3 pos = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);
};

struct Model {
	glm::mat4 transform = glm::mat4(1); // 64
	glm::mat4 invTransform = glm::mat4(1); // 64
	int matIndex = 0; // 4
	int startIndex; // 4
	int endIndex; // 4
	int BVHIndex; // 4

	void setTransform(ModelExtra data) {
		glm::mat3 rotateX = glm::rotate(glm::mat4(1.0), glm::radians(-data.rotation.x), originalUp);
		glm::mat3 rotateY = glm::rotate(glm::mat4(1.0), glm::radians(-data.rotation.y), originalSide);
		glm::mat3 rotateZ = glm::rotate(glm::mat4(1.0), glm::radians(data.rotation.z), originalDirection);

		glm::mat4 translate = glm::translate(glm::mat4(1), data.pos);
		glm::mat4 rotate = rotateX * rotateY * rotateZ;
		glm::mat4 scale = glm::scale(glm::mat4(1), data.scale);
		transform = translate * rotate * scale;
		invTransform = glm::inverse(transform);
	}

	void setTransform(ModelExtra* data) {
		glm::mat3 rotateX = glm::rotate(glm::mat4(1.0), glm::radians(-data->rotation.x), originalUp);
		glm::mat3 rotateY = glm::rotate(glm::mat4(1.0), glm::radians(-data->rotation.y), originalSide);
		glm::mat3 rotateZ = glm::rotate(glm::mat4(1.0), glm::radians(data->rotation.z), originalDirection);

		glm::mat4 translate = glm::translate(glm::mat4(1), data->pos);
		glm::mat4 rotate = rotateX * rotateY * rotateZ;
		glm::mat4 scale = glm::scale(glm::mat4(1), data->scale);
		transform = translate * rotate * scale;
		invTransform = glm::inverse(transform);

	}
};

#endif