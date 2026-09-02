#include "pipeline.hpp"

namespace Celer {
	namespace Render {
		void Pipeline::createDescriptorSet(vk::raii::Device &device) {

			std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, mDescriptorSetLayout);

			vk::DescriptorSetAllocateInfo allocInfo{ .descriptorPool = *mDescriptorPool, .descriptorSetCount = static_cast<uint32_t>(layouts.size()), .pSetLayouts = layouts.data() };

			mDescriptorSets = device.allocateDescriptorSets(allocInfo);

		}

		void Pipeline::updateDescriptorImage(vk::raii::Device &device, vk::raii::ImageView& imageView, vk::raii::Sampler& sampler, uint32_t idx) {
			for (int i{}; i < MAX_FRAMES_IN_FLIGHT; ++i) {

				vk::DescriptorImageInfo imageInfo{ .sampler = sampler, .imageView = imageView, .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal };


				vk::WriteDescriptorSet writeSet{ .dstArrayElement = idx, .descriptorCount = 1 };

				writeSet.dstSet = mDescriptorSets[i];
				writeSet.dstBinding = 0;

				writeSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
				writeSet.pImageInfo = &imageInfo;
				
				device.updateDescriptorSets(writeSet, {});

			}
		}

		void Pipeline::setPipeline(PipelineBuilder &pipelineBuilder, vk::raii::Device &device, vk::SurfaceFormatKHR& swapchainSurfaceFormat) {
			vk::PipelineLayoutCreateInfo layoutCreateInfo{ .pSetLayouts = nullptr ,.pushConstantRangeCount = 1, .pPushConstantRanges = &pipelineBuilder.pushConsts };

			mDescriptorSetLayout = vk::raii::DescriptorSetLayout(device, pipelineBuilder.mDescriptorSetLayoutCreateInfo);

			layoutCreateInfo.setLayoutCount = 1;

			layoutCreateInfo.pSetLayouts = &*mDescriptorSetLayout;

			mPipelineLayout = vk::raii::PipelineLayout(device, layoutCreateInfo);

			mDescriptorPool = vk::raii::DescriptorPool(device, pipelineBuilder.mDescriptorPoolCreateInfo);

			createDescriptorSet(device);

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