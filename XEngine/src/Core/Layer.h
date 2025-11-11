#pragma once

#include <string>
#include "src/Core/Events/Event.h"


namespace XEngine {

	class Layer
	{
	public:
		Layer(std::string& name);
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnEnable() {};
		virtual void OnDisable() {};
		virtual void OnEvent(Event& event) {};

		const std::string& GetName() const { return m_Name; }

	private:
		std::string m_Name;
	};

}