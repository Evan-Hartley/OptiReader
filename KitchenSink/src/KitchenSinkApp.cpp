#include <Keela.h>

class ExampleLayer : public Keela::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		//KEE_INFO("ExampleLayer::Update");

	}

	void OnEvent(Keela::Event& event) override
	{
		if (event.GetEventType() == Keela::EventType::KeyPressed)
		{
			Keela::KeyPressedEvent& e = (Keela::KeyPressedEvent&)event;
			if (e.GetKeyCode() == KEE_KEY_TAB)
			{
				KEE_TRACE("Tab key is pressed!");
			}
			KEE_TRACE("{Tab key is pressed!}0}", (char)e.GetKeyCode());
		}
	}

};

class KitchenSink : public Keela::Application
{
public:
	KitchenSink()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Keela::ImGuiLayer());
	}

	~KitchenSink()
	{

	}
};

Keela::Application* Keela::CreateApplication()
{
	return new KitchenSink();
}