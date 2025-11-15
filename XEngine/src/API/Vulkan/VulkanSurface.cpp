#include "src/API/Vulkan/VulkanSurface.h"
#include "src/API/Vulkan/VulkanContext.h"
#include "src/Core/Application.h"
#include "src/Window/Windows-platform/WindowsWindow.h"
#include "src/Utility/Utility.h"
#include "src/Core/LogSystem.h"

#include <GLFW/glfw3.h>


namespace XEngine {

	VulkanSurface::VulkanSurface()
		: m_Surface(VK_NULL_HANDLE)
	{
		
	}

	VulkanSurface::~VulkanSurface()
	{

	}

	void VulkanSurface::Initialize(void* window)
	{
		VulkanContext* context = VulkanContext::GetRaw();

		GLFWwindow* pGenericWindowGLFW = static_cast<GLFWwindow*>(window);
		if (pGenericWindowGLFW != nullptr)
		{
			CHECK_VK_RES(
				glfwCreateWindowSurface(
					*context->GetInstance(),
					pGenericWindowGLFW,
					nullptr,
					&m_Surface)
			);
		}
		else
		{
			XEngine_CRITICAL("Can't cast to any type of window!");
		}
	}

	void VulkanSurface::Shutdown()
	{
		VulkanContext* context = VulkanContext::GetRaw();

		vkDestroySurfaceKHR(*context->GetInstance(), m_Surface, nullptr);
	}

}