#ifndef GUI_H
#define GUI_H

#include<stb/stb_image_write.h>
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector>
#include<string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "headers/Camera.h"
#include "headers/Object.h"

using namespace ImGui;

class GUI {
public:
	GUI(GLFWwindow* window);
	void mainLoop(GLuint texture, int& width, int& height, bool& lockedMovement, bool& renderFlag, bool& changed, Camera& cam, float dt, int accumulationFrame, std::vector<Material>& materials, std::vector<Sphere>& spheres, int& rayCount, int& maxBounces, float& environmentLight);
	void render();
	void deleteGUI();

private:
	bool renderSettingsFlag = false;
	bool cameraFlag = false;
	bool objectFlag = false;
	bool materialFlag = false;
	bool debugFlag = false;
	bool newMaterialFlag = false;
	bool newSphereFlag = false;
	bool matchResolutionFlag = true;

	// render settings
	int renderFrames = 1000;
	int renderRays = 5;
	int renderBounces = 500;
	int renderWidth = 1920;
	int renderHeight = 1080;
	glm::vec3 renderCamPos = glm::vec3(0);
	glm::vec3 renderCamRot = glm::vec3(0);
	float renderCamFov = 65;
	bool renderCamFlag = false;

	// backup viewport settings
	int viewRays;
	int viewBounces;
	int viewWidth;
	int viewHeight;
	glm::vec3 viewCamPos;
	glm::vec3 viewCamRot;
	float viewCamFov;

	float scale = 1;
	Material tempMaterial;
	Sphere tempSphere;

	bool editMaterial(Material* mat);
	bool editSphere(Sphere* sphere, std::vector<Material> materials);
	void startRender(int& width, int& height, bool& lockedMovement, bool& renderFlag, bool& changed, Camera& cam, int& rayCount, int& maxBounces);

};

static void saveImage(const char* fileName, int width, int height) {
	void* data = malloc(width * height * 3);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(true);
	stbi_write_png(fileName, width, height, 3, data, width * 3);
	free(data);
}

static int GCD(int x, int y) {
	int z;
	while (y != 0) {
		z = y;
		y = x % y;
		x = z;
	}
	return x;
}

#endif