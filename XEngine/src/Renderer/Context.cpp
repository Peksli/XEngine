#include "src/Renderer/Context.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	Context::Context(GLFWwindow* window)
		: m_Window(window)
	{
		if (!window)
		{
			XEngine_CRITICAL("Can't create context because window is nullptr");
		}
	}

	Context::~Context()
	{

	}

	void Context::Initialize()
	{

	}

	void Context::SwapBuffers()
	{

	}

}