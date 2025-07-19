#ifndef OBJECT_H
#define OBJECT_H

#include<glm/glm.hpp>

#define MAX_SPHERES 100

struct Material {
	glm::vec3 color; // 12 
	float lightStrength; //4
	glm::vec3 lightColor; // 12
	float pad; // 4
};

struct Sphere {
	glm::vec3 pos; // 12
	float radius; // 4
	Material mat; // 48
};


#endif