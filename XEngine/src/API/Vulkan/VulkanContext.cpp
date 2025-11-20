#include "src/API/Vulkan/VulkanContext.h"
#include "src/Utility/Utility.h"
#include "src/Core/LogSystem.h"

// !!!!! for test
#include "src/API/Vulkan/VulkanPipeline.h"

#include <GLFW/glfw3.h>


namespace XEngine {

	VulkanContext* VulkanContext::s_Instance = nullptr;

	VulkanContext::VulkanContext(VulkanContextSpecification& spec)
		: m_Specification(spec)
	{
		s_Instance = this;
	}

	VulkanContext::~VulkanContext()
	{
		s_Instance = nullptr;
	}

	void VulkanContext::Initialize()
	{
		m_Instance = std::make_unique<VulkanInstance>();
		m_Instance->Initialize();

		m_Debugger = std::make_unique<VulkanDebugger>();
		m_Debugger->Initialize();

		m_Surface = std::make_unique<VulkanSurface>();
		m_Surface->Initialize(m_Specification.window);

		m_PhysicalDevice = std::make_unique<VulkanPhysicalDevice>();

		m_Device = std::make_unique<VulkanDevice>();
		m_Device->Initialize();

		m_Swapchain = std::make_unique<VulkanSwapchain>();
		m_Swapchain->Initialize();

		PipelineSpecification spec;
		spec.type = PipelineType::GRAPHICS;
		spec.window_width = m_Specification.window_width;
		spec.window_height = m_Specification.window_height;

		VulkanPipeline* pipeline = new VulkanPipeline(spec);
		pipeline->Initialize();
		pipeline->Shutdown();
		delete pipeline;
	}

	void VulkanContext::SwapBuffers()
	{

	}

	void VulkanContext::Shutdown()
	{
		m_Swapchain->Shutdown();
		m_Device->Shutdown();
		m_Surface->Shutdown();
		m_Debugger->Shutdown();
		m_Instance->Shutdown();
	}

}