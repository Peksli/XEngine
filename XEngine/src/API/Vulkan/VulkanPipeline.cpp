#include "src/API/Vulkan/VulkanPipeline.h"
#include "src/API/Vulkan/VulkanContext.h"
#include "src/Utility/Utility.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	VulkanPipeline::VulkanPipeline()
	{

	}

	VulkanPipeline::VulkanPipeline(const PipelineSpecification& spec)
		: m_Spec(spec)
	{

	}

	VulkanPipeline::~VulkanPipeline()
	{

	}

	void VulkanPipeline::Initialize()
	{
		switch (m_Spec.type)
		{
		case PipelineType::GRAPHICS:	CreateGraphics();	break;
		case PipelineType::COMPUTE:		CreateCompute();	break;
		case PipelineType::RAY_TRACING: CreateRayTracing(); break;
		default: std::unreachable(); break;
		}
	}

	void VulkanPipeline::Shutdown()
	{
		VulkanContext* ctx = VulkanContext::GetRaw();

		vkDestroyPipelineLayout(*ctx->GetDevice(), m_Layout, nullptr);
		// vkDestroyPipeline(*ctx->GetDevice(), m_Pipeline, nullptr);
	}

	void VulkanPipeline::Recreate()
	{

	}

	void VulkanPipeline::CreateGraphics()
	{
		VulkanContext* ctx = VulkanContext::GetRaw();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.pNext = nullptr;
		vertexInputInfo.flags = 0;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.pNext = nullptr;
		inputAssemblyInfo.flags = 0;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyInfo.primitiveRestartEnable = VK_FALSE; // if true -> strip topology

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = m_Spec.window_width;
		viewport.height = m_Spec.window_height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkExtent2D extent{};
		extent.width = m_Spec.window_width;
		extent.height = m_Spec.window_height;

		VkRect2D scissor{};
		scissor.offset = { 0,0 };
		scissor.extent = extent;

		VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.pNext = nullptr;
		dynamicStateInfo.flags = 0;
		dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(m_DynamicStates.size());
		dynamicStateInfo.pDynamicStates = m_DynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportStateInfo = {};
		viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateInfo.viewportCount = 1;
		viewportStateInfo.pViewports = &viewport;
		viewportStateInfo.scissorCount = 1;
		viewportStateInfo.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizerStateInfo = {};
		rasterizerStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerStateInfo.pNext = nullptr;
		rasterizerStateInfo.flags = 0;
		rasterizerStateInfo.depthClampEnable = VK_FALSE;
		rasterizerStateInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizerStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizerStateInfo.lineWidth = 1.0f;
		rasterizerStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizerStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizerStateInfo.depthBiasEnable = VK_FALSE;
		rasterizerStateInfo.depthBiasConstantFactor = 0.0f;
		rasterizerStateInfo.depthBiasClamp = 0.0f;
		rasterizerStateInfo.depthBiasSlopeFactor = 0.0f;

		VkPipelineMultisampleStateCreateInfo multisampleStateInfo = {};
		multisampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleStateInfo.sampleShadingEnable = VK_FALSE;
		multisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleStateInfo.minSampleShading = 1.0f;
		multisampleStateInfo.pSampleMask = nullptr;
		multisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
		multisampleStateInfo.alphaToOneEnable = VK_FALSE;

		VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo;

		VkPipelineColorBlendAttachmentState colorBlendFramebufferStateInfo = {};
		colorBlendFramebufferStateInfo.colorWriteMask = 
			VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		colorBlendFramebufferStateInfo.blendEnable = VK_FALSE;
		colorBlendFramebufferStateInfo.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; 
		colorBlendFramebufferStateInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; 
		colorBlendFramebufferStateInfo.colorBlendOp = VK_BLEND_OP_ADD; 
		colorBlendFramebufferStateInfo.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; 
		colorBlendFramebufferStateInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; 
		colorBlendFramebufferStateInfo.alphaBlendOp = VK_BLEND_OP_ADD; 

		VkPipelineColorBlendStateCreateInfo colorBlendStateInfo = {};
		colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendStateInfo.pNext = nullptr;
		colorBlendStateInfo.flags = 0;
		colorBlendStateInfo.logicOpEnable = VK_FALSE;
		colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
		colorBlendStateInfo.attachmentCount = 1;
		colorBlendStateInfo.pAttachments = &colorBlendFramebufferStateInfo;
		colorBlendStateInfo.blendConstants[0] = 0.0f;
		colorBlendStateInfo.blendConstants[1] = 0.0f;
		colorBlendStateInfo.blendConstants[2] = 0.0f;
		colorBlendStateInfo.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		CHECK_VK_RES(
			vkCreatePipelineLayout(
				*ctx->GetDevice(),
				&pipelineLayoutInfo, 
				nullptr, 
				&m_Layout)
		);
	}

	void VulkanPipeline::CreateCompute()
	{

	}

	void VulkanPipeline::CreateRayTracing()
	{

	}

}