#pragma once

#include <vulkan/vulkan.h>

/*

Goal: be able to use VulkanImage instead of VkImage in:
	1) VulkanSwapchain retrieve

*/


namespace XEngine {

	struct ImageSpecification
	{
		VkFormat format = VK_FORMAT_UNDEFINED;
		VkColorSpaceKHR colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		VkExtent2D extent = {};
	};

#pragma region Images

	class VulkanImage
	{
	public:
		VulkanImage();
		VulkanImage(const ImageSpecification&, VkImage, VkImageView);
		virtual ~VulkanImage();

		VkImage			GetRaw()			const { return m_Image;				}
		VkImageView		GetRawView()		const { return m_View;				}
		VkFormat		GetFormat()			const { return m_Spec.format;		}
		VkColorSpaceKHR GetColorSpace()		const { return m_Spec.colorSpace;	}

		operator VkImage()		const { return m_Image; }
		operator VkImageView()	const { return m_View;	}

	protected:
		ImageSpecification m_Spec;
		VkImage m_Image;
		VkImageView m_View;
	};


	class VulkanSwapchainImage : public VulkanImage
	{
	public:
		VulkanSwapchainImage();
		VulkanSwapchainImage(const ImageSpecification&, VkImage, VkImageView);
		virtual ~VulkanSwapchainImage();
	};


	class VulkanAllocatedImage : public VulkanImage
	{

	};

#pragma endregion

}