#include "headers/Object.h"

std::vector<Material> getMaterials() {
	std::vector<Material> materials;

	// mat 0
	materials.push_back(Material{});
	materials[0].color = glm::vec3(0.5, 0.2, 0.8);
	materials[0].lightStrength = 1;
	materials[0].lightColor = glm::vec3(0.5, 0.2, 0.8);
	materials[0].smoothness = 0;
	

	// mat 1
	materials.push_back(Material{});
	materials[1].color = glm::vec3(0.1, 0.8, 0.85);
	materials[1].lightStrength = 0;
	materials[1].lightColor = glm::vec3(0);
	materials[1].smoothness = 0.2;

	// mat 2
	materials.push_back(Material{});
	materials[2].color = glm::vec3(0.5, 0.7, 0.2);
	materials[2].lightStrength = 0;
	materials[2].lightColor = glm::vec3(0);
	materials[2].smoothness = 0.95;
	materials[2].gloss = 0.6;

	// mat 3
	materials.push_back(Material{});
	materials[3].color = glm::vec3(0.8, 0.25, 0.25);
	materials[3].lightStrength = 0;
	materials[3].lightColor = glm::vec3(0);
	materials[3].smoothness = 0;

	// star
	// mat 4
	materials.push_back(Material{});
	materials[4].color = glm::vec3(1, 0.9, 1);
	materials[4].lightStrength = 50;
	materials[4].lightColor = glm::vec3(1, 0.9, 1);
	materials[4].smoothness = 0;

	// mat 5
	materials.push_back(Material{});
	materials[5].smoothness = 0.8;
	materials[5].gloss = 1;

	// mat 6
	materials.push_back(Material{});
	materials[6].glass = true;
	materials[6].smoothness = 0.9995f;
	materials[6].refraction = 1.01f;

	return materials;
}


std::vector<Sphere> getSpheres() {
	std::vector<Material> materials = getMaterials();

	std::vector<Sphere> spheres;
	Sphere sphere;

	// sphere 0
	sphere = Sphere{};
	sphere.pos = glm::vec3(0, 0, 3);
	sphere.radius = 0.6;
	sphere.matIndex = 0;
	spheres.push_back(sphere);

	// sphere 1
	sphere = Sphere{};
	sphere.pos = glm::vec3(10, -1.1, 20);
	sphere.radius = 5;
	sphere.matIndex = 5;
	spheres.push_back(sphere);

	// sphere 2
	sphere = Sphere{};
	sphere.pos = glm::vec3(2, -1, 0);
	sphere.radius = 0.8;
	sphere.matIndex = 2;
	spheres.push_back(sphere);

	// sphere 3
	sphere = Sphere{};
	sphere.pos = glm::vec3(0, -10, 0);
	sphere.radius = 9;
	sphere.matIndex = 3;
	spheres.push_back(sphere);

	// star
	// sphere 4
	sphere = Sphere{};
	sphere.pos = glm::vec3(350, 100, -200);
	sphere.radius = 50;
	sphere.matIndex = 4;
	spheres.push_back(sphere);

	// sphere 5
	sphere = Sphere{};
	sphere.pos = glm::vec3(-1, -0.4, 0);
	sphere.matIndex = 6;
	spheres.push_back(sphere);

	return spheres;
}

