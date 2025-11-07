#include "src/Core/Application.h"

namespace XEngine {

	Application::Application(ApplicationSpecification& spec) {

		WindowSpecification win_spec;
		win_spec.width = spec.window_width;
		win_spec.height = spec.window_heigt;
		win_spec.wantVsync = spec.enableVsync;
		win_spec.title = spec.window_name;
		win_spec.platform = spec.platform;

		m_Window = Window::CreateWindow(win_spec);
	}

	void Application::Run() {
		while (true)
		{
			m_Window->OnUpdate();
		}
	}

}