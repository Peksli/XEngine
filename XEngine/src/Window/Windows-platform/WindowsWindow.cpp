#include "src/Window/Windows-platform/WindowsWindow.h"
#include "src/API/Vulkan/VulkanContext.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	WindowsWindow::WindowsWindow(const WindowSpecification& spec) 
	: Window(spec) 
	{

	}

	WindowsWindow::~WindowsWindow() 
	{

	}

	void WindowsWindow::OnUpdate() 
	{
		glfwPollEvents();
	}

	void WindowsWindow::InitializeContext()
	{
		VulkanContextSpecification ctxSpec;
		ctxSpec.window = m_Window;

		m_Context = std::make_unique<VulkanContext>(ctxSpec);
		m_Context->Initialize();
	}

	void WindowsWindow::Initialize()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_Window = glfwCreateWindow(m_Spec.width, m_Spec.height, m_Spec.title.c_str(), nullptr, nullptr);
		if (!m_Window)
		{
			XEngine_CRITICAL("Didn't create glfw window");
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Spec);

		// Context
		InitializeContext();

		// Window events
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowSpecification& spec = *(WindowSpecification*) glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				spec.eventCallback(event);
			});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowSpecification& spec = *(WindowSpecification*)glfwGetWindowUserPointer(window);

				WindowResizeEvent event(width, height);
				spec.eventCallback(event);
			});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
			{
				WindowSpecification& spec = *(WindowSpecification*)glfwGetWindowUserPointer(window);

				WindowMovedEvent event(xpos, ypos);
				spec.eventCallback(event);
			});

		// Mouse events
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) 
			{
				WindowSpecification& spec = *(WindowSpecification*)glfwGetWindowUserPointer(window);

				MouseCursorMovedEvent event(xpos, ypos);
				spec.eventCallback(event);
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowSpecification& spec = *(WindowSpecification*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event(xoffset, yoffset);
				spec.eventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int modes) 
			{
				WindowSpecification& spec = *(WindowSpecification*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					spec.eventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					spec.eventCallback(event);
					break;
				}

				default: 
					XEngine_CRITICAL("[MouseButton] Didn't send event to eventCallback");
				}
			});
		
		// Keyboard events
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int modes) 
			{
				WindowSpecification& spec = *(WindowSpecification*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, false);
					spec.eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					spec.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					spec.eventCallback(event);
					break;
				}

				default:
					XEngine_CRITICAL("[Keyboard] Undefined action");
				}
			});
	}

	void WindowsWindow::Shutdown() 
	{
		m_Context->Shutdown();

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

}