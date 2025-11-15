#include "src/Window/Window.h"
#include "src/Window/Windows-platform/WindowsWindow.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	Window::Window(const WindowSpecification& spec)
		:	m_Spec(spec)
	{

	}

	Window::~Window()
	{

	}

	std::unique_ptr<Window> Window::Create(WindowSpecification& spec)
	{
		switch (spec.platform)
		{
		case Platform::Windows: return std::make_unique<WindowsWindow>(spec); break;
			/* ... */
		default:
			XEngine_CRITICAL("Unknown platform, selecting windows");
			return std::make_unique<WindowsWindow>(spec);
		}
	}

}