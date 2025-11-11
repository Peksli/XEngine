#include "src/Renderer/Context.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	Context::Context(GLFWwindow* window)
		: m_Window(window)
	{
		if (!window)
		{
			XEngine_CRITICAL("Can't create context because spec contains nullptr in window");
		}
	}

	Context::~Context()
	{

	}

}