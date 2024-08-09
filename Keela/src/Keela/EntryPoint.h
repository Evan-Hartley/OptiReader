#pragma once

//Define start to program

#ifdef KEE_PLATFORM_WINDOWS

extern Keela::Application* Keela::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Keela::CreateApplication();
	app->Run();
	delete app;
}

#endif

