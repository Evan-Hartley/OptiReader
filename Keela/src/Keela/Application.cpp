#include "keepch.h"
#include "Application.h"

#include "Keela/Events/ApplicationEvent.h"
#include "Keela/Log.h"

namespace Keela {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		KEE_TRACE(e.ToString());

		while (true);
	}

}
