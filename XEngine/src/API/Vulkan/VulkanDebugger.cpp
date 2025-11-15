#include "src/API/Vulkan/VulkanDebugger.h"
#include "src/API/Vulkan/VulkanContext.h"
#include "src/Core/LogSystem.h"
#include "src/Utility/Utility.h"


namespace XEngine {

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

	VulkanDebugger::VulkanDebugger()
		: m_Debugger(VK_NULL_HANDLE)
	{

	}

	VulkanDebugger::~VulkanDebugger()
	{

	}

	DebuggerInfo VulkanDebugger::GetInfo()
	{
		DebuggerInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr;

		return createInfo;
	}

	void VulkanDebugger::Initialize()
	{
		VulkanContext* ctx = VulkanContext::GetRaw();
		DebuggerInfo createInfo = GetInfo();

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*ctx->GetInstance(), "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			CHECK_VK_RES(func(*ctx->GetInstance(), &createInfo, nullptr, &m_Debugger));
		}
		else
		{
			XEngine_CRITICAL("Failed to init debug messenger!");
		}
	}

	void VulkanDebugger::Shutdown()
	{
		VulkanContext* ctx = VulkanContext::GetRaw();
		DebuggerInfo createInfo = GetInfo();

		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*ctx->GetInstance(), "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(*ctx->GetInstance(), m_Debugger, nullptr);
		}
		else
		{
			XEngine_CRITICAL("Failed to init debug messenger!");
		}
	}

}