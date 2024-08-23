#pragma once

#include "Core.h"
#include "Keela/Events/Event.h"
#include "Keela/Events/ApplicationEvent.h"

#include "Window.h"
#include "Keela/LayerStack.h"

#include "Keela/ImGui/ImGuiLayer.h"

namespace Keela {

	class KEELA_API Application
	{

	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);

		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

		//To be defined in CLIENT
		Application* CreateApplication();

}

