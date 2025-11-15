#include "src/API/Vulkan/VulkanDevice.h"
#include "src/API/Vulkan/VulkanContext.h"
#include "src/Core/LogSystem.h"
#include "src/Utility/Utility.h"


namespace XEngine {

	VulkanDevice::VulkanDevice()
		:	m_Device(VK_NULL_HANDLE), 
			m_GraphicsQueue(VK_NULL_HANDLE), 
			m_TransferQueue(VK_NULL_HANDLE),
			m_PresentationQueue(VK_NULL_HANDLE)
	{

	}

	VulkanDevice::~VulkanDevice()
	{

	}

	void VulkanDevice::Initialize()
	{
		VulkanContext* ctx = VulkanContext::GetRaw();
	
		std::vector<VkDeviceQueueCreateInfo> queueInfos;

		std::set<uint32_t> uniqueIndices = 
		{
			ctx->GetPhysicalDevice()->GetGraphicsFamilyIdx(),
			ctx->GetPhysicalDevice()->GetTransferFamilyIdx(),
			ctx->GetPhysicalDevice()->GetPresentationFamilyIdx()
		};

		float priority = 1.0f;
		for (uint32_t familyIdx : uniqueIndices)
		{
			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.pNext = nullptr;
			queueInfo.flags = 0;
			queueInfo.queueFamilyIndex = familyIdx;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &priority;
			queueInfos.push_back(queueInfo);
		}

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

		const std::vector<const char*>& extensions = ctx->GetPhysicalDevice()->GetExtensions();

		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pNext = &features2;
		deviceInfo.flags = 0;
		deviceInfo.queueCreateInfoCount = queueInfos.size();
		deviceInfo.pQueueCreateInfos = queueInfos.data();
		deviceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		deviceInfo.ppEnabledExtensionNames = extensions.data();
		deviceInfo.pEnabledFeatures = nullptr;

		// Creating device
		CHECK_VK_RES(
			vkCreateDevice(
				ctx->GetPhysicalDevice()->GetRaw(), 
				&deviceInfo, 
				nullptr, 
				&m_Device)
		);

		// setting up queues
		vkGetDeviceQueue(m_Device, ctx->GetPhysicalDevice()->GetGraphicsFamilyIdx(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, ctx->GetPhysicalDevice()->GetTransferFamilyIdx(), 0, &m_TransferQueue);
		vkGetDeviceQueue(m_Device, ctx->GetPhysicalDevice()->GetPresentationFamilyIdx(), 0, &m_PresentationQueue);
	}

	void VulkanDevice::Shutdown()
	{
		vkDestroyDevice(
			m_Device, 
			nullptr);
	}

}