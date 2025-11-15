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
		const std::vector<const char*>& GetExtensions() const { return m_Extensions; }

		operator VkPhysicalDevice() const { return m_PhysicalDevice; }

	private:
		std::multimap<uint32_t, VkPhysicalDevice> EvaluateDevices(const std::vector<VkPhysicalDevice>&) const;
		bool CheckRequiredExtensions(const VkPhysicalDevice&) const;
		void DetermineQueueFamilies();

	private:
		VkPhysicalDevice m_PhysicalDevice;
		std::optional<uint32_t> m_GraphicsQueueFamily;
		std::optional<uint32_t> m_TransferQueueFamily;
		std::optional<uint32_t> m_PresentationQueueFamily;

		std::vector<const char*> m_Extensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
	};

}
