#include "src/Window/Window.h"
#include "src/Window/Windows-platform/WindowsWindow.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	std::unique_ptr<Window> XEngine::Window::CreateWindow(const WindowSpecification& spec) {
		switch (spec.platform) 
		{
		case Platform::Windows: return std::make_unique<WindowsWindow>(spec); break;
			/* ... */
		default: XEngine_CRITICAL("Unknown platform, can't create window");
		}
	}

	Window::Window(const WindowSpecification& spec)
		: m_Width(spec.width), m_Height(spec.height), m_IsVsync(spec.wantVsync) {

	}

}