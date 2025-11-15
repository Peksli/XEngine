#include "src/API/Vulkan/VulkanPhysicalDevice.h"
#include "src/API/Vulkan/VulkanContext.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	VulkanPhysicalDevice::VulkanPhysicalDevice()
		: m_PhysicalDevice(VK_NULL_HANDLE)
	{
		LogDevices();

		auto scoredDevices = EvaluateDevices(GetDevices());

		if (!scoredDevices.empty()) 
		{
			m_PhysicalDevice = scoredDevices.rbegin()->second;
			XEngine_DEBUG(fmt::runtime("Selected device: {0}"), GetName());
		}
		else 
		{
			XEngine_CRITICAL("No suitable Vulkan physical devices found!");
		}

		DetermineQueueFamilies();
	}

	std::vector<VkPhysicalDevice> VulkanPhysicalDevice::GetDevices()
	{
		VulkanContext* ctx = VulkanContext::GetRaw();

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(*ctx->GetInstance(), &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(*ctx->GetInstance(), &deviceCount, devices.data());

		return devices;
	}

	void VulkanPhysicalDevice::LogDevices()
	{
		VulkanContext* ctx = VulkanContext::GetRaw();

		auto devices = std::move(GetDevices());

		XEngine_DEBUG("Found physical devices:");
		for (const auto& device : devices)
		{
			VkPhysicalDeviceProperties props{};
			vkGetPhysicalDeviceProperties(device, &props);
			XEngine_DEBUG(fmt::runtime("\t- {0}"), props.deviceName);
		}
	}

	std::multimap<uint32_t, VkPhysicalDevice> VulkanPhysicalDevice::EvaluateDevices(const std::vector<VkPhysicalDevice>& devices) const
	{
		std::multimap<uint32_t, VkPhysicalDevice> scoredDevices;

		for (const auto& device : devices)
		{
			uint32_t score = 0;
			VkPhysicalDeviceProperties props{};
			vkGetPhysicalDeviceProperties(device, &props);

			VkPhysicalDeviceVulkan12Features features12 = {};
			features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;

			VkPhysicalDeviceVulkan13Features features13 = {};
			features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
			features13.pNext = &features12;

			VkPhysicalDeviceFeatures2 features2 = {};
			features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
			features2.pNext = &features13;

			vkGetPhysicalDeviceFeatures2(device, &features2);

			if (props.apiVersion >= VK_API_VERSION_1_4)
				score += 100;	

			if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 150;

			if (!features13.dynamicRendering || !features13.synchronization2)
				continue;

			if (!features12.bufferDeviceAddress || !features12.descriptorIndexing)
				continue;

			if(!CheckRequiredExtensions(device))
				continue;

			scoredDevices.emplace(score, device);
		}

		return scoredDevices;
	}

	void VulkanPhysicalDevice::DetermineQueueFamilies()
	{
		uint32_t familyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties2(m_PhysicalDevice, &familyCount, nullptr);

		std::vector<VkQueueFamilyProperties2> familyProps(familyCount);
		for (auto& fp : familyProps)
		{
			fp.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
			fp.pNext = nullptr;
		}
		vkGetPhysicalDeviceQueueFamilyProperties2(m_PhysicalDevice, &familyCount, familyProps.data());

		m_GraphicsQueueFamily.reset();
		m_TransferQueueFamily.reset();
		m_PresentationQueueFamily.reset();

		VulkanContext* ctx = VulkanContext::GetRaw();
		auto surface = ctx->GetSurface()->GetRaw();

		for (uint32_t i = 0; i < familyCount; ++i)
		{
			const auto& props = familyProps[i].queueFamilyProperties;

			if (!m_GraphicsQueueFamily.has_value() && (props.queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				m_GraphicsQueueFamily = i;
			}

			if (!m_TransferQueueFamily.has_value() &&
				(props.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
				!(props.queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				m_TransferQueueFamily = i;
			}

			VkBool32 isSupported = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, surface, &isSupported);
			if (isSupported && !m_PresentationQueueFamily.has_value())
			{
				m_PresentationQueueFamily = i;
			}
		}

		if (!m_GraphicsQueueFamily.has_value()) XEngine_CRITICAL("Didn't find graphics queue");
		if (!m_TransferQueueFamily.has_value()) XEngine_CRITICAL("Didn't find transfer queue");
		if (!m_PresentationQueueFamily.has_value()) XEngine_CRITICAL("Didn't find presentation queue");
	}

	bool VulkanPhysicalDevice::CheckRequiredExtensions(const VkPhysicalDevice& physicalDevice) const
	{
		uint32_t extensionsCount = 0;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionsCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, availableExtensions.data());

		for (const auto& required : m_Extensions) 
		{
			bool found = false;
			for (const auto& available : availableExtensions)
			{
				if (strcmp(required, available.extensionName) == 0) 
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				XEngine_DEBUG(fmt::runtime("Missing required extension: {}"), required);
				return false;
			}
		}

		return true;
	}

	std::string VulkanPhysicalDevice::GetName() const
	{
		if (m_PhysicalDevice == VK_NULL_HANDLE)
			return "No device selected";

		VkPhysicalDeviceProperties props{};
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &props);
		return std::string(props.deviceName);
	}

}