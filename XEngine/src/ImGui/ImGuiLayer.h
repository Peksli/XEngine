#pragma once

#include "src/Core/Layer.h"


namespace XEngine {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(std::string name);
		virtual ~ImGuiLayer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate();
		virtual void OnEnable();
		virtual void OnDisable();
		virtual void OnEvent(Event& event);
	};

}