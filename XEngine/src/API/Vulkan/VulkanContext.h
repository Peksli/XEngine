#pragma once

#include "src/Renderer/Context.h"
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace XEngine {

	struct VulkanContextSpecification
	{
		GLFWwindow* window = nullptr;
	};

	class VulkanContext : public Context
	{
	public:
		explicit VulkanContext(const VulkanContextSpecification& spec);
		~VulkanContext() override;

		void Initialize() override;
		void SwapBuffers() override;

	private:
		bool CheckLayersSupport(const std::vector<const char*>& requiredLayers) const;
		bool CheckRequiredExtensionsSupport(const std::vector<const char*>& requiredExtensions) const;
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkResult CreateDebugMessenger();
		void DestroyDebugMessenger();

	private:
		VulkanContextSpecification m_Spec;
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
	};

}