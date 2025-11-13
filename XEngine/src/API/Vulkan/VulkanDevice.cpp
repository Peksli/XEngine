#include "src/API/Vulkan/VulkanDevice.h"
#include "src/API/Vulkan/VulkanContext.h"
#include "src/Core/LogSystem.h"
#include "src/Utility/Utility.h"


namespace XEngine {

	VulkanDevice::VulkanDevice()
		:	m_Device(VK_NULL_HANDLE), 
			m_GraphicsQueue(VK_NULL_HANDLE), 
			m_TransferQueue(VK_NULL_HANDLE)
	{

	}

	VulkanDevice::~VulkanDevice()
	{

	}

	void VulkanDevice::Initialize()
	{
		VulkanContext* context = VulkanContext::GetHandle();

		constexpr uint32_t queuesCount = 2;
		float queuePriority = 1.0f;

		// from which queue family we will take graphicsQueue and how much
		VkDeviceQueueCreateInfo graphicsQueueInfo = {};
		graphicsQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsQueueInfo.pNext = nullptr;
		graphicsQueueInfo.flags = 0;
		graphicsQueueInfo.queueFamilyIndex = context->GetPhysicalDevice()->GetGraphicsFamilyIdx().value();
		graphicsQueueInfo.queueCount = 1;
		graphicsQueueInfo.pQueuePriorities = nullptr;
		graphicsQueueInfo.pQueuePriorities = &queuePriority;

		// from which queue family we will take transferQueue and how much
		VkDeviceQueueCreateInfo trasnferQueueInfo = {};
		trasnferQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		trasnferQueueInfo.pNext = nullptr;
		trasnferQueueInfo.flags = 0;
		trasnferQueueInfo.queueFamilyIndex = context->GetPhysicalDevice()->GetTransferFamilyIdx().value();
		trasnferQueueInfo.queueCount = 1;
		trasnferQueueInfo.pQueuePriorities = &queuePriority;

		std::array<VkDeviceQueueCreateInfo, 2> queueInfos;
		queueInfos[0] = graphicsQueueInfo;
		queueInfos[1] = trasnferQueueInfo;

		// since we already filtered physicalDevice, we can just request features
		VkPhysicalDeviceVulkan12Features features12 = {};
		features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
		features12.bufferDeviceAddress = true;
		features12.descriptorIndexing = true;

		VkPhysicalDeviceVulkan13Features features13 = {};
		features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		features13.pNext = &features12;
		features13.dynamicRendering = true;
		features13.synchronization2 = true;

		VkPhysicalDeviceFeatures2 features2 = {};
		features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		features2.pNext = &features13;

		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pNext = &features2;
		deviceInfo.flags = 0;
		deviceInfo.queueCreateInfoCount = queueInfos.size();
		deviceInfo.pQueueCreateInfos = queueInfos.data();
		deviceInfo.enabledExtensionCount = 0;
		deviceInfo.ppEnabledExtensionNames = nullptr;
		deviceInfo.pEnabledFeatures = nullptr;

		// Creating device
		CHECK_VK_RES(
			vkCreateDevice(
				context->GetPhysicalDevice()->GetRaw(), 
				&deviceInfo, 
				nullptr, 
				&m_Device)
		);

		// setting up queues
		vkGetDeviceQueue(m_Device, context->GetPhysicalDevice()->GetGraphicsFamilyIdx().value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, context->GetPhysicalDevice()->GetTransferFamilyIdx().value(), 0, &m_TransferQueue);
	}

	void VulkanDevice::Destroy()
	{
		vkDestroyDevice(
			m_Device, 
			nullptr);
	}

}