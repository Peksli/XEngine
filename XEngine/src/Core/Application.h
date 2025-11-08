#pragma once

#include "src/Window/Window.h"


namespace XEngine {

	struct ApplicationSpecification {
		Platform platform;
		std::string window_name;
		int window_width;
		int window_heigt;
		bool enableVsync;
	};

	class Application {
	public:
		Application(ApplicationSpecification& spec);
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	extern Application* CreateApplication();

}