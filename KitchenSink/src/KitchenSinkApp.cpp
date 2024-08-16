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
		KEE_INFO("ExampleLayer::Update");
	}

	void OnEvent(Keela::Event& event) override
	{
		KEE_TRACE("{0}", event);
	}

};

class KitchenSink : public Keela::Application
{
public:
	KitchenSink()
	{
		PushLayer(new ExampleLayer());
	}

	~KitchenSink()
	{

	}
};

Keela::Application* Keela::CreateApplication()
{
	return new KitchenSink();
}