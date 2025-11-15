#include "src/API/Vulkan/VulkanSwapchain.h"
#include "src/API/Vulkan/VulkanContext.h"
#include "src/Core/LogSystem.h"
#include "src/Utility/Utility.h"

#include <GLFW/glfw3.h>


namespace XEngine {

	VulkanSwapchain::VulkanSwapchain()
		: m_Swapchain(VK_NULL_HANDLE)
	{

	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		
	}

	void VulkanSwapchain::Initialize()
	{
		VulkanContext* ctx = VulkanContext::GetRaw();

		VulkanSwapchainDetails details = GetDetails();
		VkSurfaceCapabilitiesKHR capabilities = details.capabilities;
		VkSurfaceFormatKHR format = ChooseSurfaceFormat(details.formats);
		VkPresentModeKHR presentMode = ChoosePresentMode(details.present_modes);
		VkExtent2D extent2D = ChooseExtent(capabilities);

		// Info about images in swapchain
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.surface = *ctx->GetSurface();
		createInfo.minImageCount = capabilities.minImageCount + 1;
		createInfo.imageFormat = format.format;
		createInfo.imageColorSpace = format.colorSpace;
		createInfo.imageExtent = extent2D;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		createInfo.imageArrayLayers = 1;

		// Sharing mode + queues
		uint32_t graphicsFamilyIdx = ctx->GetPhysicalDevice()->GetGraphicsFamilyIdx();
		uint32_t presentationFamilyIdx = ctx->GetPhysicalDevice()->GetPresentationFamilyIdx();
		std::array<uint32_t, 2> queueFamilyIndices = 
		{
			graphicsFamilyIdx,
			presentationFamilyIdx 
		};

		if (graphicsFamilyIdx != presentationFamilyIdx)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
			createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		// other info
		createInfo.preTransform = capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		CHECK_VK_RES(
			vkCreateSwapchainKHR(
				*ctx->GetDevice(),
				&createInfo, 
				nullptr,
				&m_Swapchain)
		);

		uint32_t imageCount = 0;
		vkGetSwapchainImagesKHR(*ctx->GetDevice(), m_Swapchain, &imageCount, nullptr);
		m_Images.resize(imageCount);
		vkGetSwapchainImagesKHR(*ctx->GetDevice(), m_Swapchain, &imageCount, m_Images.data());
	}

	void VulkanSwapchain::Shutdown()
	{
		VulkanContext* ctx = VulkanContext::GetRaw();

		vkDestroySwapchainKHR(*ctx->GetDevice(), m_Swapchain, nullptr);
	}

	VulkanSwapchainDetails VulkanSwapchain::GetDetails()
	{
		VulkanContext* ctx = VulkanContext::GetRaw();

		VulkanSwapchainDetails details;

		// capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
			*ctx->GetPhysicalDevice(), 
			*ctx->GetSurface(), 
			&details.capabilities
		);

		// formats
		uint32_t formatsCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			*ctx->GetPhysicalDevice(),
			*ctx->GetSurface(), 
			&formatsCount, 
			nullptr
		);
		if (formatsCount != 0)
		{
			details.formats.resize(formatsCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(
				*ctx->GetPhysicalDevice(),
				*ctx->GetSurface(),
				&formatsCount,
				details.formats.data()
			);
		}

		// present modes
		uint32_t modesCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			*ctx->GetPhysicalDevice(),
			*ctx->GetSurface(),
			&modesCount,
			nullptr
		);
		if (modesCount != 0)
		{
			details.present_modes.resize(modesCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(
				*ctx->GetPhysicalDevice(),
				*ctx->GetSurface(),
				&modesCount,
				details.present_modes.data()
			);
		}

		return details;
	}

	VkPresentModeKHR VulkanSwapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& modes)
	{
		if (modes.empty())
		{
			XEngine_CRITICAL("No present modes available!");
			return VkPresentModeKHR{};
		}

		return ChooseBest(modes,
			[](const VkPresentModeKHR& mode) 
			{
				return mode == VK_PRESENT_MODE_MAILBOX_KHR;
			},
			ChooseBest(modes, // fallback
				[](const VkPresentModeKHR& mode) 
				{
					return mode == VK_PRESENT_MODE_FIFO_KHR;
				},
				VK_PRESENT_MODE_IMMEDIATE_KHR)
		);
	}

	VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
	{
		if (formats.empty()) 
		{
			XEngine_CRITICAL("No surface formats available!");
			return VkSurfaceFormatKHR{};
		}

		return ChooseBest(formats,
			[](const VkSurfaceFormatKHR& format) 
			{
				return format.format == VK_FORMAT_R8G8B8A8_SRGB &&
					format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
			},
			ChooseBest(formats, // fallback
				[](const VkSurfaceFormatKHR& format) 
				{
					return format.format == VK_FORMAT_R8G8B8A8_SRGB;
				},
				formats.empty() ? VkSurfaceFormatKHR{} : formats[0])
		);
	}

	VkExtent2D VulkanSwapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			VulkanContext* ctx = VulkanContext::GetRaw();
			GLFWwindow* window = static_cast<GLFWwindow*>(ctx->GetSpecification().window);

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			VkExtent2D realExtent =
			{
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			realExtent.width = std::clamp(
				realExtent.width, 
				capabilities.minImageExtent.width, 
				capabilities.maxImageExtent.width
			);
			realExtent.height = std::clamp(
				realExtent.height, 
				capabilities.minImageExtent.height, 
				capabilities.maxImageExtent.height
			);

			return realExtent;
		}
	}

}