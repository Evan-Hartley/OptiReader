#include <Keela.h>

class KitchenSink : public Keela::Application
{
public:
	KitchenSink()
	{

	}

	~KitchenSink()
	{

	}
};

Keela::Application* Keela::CreateApplication()
{
	return new KitchenSink();
}