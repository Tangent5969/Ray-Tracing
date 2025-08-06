#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include<nfd/nfd.h>
#include<windows.h>
#include<string>
#include<vector>
#include <fstream>
#include "headers/Object.h"

#include <iostream>


static void saveImage(std::string filePath, int width, int height) {
	void* data = malloc(width * height * 3);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(true);
	stbi_write_png(filePath.c_str(), width, height, 3, data, width * 3);
	free(data);

	// opens image in default program if windows
#ifdef _WIN32
	ShellExecuteA(NULL, "open", filePath.c_str(), NULL, NULL, SW_SHOW);
#endif
}


static std::string getSavePath(std::string defaultPath, int mode) {
	NFD_Init();
	nfdchar_t* path;
	nfdsavedialogu8args_t args = {0};

	switch (mode) {
		// image
	case 0: {
		nfdfilteritem_t filterList[1] = {{"image", "png"}};
		args.filterList = filterList;
		args.filterCount = 1;
		break;
		}
		
		// scene
	case 1: {
		nfdfilteritem_t filterList[1] = {{"scene", "ray"}};
		args.filterList = filterList;
		args.filterCount = 1;
		break;
		}
	}
	
	args.defaultName = defaultPath.c_str();

	nfdresult_t result = NFD_SaveDialogU8_With(&path, &args);
	if (result == NFD_OKAY) {
		defaultPath = path;
		NFD_FreePath(path);
	}
	// closed or error
	else defaultPath = "";

	NFD_Quit();
	return defaultPath;
}


static std::string getLoadPath() {
	NFD_Init();
	nfdchar_t* path;
	std::string loadPath;

	nfdopendialogu8args_t args = {0};
	nfdfilteritem_t filterList[1] = {{"scene", "ray"}};
	args.filterList = filterList;
	args.filterCount = 1;

	nfdresult_t result = NFD_OpenDialogU8_With(&path, &args);
	if (result == NFD_OKAY) {
		loadPath = path;
		NFD_FreePath(path);
	}
	// closed or error
	else loadPath = "";

	NFD_Quit();
	return loadPath;
}


static bool saveScene(std::string filePath, std::vector<Material> materials, std::vector<Sphere> spheres) {
	std::ofstream file;
	file.open(filePath);

	if (!file.is_open()) return false;

	file << "Materials: " << materials.size() << "\n";
	file << "Spheres: " << spheres.size() << "\n\n";

	for (int i = 0; i < materials.size(); i++) {
		Material mat = materials[i];
		file << "Material: " << i << "\n";
		file << "Colour: " << mat.color.x << " " << mat.color.y << " " << mat.color.z << "\n";
		file << "Light Strength: " << mat.lightStrength << "\n";
		file << "Light Colour: " << mat.lightColor.x << " " << mat.lightColor.y << " " << mat.lightColor.z << "\n";
		file << "Smoothness: " << mat.smoothness << "\n";
		file << "Gloss: " << mat.gloss << "\n";
		file << "Glass: " << mat.glass << "\n";
		file << "Refraction: " << mat.refraction << "\n\n";
	}

	for (int i = 0; i < spheres.size(); i++) {
		Sphere sphere = spheres[i];
		file << "Sphere: " << i << "\n";
		file << "Position: " << sphere.pos.x << " " << sphere.pos.y << " " << sphere.pos.z << "\n";
		file << "Radius: " << sphere.radius << "\n";
		file << "Material: " << sphere.matIndex << "\n\n";
	}

	file.close();
	return true;
}

static bool loadScene(std::string filePath, std::vector<Material>& materials, std::vector<Sphere>& spheres) {
	std::ifstream file;
	file.open(filePath);
	if (!file.is_open()) return false;
	std::string line;

	std::vector<Material> fileMaterials;
	std::vector<Sphere> fileSpheres;

	int matSize;
	int spheresSize;

	file >> line; // materials
	file >> matSize; // int
	file >> line; // spheres
	file >> spheresSize; // int


	for (int i = 0; i < matSize; i++) {
		Material mat;
		file >> line; // material
		file >> line; // material num
		file >> line; // colour
		file >> mat.color.x >> mat.color.y >> mat.color.z; // floats
		file >> line; // light
		file >> line; // strength
		file >> mat.lightStrength; // float
		file >> line; // light
		file >> line; // colour
		file >> mat.lightColor.x >> mat.lightColor.y >> mat.lightColor.z; // floats
		file >> line; //smoothness
		file >> mat.smoothness; // float
		file >> line; // gloss
		file >> mat.gloss; // float
		file >> line; // glass
		file >> mat.glass; // float
		file >> line; // refraction
		file >> mat.refraction; // float
		fileMaterials.push_back(mat);
	}
	
	for (int i = 0; i < spheresSize; i++) {
		Sphere sphere;
		file >> line; // sphere
		file >> line; // sphere num
		file >> line; // position
		file >> sphere.pos.x >> sphere.pos.y >> sphere.pos.z; // floats
		file >> line; // radius
		file >> sphere.radius; // float
		file >> line; // material
		file >> sphere.matIndex; // int
		fileSpheres.push_back(sphere);
	}

	file.close();

	if (fileMaterials.empty()) fileMaterials.push_back(Material{});

	materials = fileMaterials;
	spheres = fileSpheres;
	return true;
}

#endif