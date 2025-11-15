#pragma once

#include <vulkan/vulkan.h>


namespace XEngine {

	class VulkanSurface
	{
	public:
		VulkanSurface();
		virtual ~VulkanSurface();

		void Initialize(void* window);
		void Shutdown();

		VkSurfaceKHR GetRaw() const { return m_Surface; }

		operator VkSurfaceKHR() const { return m_Surface; }

	private:
		VkSurfaceKHR m_Surface;
	};

}