#ifndef GUI_H
#define GUI_H

#include<GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "headers/Camera.h"

using namespace ImGui;

class GUI {
public:
	GUI(GLFWwindow* window);
	void mainLoop(GLuint texture, int& width, int& height, bool lockedMovement, Camera& cam, float dt);
	void render();
	void deleteGUI();

private:
	bool cameraFlag = false;
	bool objectFlag = false;
	bool materialFlag = false;
	bool debugFlag = false;
};

#endif