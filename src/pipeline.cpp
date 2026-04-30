#include "pipeline.hpp"

namespace Celer {
	namespace Core {
		Pipeline::Pipeline(PipelineBuilder const& pipelineBuilder, vk::raii::Device &device) {
			//vk::PipelineLayoutCreateInfo layoutCreateInfo{ .setLayoutCount = 1, .pSetLayouts = &*pipelineBuilder.mdesc ,.pushConstantRangeCount = 1, .pPushConstantRanges = &pushConstRange }; /*"my shaders don't use any uniforms or push constants right now."*/
			//mPipelineLayout = vk::raii::PipelineLayout(device, layoutCreateInfo);

			//vk::Format depthFormat{ findDepthFormat() };

			///*Rendering, Pipeline creation*/
			//vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> chain{
			//	{
			//		.stageCount = pipelineBuilder.sgader, .pStages = shadersStage, .pVertexInputState = &vertexInputInfo, .pInputAssemblyState = &inputAssemblyInfo,
			//		.pViewportState = &viewportState, .pRasterizationState = &rasterizer, .pMultisampleState = &multisampling,

			//		.pDepthStencilState = &depthStencil,
			//		.pColorBlendState = &colorBlending,
			//		.pDynamicState = &dynamicState,
			//		.layout = pipelineLayout,
			//		.renderPass = nullptr /*Note that the renderPass parameter is set to nullptr because we’re using dynamic rendering instead of a traditional render pass.*/
			//	},
			//	{.colorAttachmentCount = 1, .pColorAttachmentFormats = &swapChainSurfaceFormat.format, .depthAttachmentFormat = depthFormat}
			//};

			///*


			//	Shader stages: the shader modules that define the functionality of the programmable stages of the graphics pipeline

			//	Fixed-function state: all the structures that define the fixed-function stages of the pipeline, like input assembly, rasterizer, viewport and color blending

			//	Pipeline layout: the uniform and push values referenced by the shader that can be updated at draw time

			//	Dynamic rendering: the formats of the attachments that will be used during rendering


			//*/

			//mPipeline = vk::raii::Pipeline(device, nullptr, chain.get<vk::GraphicsPipelineCreateInfo>());
		
		}
	}
}