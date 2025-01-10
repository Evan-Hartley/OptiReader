#include "keepch.h"
#include "ImGuiLayer.h"
#include "ControlWindow.h"

#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"

#define IMGUI_IMPL_API
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Keela/Application.h"

//Temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Keela {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
		
	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		//Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;


		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	//Enable Keyboard controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	//Enable Gamepad controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		//Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		//Enable Viewports
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImPlot::SetImGuiContext(ImGui::GetCurrentContext());

		ImPlot::CreateContext();

		// Setup Dear ImGui Style
		ImGui::StyleColorsLight();

		//When viewpoerts are enabled we tweak WindowRounding/WindowBg so platform windows ca look identical to each other
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 4.0f;
			style.Colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f); // Base Text is off black
			style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f); // Internal Windows Are White
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.75f, 0.45f, 0.27f, 1.00f); // Menu Bar for Internal Windows is Brown Rust
			style.Colors[ImGuiCol_Border] = ImVec4(0.49f, 0.02f, 0.02f, 1.00f); // Border for Internal Windows is Dark Burgundy
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); //Boarder for Internal Windows has no shadow
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.98f, 0.94f, 0.8f, 1.00f); // Variable Framing Is Champagne
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.75f, 0.45f, 0.27f, 1.00f); // Variable Framing Is Brown Rust When Hovering
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.94f, 0.8f, 1.00f); // Variable Framing Is Champagne When Selected
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.98f, 0.88f, 0.51f, 1.00f); // Title Bar is Sweet Corn
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.98f, 0.88f, 0.51f, 1.00f); // Title Bar is Seet Corn
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.62f, 0.80f, 0.15f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.64f, 0.80f, 0.30f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.67f, 0.80f, 0.59f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.48f, 0.53f, 0.67f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.48f, 0.47f, 0.47f, 0.71f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.47f, 0.99f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.79f, 0.18f, 0.78f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.42f, 0.82f, 1.00f, 0.81f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 1.00f, 1.00f, 0.86f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.49f, 0.02f, 0.02f, 1.0f); // Header is Dark Burgandy
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.45f, 0.27f, 1.0f); // Header is Brown Rust When Hovered
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.49f, 0.02f, 0.02f, 1.0f); // Header is Dark Burgandy When Active
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
			//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.98f, 0.94f, 0.8f, 0.99f); // Color for Dropdown Selections is Champagne
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.88f, 0.51f, 0.8f); // Selected Text is Sweet Corn
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.94f, 0.8f, 0.35f); // Text Select is Champagne
			style.Alpha = 1.0f;
			style.FrameRounding = 4;
			style.IndentSpacing = 12.0f;


		}

		// Add a new font with the desired size, e.g., 18.0f
		io.Fonts->AddFontFromFileTTF("BrassMonoBold.ttf", 24.0f);

		// Ensure the font atlas is built with the new font size
		io.Fonts->Build();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		// Begin a new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();

		Application& app = Application::Get();

		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
		
		// Rendering
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowControlWindow(&show);
	}
}
