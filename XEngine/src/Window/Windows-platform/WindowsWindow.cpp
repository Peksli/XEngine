#include "src/Window/Windows-platform/WindowsWindow.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	WindowsWindow::WindowsWindow(const WindowSpecification& spec) 
	: Window(spec) {
		Initialize();
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
	}

	void WindowsWindow::Initialize()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

		if (!m_Window) {
			XEngine_CRITICAL("Didn't create glfw window");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_Window);

	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

}