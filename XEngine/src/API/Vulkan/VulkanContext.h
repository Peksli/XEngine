#pragma once

#include "src/Renderer/Context.h"
#include "src/API/Vulkan/VulkanPhysicalDevice.h"
#include "src/API/Vulkan/VulkanDevice.h"

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
		explicit VulkanContext(const VulkanContextSpecification&);
		~VulkanContext() override;

		void Initialize() override;
		void SwapBuffers() override;

		static VulkanContext* GetHandle() { return s_Instance; }
		VkInstance GetIntance() const { return m_Instance; }
		const std::unique_ptr<VulkanPhysicalDevice>& GetPhysicalDevice() const { return m_PhysicalDevice; }
		const std::unique_ptr<VulkanDevice>& GetDevice() const { return m_Device; }

	private:
		bool CheckLayersSupport(const std::vector<const char*>&) const;
		bool CheckRequiredExtensionsSupport(const std::vector<const char*>&) const;
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);
		VkResult CreateDebugMessenger();
		void DestroyDebugMessenger();

	private:
		VulkanContextSpecification m_Spec;
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		std::unique_ptr<VulkanPhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<VulkanDevice> m_Device;

		static VulkanContext* s_Instance;
	};

}