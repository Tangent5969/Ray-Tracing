#ifndef OBJECT_H
#define OBJECT_H

#include<glm/glm.hpp>

#define MAX_SPHERES 100

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
	Material mat; // 64
};


#endif