#include "pipeline.hpp"

namespace Celer {
	namespace Render {
		void Pipeline::setPipeline(PipelineBuilder const& pipelineBuilder, vk::raii::Device &device, vk::SurfaceFormatKHR& swapchainSurfaceFormat) {
			vk::PipelineLayoutCreateInfo layoutCreateInfo{ .setLayoutCount = 0, .pSetLayouts = nullptr ,.pushConstantRangeCount = 0, .pPushConstantRanges = &pipelineBuilder.pushConsts }; /*"my shaders don't use any uniforms or push constants right now."*/
			mPipelineLayout = vk::raii::PipelineLayout(device, layoutCreateInfo);

			//vk::Format depthFormat{ findDepthFormat() };


			vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> chain{
				{
					.stageCount = static_cast<uint32_t>(pipelineBuilder.mShadersStage.size()), 
					.pStages = pipelineBuilder.mShadersStage.data(),
					.pVertexInputState = &pipelineBuilder.mVertexInputInfo,
					.pInputAssemblyState = &pipelineBuilder.mInputAssemblyInfo,
					.pViewportState = &pipelineBuilder.mViewportState, 
					.pRasterizationState = &pipelineBuilder.mRasterizer, 
					.pMultisampleState = &pipelineBuilder.mMultisampling,
					.pDepthStencilState = &pipelineBuilder.mDepthStencil,
					.pColorBlendState = &pipelineBuilder.mColorBlending,
					.pDynamicState = &pipelineBuilder.mDynamicState,
					.layout = mPipelineLayout,
					.renderPass = nullptr /*Note that the renderPass parameter is set to nullptr because we’re using dynamic rendering instead of a traditional render pass.*/
				},
				{.colorAttachmentCount = 1, .pColorAttachmentFormats = &swapchainSurfaceFormat.format }
			};

			/*


				Shader stages: the shader modules that define the functionality of the programmable stages of the graphics pipeline

				Fixed-function state: all the structures that define the fixed-function stages of the pipeline, like input assembly, rasterizer, viewport and color blending

				Pipeline layout: the uniform and push values referenced by the shader that can be updated at draw time

				Dynamic rendering: the formats of the attachments that will be used during rendering


			*/

			mPipeline = vk::raii::Pipeline(device, nullptr, chain.get<vk::GraphicsPipelineCreateInfo>());
		
		}
	}
}