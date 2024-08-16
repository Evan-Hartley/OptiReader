#pragma once

#include "Keela/Core.h"
#include "Layer.h"

#include "keepch.h"

// The layer stack is not an actual stack, it is a wrapper over the vector 
// (every frame we will iterate over it) of layers
namespace Keela {
	class KEELA_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}

