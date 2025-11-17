#include "src/API/Vulkan/VulkanImage.h"


namespace XEngine {

	// Vulkan image

	VulkanImage::VulkanImage()
		: m_Spec(),
		  m_Image(VK_NULL_HANDLE),
		  m_View(VK_NULL_HANDLE)
	{

	}

	VulkanImage::VulkanImage(const ImageSpecification& spec, VkImage image, VkImageView view)
		: m_Spec(spec),
		  m_Image(image),
		  m_View(view)
	{

	}

	VulkanImage::~VulkanImage()
	{

	}

	// Vulkan swapchain image

	VulkanSwapchainImage::VulkanSwapchainImage()
		: VulkanImage()
	{

	}

	VulkanSwapchainImage::VulkanSwapchainImage(const ImageSpecification& spec, VkImage image, VkImageView view)
		: VulkanImage(spec, image, view)
	{

	}

	VulkanSwapchainImage::~VulkanSwapchainImage()
	{

	}

}