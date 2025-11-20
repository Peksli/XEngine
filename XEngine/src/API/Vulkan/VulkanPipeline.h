#pragma once

#include "src/Renderer/Pipeline.h"

#include <vulkan/vulkan.h>


namespace XEngine {

	enum class PipelineType
	{
		GRAPHICS,
		COMPUTE,
		RAY_TRACING
	};

	struct PipelineSpecification
	{
		PipelineType type;
		uint32_t window_width;
		uint32_t window_height;
	};

	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline();
		VulkanPipeline(const PipelineSpecification&);
		virtual ~VulkanPipeline();

		void Initialize() override;
		void Shutdown() override;
		void Recreate() override;

		VkPipeline GetRaw() const { return m_Pipeline; }
		const PipelineSpecification& GetSpec() const { return m_Spec; }

	private:
		void CreateGraphics();
		void CreateCompute();
		void CreateRayTracing();

	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_Layout;
		PipelineSpecification m_Spec;

		std::vector<VkDynamicState> m_DynamicStates =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
	};

}