#pragma once

#include "src/Window/Window.h"
#include "src/Core/Layer.h"
#include "src/Core/LayerStack.h"


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

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application* GetHandle() { return s_Instance; }
		const std::unique_ptr<Window>& GetWindow() const { return m_Window; }

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	extern Application* CreateApplication();

}