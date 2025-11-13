#include "src/API/Vulkan/VulkanContext.h"
#include "src/Utility/Utility.h"
#include "src/Core/LogSystem.h"

#include <GLFW/glfw3.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT type,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	switch (severity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		XEngine_TRACE(fmt::runtime("[Diagnostic] {0}"), pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		XEngine_INFO(fmt::runtime("[Warning] {0}"), pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		XEngine_CRITICAL(fmt::runtime("[Error] {0}"), pCallbackData->pMessage);
		break;
	default:
		break;
	}

	return VK_FALSE;
}

namespace XEngine {

	VulkanContext* VulkanContext::s_Instance = nullptr;

	VulkanContext::VulkanContext(const VulkanContextSpecification& spec)
		: Context(spec.window), m_Spec(spec), m_Instance(VK_NULL_HANDLE), m_DebugMessenger(VK_NULL_HANDLE)
	{
		if (s_Instance)
		{
			XEngine_CRITICAL("Trying to create another context while only 1 can be");
			abort();
		}
		else
		{
			s_Instance = this;
		}
	}

	VulkanContext::~VulkanContext()
	{
		DestroyDebugMessenger();
		if (m_Instance != VK_NULL_HANDLE) 
		{
			vkDestroyInstance(m_Instance, nullptr);
		}
	}

	void VulkanContext::Initialize()
	{
		/* =============================== INSTANCE =============================== */
#if defined(XEngine_DEBUG_BUILD)
		std::vector<const char*> reqLayers = { "VK_LAYER_KHRONOS_validation" };

		if (!CheckLayersSupport(reqLayers))
		{
			XEngine_CRITICAL("Missing required validation layers!");
		}
#endif

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan app";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = nullptr;
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_4;

		uint32_t glfwExtCount = 0;
		const char** glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
		std::vector<const char*> reqExtensions(glfwExts, glfwExts + glfwExtCount);
#if defined(XEngine_DEBUG_BUILD)
		reqExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
		if (!CheckRequiredExtensionsSupport(reqExtensions))
		{
			XEngine_CRITICAL("Missing required extensions");
		}

		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = static_cast<uint32_t>(reqExtensions.size());
		instanceInfo.ppEnabledExtensionNames = reqExtensions.data();
#if defined(XEngine_DEBUG_BUILD)
		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
		PopulateDebugMessengerCreateInfo(debugMessengerInfo);
		instanceInfo.pNext = &debugMessengerInfo;
		instanceInfo.enabledLayerCount = static_cast<uint32_t>(reqLayers.size());
		instanceInfo.ppEnabledLayerNames = reqLayers.data();
#else
		instanceInfo.pNext = nullptr;
		instanceInfo.enabledLayerCount = 0;
		instanceInfo.ppEnabledLayerNames = nullptr;
#endif
		CHECK_VK_RES(vkCreateInstance(&instanceInfo, nullptr, &m_Instance));

		/* =============================== DEBUG MESSENGER =============================== */
#if defined(XEngine_DEBUG_BUILD)
		CHECK_VK_RES(CreateDebugMessenger());
#endif

		/* =============================== PHYSICAL DEVICE =============================== */
		m_PhysicalDevice = std::make_unique<VulkanPhysicalDevice>();

		/* =============================== LOGICAL DEVICE =============================== */
		m_Device = std::make_unique<VulkanDevice>();
	}

	void VulkanContext::SwapBuffers()
	{
		/* ... */
	}

	bool VulkanContext::CheckLayersSupport(const std::vector<const char*>& requiredLayers) const
	{
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : requiredLayers)
		{
			bool found = false;
			for (const auto& layer : availableLayers)
			{
				if (strcmp(layerName, layer.layerName) == 0)
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				XEngine_CRITICAL(fmt::runtime("Missing required layer: {0}"), layerName);
				return false;
			}
		}
		return true;
	}

	bool VulkanContext::CheckRequiredExtensionsSupport(const std::vector<const char*>& requiredExtensions) const
	{
		uint32_t totalCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &totalCount, nullptr);
		std::vector<VkExtensionProperties> allExts(totalCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &totalCount, allExts.data());

		for (auto& reqExtension : requiredExtensions)
		{
			bool isPresent = false;

			for (const VkExtensionProperties& extension : allExts)
			{
				if (strcmp(reqExtension, extension.extensionName) == 0)
				{
					isPresent = true;
					break;
				}
			}

			if (!isPresent)
			{
				XEngine_CRITICAL(fmt::runtime("Didn't find extension: {0}"), reqExtension);
				return false;
			}
		}

		return true;
	}

	void VulkanContext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr;
	}

	VkResult VulkanContext::CreateDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		PopulateDebugMessengerCreateInfo(createInfo);

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(m_Instance, &createInfo, nullptr, &m_DebugMessenger);
		}
		else
		{
			XEngine_CRITICAL("Failed to find vkCreateDebugUtilsMessengerEXT function");
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanContext::DestroyDebugMessenger()
	{
		if (m_DebugMessenger != VK_NULL_HANDLE)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				func(m_Instance, m_DebugMessenger, nullptr);
			}
			else
			{
				XEngine_CRITICAL("Failed to find vkDestroyDebugUtilsMessengerEXT function");
			}
		}
	}

}