// ◦ Xyz ◦

#include "ImGuiManage.h"
#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "../Help/FileManager.h"
#include "../Help/Log.h"

namespace Window {
	void Init(void* window) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		const std::string fontFileNamePath = TO_STRING("{}/font.ttf", help::FileManager::GetFullFileNameDir());
		if (std::filesystem::exists(fontFileNamePath)) {
			ImGuiIO& io = ImGui::GetIO();
			if (ImFont* newFont = io.Fonts->AddFontFromFileTTF(fontFileNamePath.c_str(), 16.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic())) {
				io.FontDefault = newFont;
			}
			else {
				LOG("[Window::Init] error load font '{}'.", fontFileNamePath);
			}
		}
		else {
			LOG("[Window::Init] font '{}' not founded.", fontFileNamePath);
		}

		// Тема, светлая и тёмная
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsLight();

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window), true);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void Cleanup() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void RenderPush() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void RenderPop() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
