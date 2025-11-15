#pragma once

#include <vulkan/vulkan.h>

namespace XEngine {

	using DebuggerInfo = VkDebugUtilsMessengerCreateInfoEXT;
	using DebugMessenger = VkDebugUtilsMessengerEXT;

	class VulkanDebugger
	{
	public:
		VulkanDebugger();
		virtual ~VulkanDebugger();

		static DebuggerInfo GetInfo();

		void Initialize();
		void Shutdown();

		DebugMessenger GetRaw() { return m_Debugger; }

	private:
		DebugMessenger m_Debugger;
	};

}