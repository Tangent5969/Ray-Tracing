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


void GUI::mainLoop(GLuint texture, int& width, int& height, bool& lockedMovement, bool& changed, Camera& cam, float dt, std::vector<Material>& materials, std::vector<Sphere>& spheres) {
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

	// material customisation
	if (materialFlag) {
		if (Begin("Materials", &materialFlag, windowFlags)) {

			// tree buttons
			bool openAllMat = false, closeAllMat = false;
			if (Button("Expand All")) {
				openAllMat = true;
			}
			SameLine();
			if (Button("Close All")) {
				closeAllMat = true;
			}

			// add new material
			if (newMaterialFlag) {
				Begin("New Material", &newMaterialFlag, windowFlags | ImGuiWindowFlags_NoDocking);
				editMaterial(&tempMaterial);
				Separator();
				if (Button("Add Material")) {
					materials.push_back(tempMaterial);
				}
				End();
			}
			else {
				if (Button("New Material")) {
					tempMaterial = Material{};
					newMaterialFlag = true;
				}
			}

			// list all materials
			for (int i = 0; i < materials.size(); i++) {
				if (openAllMat) SetNextItemOpen(true);
				if (closeAllMat) SetNextItemOpen(false);

				if (TreeNodeEx(("Material " + std::to_string(i)).c_str())) {
					Material* mat = &materials[i];
					changed = editMaterial(mat);
					TreePop();
				}
			}
		}
		End();
	}

	// object customisation
	if (objectFlag) {
		if (Begin("Objects", &objectFlag, windowFlags)) {

			// tree buttons
			bool openAllSphere = false, closeAllSphere = false;
			if (Button("Expand All##1")) {
				openAllSphere = true;
			}
			SameLine();
			if (Button("Close All##1")) {
				closeAllSphere = true;
			}

			// add new sphere
			if (newSphereFlag) {
				Begin("New Sphere", &newSphereFlag, windowFlags | ImGuiWindowFlags_NoDocking);
				editSphere(&tempSphere, materials);
				Separator();
				if (Button("Add Sphere")) {
					spheres.push_back(tempSphere);
					changed = true;
				}
				End();
			}
			else {
				if (Button("New Sphere")) {
					tempSphere = Sphere{};
					newSphereFlag = true;
				}
				SameLine();
			}

			// delete all spheres
			if (!spheres.empty()) {
				if (Button("Delete All")) {
					OpenPopup("Delete?");
				}

				// pop up
				SetNextWindowPos(GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				if (BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					Text("Delete All Spheres?");
					Separator();
					if (Button("Confirm")) {
						spheres.clear();
						changed = true;
						CloseCurrentPopup();
					}
					SameLine();
					if (Button("Cancel")) {
						CloseCurrentPopup();
					}
					EndPopup();
				}
			}


			// list all spheres
			for (int i = 0; i < spheres.size(); i++) {
				if (openAllSphere) SetNextItemOpen(true);
				if (closeAllSphere) SetNextItemOpen(false);

				if (TreeNodeEx(("Sphere " + std::to_string(i)).c_str())) {
					Sphere* sphere = &spheres[i];
					changed = editSphere(sphere, materials);

					// delete spheres
					ImGui::SeparatorText("Delete Sphere");
					if (Button(("Delete##" + std::to_string(i)).c_str())) {
						spheres.erase(spheres.begin() + i);
						changed = true;
					}
					TreePop();
				}
			}
		}
		End();
	}

	// debug display
	if (debugFlag) {
		SetNextWindowBgAlpha(0.35f);
		Begin("Debug", NULL, windowFlags | ImGuiWindowFlags_NoTitleBar);
		Text(" %.0f fps", 1.0f / dt);
		Text(" %.2f ms", dt * 1000);
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


bool GUI::editMaterial(Material* mat) {
	bool changed = false;

	// object color
	float color[]{ mat->color.x, mat->color.y, mat->color.z };
	if (ColorEdit3("Colour", color)) {
		mat->color = glm::vec3(color[0], color[1], color[2]);
		changed = true;
	}

	// light
	float lightColor[]{ mat->lightColor.x, mat->lightColor.y, mat->lightColor.z };
	if (ColorEdit3("Light Colour", lightColor)) {
		mat->lightColor = glm::vec3(lightColor[0], lightColor[1], lightColor[2]);
		changed = true;
	}

	if (DragFloat("Light Strength", &mat->lightStrength, 0.05, 0, ImGuiSliderFlags_AlwaysClamp)) {
		if (mat->lightStrength < 0) mat->lightStrength = 0;
		changed = true;
	}

	// glass
	bool glass = mat->glass;
	if (Checkbox("Glass", &glass)) {
		mat->glass = glass;
		changed = true;
	}
	if (glass) {
		if (DragFloat("Refraction", &mat->refraction, 0.001, 0, ImGuiSliderFlags_AlwaysClamp)) {
			if (mat->refraction < 0) mat->refraction = 0;
			changed = true;
		}
	}

	// other properties
	else {
		if (DragFloat("Gloss", &mat->gloss, 0.001, 0, 1, "%f", ImGuiSliderFlags_AlwaysClamp)) {
			if (mat->gloss < 0) mat->gloss = 0;
			changed = true;
		}
	}

	if (DragFloat("Smoothness", &mat->smoothness, 0.001, 0, 1, "%f", ImGuiSliderFlags_AlwaysClamp)) {
		if (mat->smoothness < 0) mat->smoothness = 0;
		changed = true;
	}

	return changed;
}

bool GUI::editSphere(Sphere* sphere, std::vector<Material> materials) {
	bool changed = false;

	ImGui::SeparatorText("Sphere Position");
	if (DragFloat("Pos X", &sphere->pos.x, 0.05)) {
		changed = true;
	}
	if (DragFloat("Pos Y", &sphere->pos.y, 0.05)) {
		changed = true;
	}
	if (DragFloat("Pos Z", &sphere->pos.z, 0.05)) {
		changed = true;
	}

	ImGui::SeparatorText("Other");
	if (DragFloat("Radius", &sphere->radius, 0.01, 0.01)) {
		if (sphere->radius < 0.01) sphere->radius = 0.01;
		changed = true;
	}

	if (SliderInt("Material", &sphere->matIndex, 0, materials.size() - 1, "%i", ImGuiSliderFlags_AlwaysClamp)) {
		changed = true;
	}

	return changed;
}