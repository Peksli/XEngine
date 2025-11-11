// VulkanPhysicalDevice.h
#pragma once

#include <vulkan/vulkan.h>
#include <map>
#include <vector>
#include <string>
#include <optional>

namespace XEngine {

	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice() = default;

		VkPhysicalDevice GetSelectedDevice() const { return m_PhysicalDevice; }
		std::string GetName() const;

	private:
		std::vector<VkPhysicalDevice> EnumeratePhysicalDevices() const;
		void LogAvailableDevices(const std::vector<VkPhysicalDevice>&) const;
		std::multimap<uint32_t, VkPhysicalDevice> EvaluateDevices(const std::vector<VkPhysicalDevice>&) const;
		void DetermineQueueFamilies();

	private:
		VkPhysicalDevice m_PhysicalDevice;
		std::optional<uint32_t> m_GraphicsQueueFamily;
		std::optional<uint32_t> m_TransferQueueFamily;
	};

}
