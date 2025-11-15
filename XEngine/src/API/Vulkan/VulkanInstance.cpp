#include "src/API/Vulkan/VulkanInstance.h"
#include "src/API/Vulkan/VulkanDebugger.h"
#include "src/Core/LogSystem.h"
#include "src/Utility/Utility.h"

#include <GLFW/glfw3.h>


namespace XEngine {

	VulkanInstance::VulkanInstance()
		: m_Instance(VK_NULL_HANDLE)
	{

	}

	VulkanInstance::~VulkanInstance()
	{

	}

	void VulkanInstance::Initialize()
	{
		// Init for glfw extensions
		uint32_t glfwExtensionsCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

		// App info
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan app";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = nullptr;
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_4;

		// Layers
		std::vector<const char*> requiredLayers = 
		{
			"VK_LAYER_KHRONOS_validation"
		};

		// Extensions
		std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);
		requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		// Info for future use
		DebuggerInfo debuggerInfo = VulkanDebugger::GetInfo();

		// Instance creation
		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pNext = &debuggerInfo;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();
		instanceInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
		instanceInfo.ppEnabledLayerNames = requiredLayers.data();

		CHECK_VK_RES(vkCreateInstance(&instanceInfo, nullptr, &m_Instance));
	}

	void VulkanInstance::Shutdown()
	{
		vkDestroyInstance(m_Instance, nullptr);
	}

}