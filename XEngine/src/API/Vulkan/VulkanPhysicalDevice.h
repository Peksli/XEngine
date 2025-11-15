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

		static std::vector<VkPhysicalDevice> GetDevices();
		static void LogDevices();

		VkPhysicalDevice GetRaw() const { return m_PhysicalDevice; }
		std::string GetName() const;
		uint32_t GetGraphicsFamilyIdx()		const { return m_GraphicsQueueFamily.value();		}
		uint32_t GetTransferFamilyIdx()		const { return m_TransferQueueFamily.value();		}
		uint32_t GetPresentationFamilyIdx() const { return m_PresentationQueueFamily.value();	}

	private:
		std::multimap<uint32_t, VkPhysicalDevice> EvaluateDevices(const std::vector<VkPhysicalDevice>&) const;
		void DetermineQueueFamilies();

	private:
		VkPhysicalDevice m_PhysicalDevice;
		std::optional<uint32_t> m_GraphicsQueueFamily;
		std::optional<uint32_t> m_TransferQueueFamily;
		std::optional<uint32_t> m_PresentationQueueFamily;
	};

}
