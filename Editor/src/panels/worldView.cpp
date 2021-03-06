#include "worldView.h"
#include "imgui.h"
#include "glm/glm.hpp"
#include "sceneRenderer.h"
#include "window.h"

extern SceneRenderer* g_SceneRenderer;

WorldView::WorldView() {
	const char* imgPath = "C:\\Sarthak\\programming\\3dFileLoader\\Editor\\src\\images\\suzanneTexture.png";
	testTexture = Texture(imgPath, 0);
}

void WorldView::update() {

	ImGuiWindowFlags worldViewWinFlags = ImGuiWindowFlags_None;

	ImGuiViewport* mainViewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowViewport(mainViewport->ID);

	ImVec2 sceneViewWinPadding(5.0f, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, sceneViewWinPadding);

	float fontSize = 16.0f;
	ImGui::Begin("World view window", NULL, worldViewWinFlags);
	{
		ImVec2 winSize = ImGui::GetWindowSize();
		ImVec2 actualRenderView(winSize.x - sceneViewWinPadding.x, winSize.y - (2.0f * fontSize) - (2 * sceneViewWinPadding.y));
		// render color texture from scene framebuffer
		// g_SceneRenderer->sceneFbo.bind();
		// FrameBuffer::ClearBuffers(glm::vec3(1, 0, 0));
		// g_SceneRenderer->sceneFbo.unbind();
		ImGui::Image((ImTextureID)g_SceneRenderer->sceneFbo.colorTexture, actualRenderView, ImVec2(0, 1), ImVec2(1, 0));
		// ImGui::Image((ImTextureID)testTexture.texture, actualRenderView, ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::End();
	ImGui::PopStyleVar();


}
