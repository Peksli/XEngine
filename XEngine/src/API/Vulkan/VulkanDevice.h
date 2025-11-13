#pragma once

#include <vulkan/vulkan.h>


namespace XEngine {

	class VulkanDevice
	{
	public:
		VulkanDevice();
		virtual ~VulkanDevice();

		void Initialize();
		void Destroy();

		VkDevice GetHandle() const { return m_Device; }
		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		VkQueue GetTransferQueue() const { return m_TransferQueue; }

	private:
		VkDevice m_Device;
		VkQueue m_GraphicsQueue;
		VkQueue m_TransferQueue;
	};

}