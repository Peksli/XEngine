#pragma once

#include "src/Renderer/Context.h"
#include "src/API/Vulkan/VulkanDebugger.h"
#include "src/API/Vulkan/VulkanInstance.h"
#include "src/API/Vulkan/VulkanSurface.h"
#include "src/API/Vulkan/VulkanPhysicalDevice.h"
#include "src/API/Vulkan/VulkanDevice.h"

#include <memory>


namespace XEngine {

	struct VulkanContextSpecification
	{
		void* window = nullptr;
	};

	class VulkanContext : public Context
	{
	public:
		VulkanContext(VulkanContextSpecification& spec);
		virtual ~VulkanContext();

		void Initialize() override;
		void SwapBuffers() override;
		void Shutdown() override;

		static VulkanContext* GetRaw() { return s_Instance; }

		const std::unique_ptr<VulkanDebugger>&			GetDebugger()		const { return m_Debugger;			}
		const std::unique_ptr<VulkanInstance>&			GetInstance()		const { return m_Instance;			}
		const std::unique_ptr<VulkanSurface>&			GetSurface()		const { return m_Surface;			}
		const std::unique_ptr<VulkanPhysicalDevice>&	GetPhysicalDevice() const { return m_PhysicalDevice;	}
		const std::unique_ptr<VulkanDevice>&			GetDevice()			const { return m_Device;			}
		

	private:
		std::unique_ptr<VulkanDebugger> m_Debugger;
		std::unique_ptr<VulkanInstance> m_Instance;
		std::unique_ptr<VulkanSurface> m_Surface;
		std::unique_ptr<VulkanPhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<VulkanDevice> m_Device;

		static VulkanContext* s_Instance;
		VulkanContextSpecification m_Specification;
	};

}