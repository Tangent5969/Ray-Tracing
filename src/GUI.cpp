#include "headers/GUI.h"

GUI::GUI(GLFWwindow* window) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}


void GUI::mainLoop(GLuint texture, int& width, int& height, bool& lockedMovement, bool& changed, Camera& cam, float dt) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	NewFrame();
	//ShowDemoWindow();

	ImGuiWindowFlags windowFlags = lockedMovement ? 0 : ImGuiWindowFlags_NoMove;


	// main menu
	BeginMainMenuBar();
	int menuHeight = GetFrameHeight();
	if (MenuItem("Camera")) cameraFlag = !cameraFlag;
	if (MenuItem("Objects")) objectFlag = !objectFlag;
	if (MenuItem("Materials")) materialFlag = !materialFlag;
	if (MenuItem("Debug")) debugFlag = !debugFlag;
	EndMainMenuBar();

	ImVec2 windowSize = GetIO().DisplaySize;
	windowSize = ImVec2(windowSize.x, windowSize.y - menuHeight);

	// dock for nodes 
	PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	SetNextWindowSize(windowSize);
	SetNextWindowPos(ImVec2(0, menuHeight));
	Begin("Dock", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGuiID dockID = GetID("Dock");
	DockSpace(dockID, ImVec2(0.0f, 0.0f));

	End();

	// shader output
	if (Begin("View", NULL, windowFlags)) {
		ImVec2 viewSize = GetContentRegionAvail();
		width = viewSize[0];
		height = viewSize[1];
		Image((void*)(intptr_t)texture, ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
	}
	End();

	// camera controls
	if (cameraFlag) {
		if (Begin("Camera", &cameraFlag, windowFlags)) {

			ImGui::SeparatorText("Camera Position");
			if (DragFloat("Pos X", &cam.pos.x, 0.05)) {
				changed = true;
			}
			if (DragFloat("Pos Y", &cam.pos.y, 0.05)) {
				changed = true;
			}
			if (DragFloat("Pos Z", &cam.pos.z, 0.05)) {
				changed = true;
			}

			ImGui::SeparatorText("Camera Direction");
			if (SliderFloat("Pitch", &cam.rotation.y, -180, 180, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
				cam.updateModel();
				changed = true;
			}
			if (SliderFloat("Yaw", &cam.rotation.x, -180, 180, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
				cam.updateModel();
				changed = true;
			}
			if (SliderFloat("Roll", &cam.rotation.z, -180, 180, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
				cam.updateModel();
				changed = true;
			}

			ImGui::SeparatorText("Other");
			if (SliderFloat("Fov", &cam.fov, 1, 180, "%.0f", ImGuiSliderFlags_AlwaysClamp)) {
				cam.updateFov();
				changed = true;
			}
			if (DragFloat("Speed", &cam.moveSpeed, 0.05, 0.1, ImGuiSliderFlags_AlwaysClamp)) {
				if (cam.moveSpeed < 0.1f) cam.moveSpeed = 0.1f;
			}
			if (DragFloat("Sensitivity", &cam.sensitivity, 0.05, 0.1, ImGuiSliderFlags_AlwaysClamp)) {
				if (cam.sensitivity < 0.1f) cam.sensitivity = 0.1f;
			}
		}
		End();
	}

	// object customisation
	if (objectFlag) {
		if (Begin("Objects", &objectFlag, windowFlags)) {
			// add objects
			// remove objects
			// edit existing objects
		}
		End();
	}

	// material customisation
	if (materialFlag) {
		if (Begin("Materials", &materialFlag, windowFlags)) {
			// add materials
			// remove materials
			// edit existing materials
		}
		End();
	}

	// debug display
	if (debugFlag) {
		SetNextWindowBgAlpha(0.35f);
		Begin("Debug", NULL, windowFlags | ImGuiWindowFlags_NoTitleBar);
		Text("FPS : %.0f", 1.0f / dt);
		End();
	}


	PopStyleVar();
}

void GUI::render() {
	Render();
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}

void GUI::deleteGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	DestroyContext();
}