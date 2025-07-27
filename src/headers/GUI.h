#ifndef GUI_H
#define GUI_H

#include<GLFW/glfw3.h>
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
	void mainLoop(GLuint texture, int& width, int& height, bool& lockedMovement, bool& changed, Camera& cam, float dt, std::vector<Material>& materials, std::vector<Sphere>& spheres);
	void render();
	void deleteGUI();

private:
	bool cameraFlag = false;
	bool objectFlag = false;
	bool materialFlag = false;
	bool debugFlag = false;
	bool newMaterialFlag = false;
	bool newSphereFlag = false;


	Material tempMaterial;
	Sphere tempSphere;

	bool editMaterial(Material* mat);
	bool editSphere(Sphere* sphere, std::vector<Material> materials);
};

#endif