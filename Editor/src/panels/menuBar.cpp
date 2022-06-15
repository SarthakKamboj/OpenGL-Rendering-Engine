#include "menuBar.h"
#include "renderer/frameBuffer.h"
#include "glad/glad.h"
#include "imgui.h"
#include "panels/panelsManager.h"
#include "helper.h"

extern PanelsManager* g_PanelsManager;
extern int width, height;

void MenuBar::update() {
	FileBrowser& fileBrowser = g_PanelsManager->fileBrowser;
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open fbx file")) {
				fileBrowser.open = true;
				fileBrowser.resultBuffer = fbxToLoadPath;
				fileBrowser.loadMode = FileBrowserLoadMode::SCENE;
				memset(fileBrowser.curFolderPath, 0, 200);
				// later change default filebrowser path
				Helper::CopyBuffer("C:\\Sarthak\\programming\\3dFileLoader\\Editor\\assets\\3d", fileBrowser.curFolderPath, 200);
				selectingFbxToLoad = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (selectingFbxToLoad && !fileBrowser.open) {
		selectingFbxToLoad = false;
		g_PanelsManager->sceneList.loadSceneFromFbxFile(fbxToLoadPath);
	}
}