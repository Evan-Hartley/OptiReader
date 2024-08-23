#pragma once

#include "Keela/Layer.h"
#include "Keela/Events/Event.h"
#include "Keela/Events/ApplicationEvent.h"
#include "Keela/Events/KeyEvent.h"
#include "Keela/Events/MouseEvent.h"

namespace Keela {
	class KEELA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;

	};
}

