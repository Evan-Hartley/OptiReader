#pragma once

#include "Core.h"
#include "Keela/Events/Event.h"
#include "Keela/Events/ApplicationEvent.h"

#include "Window.h"
#include "Keela/LayerStack.h"

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

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

		//To be defined in CLIENT
		Application* CreateApplication();

}

