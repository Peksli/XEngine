#pragma once

#include <vulkan/vulkan.h>


namespace XEngine {

	class VulkanDevice
	{
	public:
		VulkanDevice();
		virtual ~VulkanDevice();

		void Initialize();
		void Shutdown();

		VkDevice GetRaw() const { return m_Device; }
		VkQueue  GetGraphicsQueue() const { return m_GraphicsQueue; }
		VkQueue  GetTransferQueue() const { return m_TransferQueue; }
		VkQueue  GetPresentationQueue() const { return m_PresentationQueue; }

		operator VkDevice() const { return m_Device; }

	private:
		VkDevice m_Device;
		VkQueue m_GraphicsQueue;
		VkQueue m_TransferQueue;
		VkQueue m_PresentationQueue;
	};

}