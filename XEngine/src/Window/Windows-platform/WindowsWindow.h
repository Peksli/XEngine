#pragma once

#include <glfw/glfw3.h>
#include "src/Window/Window.h"


namespace XEngine {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowSpecification& spec);
		virtual ~WindowsWindow();

		void Initialize() override;
		void Shutdown() override;
		void OnUpdate() override;

	protected:
		void InitializeContext() override;

	private:
		GLFWwindow* m_Window = nullptr;
	};

}