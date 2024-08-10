#include "keepch.h"
#include "Application.h"

#include "Keela/Events/ApplicationEvent.h"
#include "Keela/Log.h"

namespace Keela {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

}
