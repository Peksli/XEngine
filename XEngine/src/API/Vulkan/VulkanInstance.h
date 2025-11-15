#pragma once

#include <vulkan/vulkan.h>


namespace XEngine {

	class VulkanInstance
	{
	public:
		VulkanInstance();
		virtual ~VulkanInstance();

		void Initialize();
		void Shutdown();

		VkInstance GetRaw() const { return m_Instance; }

		operator VkInstance() const { return m_Instance; }

	private:
		VkInstance m_Instance;
	};

}