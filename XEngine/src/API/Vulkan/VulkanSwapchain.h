#pragma once

#include "src/API/Vulkan/VulkanImage.h"

#include <vulkan/vulkan.h>


namespace XEngine {

	struct VulkanSwapchainDetails
	{
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> present_modes;
	};

	class VulkanSwapchain
	{
	public:
		VulkanSwapchain();
		virtual ~VulkanSwapchain();

		void Initialize();
		void Shutdown();
		
		const std::vector<VulkanSwapchainImage> GetImages() const { return m_Images; }

	private:
		VulkanSwapchainDetails GetDetails();
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>&);
		VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
		VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR&);
		void CreateSwapchainImages(const ImageSpecification& spec);

		template<typename T, typename Predicate>
		T ChooseBest(const std::vector<T>& options, Predicate pred, T fallback) const 
		{
			auto it = std::find_if(options.begin(), options.end(), pred);
			return it != options.end() ? *it : fallback;
		} 

	private:
		VkSwapchainKHR m_Swapchain;
		std::vector<VulkanSwapchainImage> m_Images;
	};

}