#include "src/API/Vulkan/VulkanPhysicalDevice.h"
#include "src/API/Vulkan/VulkanContext.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	VulkanPhysicalDevice::VulkanPhysicalDevice()
		: m_PhysicalDevice(VK_NULL_HANDLE), m_Features2()
	{
		VulkanContext* context = VulkanContext::GetHandle();

		auto devices = EnumeratePhysicalDevices();
		LogAvailableDevices(devices);

		auto scoredDevices = EvaluateDevices(devices);

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

	std::vector<VkPhysicalDevice> VulkanPhysicalDevice::EnumeratePhysicalDevices() const
	{
		VulkanContext* context = VulkanContext::GetHandle();

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(context->GetIntance(), &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(context->GetIntance(), &deviceCount, devices.data());

		return devices;
	}

	void VulkanPhysicalDevice::LogAvailableDevices(const std::vector<VkPhysicalDevice>& devices) const
	{
		XEngine_DEBUG("Found Vulkan Physical Devices:");
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

		for (uint32_t i = 0; i < familyCount; ++i)
		{
			const auto& props = familyProps[i].queueFamilyProperties;

			if ((props.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !m_GraphicsQueueFamily.has_value())
			{
				m_GraphicsQueueFamily = i;
			}

			if ((props.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
				!(props.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
				!m_TransferQueueFamily.has_value())
			{
				m_TransferQueueFamily = i;
			}
		}

		if (!m_GraphicsQueueFamily.has_value()) XEngine_CRITICAL("Didn't find graphics queue");
		if (!m_TransferQueueFamily.has_value()) XEngine_CRITICAL("Didn't find transfer queue");
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