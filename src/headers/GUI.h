#ifndef GUI_H
#define GUI_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class GUI {
public:
	void init(GLFWwindow* window);
	void mainLoop();
	void render();
	void deleteGUI();
};

#endif