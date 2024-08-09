#pragma once

//Define start to program

#ifdef KEE_PLATFORM_WINDOWS

extern Keela::Application* Keela::CreateApplication();

int main(int argc, char** argv)
{

	Keela::Log::Init();
	KEE_CORE_WARN("Initialized Log!");
	int a = 5;
	KEE_INFO("Hello! Var={0}", a);

	auto app = Keela::CreateApplication();
	app->Run();
	delete app;
}

#endif

