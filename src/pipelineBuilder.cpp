#include "pipelineBuilder.hpp"

namespace Celer {
	namespace Core {
		PipelineBuilder::PipelineBuilder() {

			/*DYNAMIC STATES---------------*/
			std::vector<vk::DynamicState> dynamicStates{ vk::DynamicState::eViewport, vk::DynamicState::eScissor }; //This will cause the configuration of these values to be ignored, and you will be able (and required) to specify the data at drawing time
			mDynamicState = vk::PipelineDynamicStateCreateInfo { .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()), .pDynamicStates = dynamicStates.data() };
			

			/*Vertex input----------------*/
			//auto bindingDescription{ Vertex::getBindingDescription() };
			//auto attributeDescription{ Vertex::getAttributeDescription() };



			//vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
			//	.vertexBindingDescriptionCount = 1,
			//	.pVertexBindingDescriptions = &bindingDescription,
			//	.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size()),
			//	.pVertexAttributeDescriptions = attributeDescription.data()
			//};



			/*Input Assembly(Type of geometry to draw)*/
			mInputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo{ .topology = vk::PrimitiveTopology::eTriangleList };

			/*Viewports and Scissors*/
				/*Viewport = where to draw
				Scissor = what to keep*/
				/*insert here if you want a static, unchanging viewport or scissor
					vk::PipelineViewportStateCreateInfo viewportState(
						.viewportCount = 1,
						.pViewports = &viewport,
						.scissorCount = 1,
						.pScissors = &scissor
					);
				*/
			mViewportState = vk::PipelineViewportStateCreateInfo{ .viewportCount = 1, .scissorCount = 1 };

			/*Independent of how you set them, it’s possible to use multiple viewports and scissor rectangles on some graphics cards, so the structure members reference an array of them. Using multiple requires enabling a GPU feature (see logical device creation).*/


			/*Rasterizer*/
			mRasterizer = vk::PipelineRasterizationStateCreateInfo { .depthClampEnable = vk::False, .rasterizerDiscardEnable = vk::False, .polygonMode = vk::PolygonMode::eFill, .cullMode = vk::CullModeFlagBits::eNone,
				.frontFace = vk::FrontFace::eCounterClockwise, .depthBiasEnable = vk::False, .depthBiasSlopeFactor = 1.f, .lineWidth = 1.f
			};

			/*Multisampling*/
			mMultisampling = vk::PipelineMultisampleStateCreateInfo { .rasterizationSamples = vk::SampleCountFlagBits::e1, .sampleShadingEnable = vk::False };

			///*Depth and stencil testing*/
			///*VkPipelineDepthStencilStateCreateInfo. We don’t have one right now, so we can simply pass a nullptr instead of a pointer to such a struct. We’ll get back to it in the depth buffering chapter.*/
			//vk::PipelineDepthStencilStateCreateInfo depthStencil{
			//	.depthTestEnable = vk::True,
			//	.depthWriteEnable = vk::True,
			//	.depthCompareOp = vk::CompareOp::eLess,
			//	.depthBoundsTestEnable = vk::False,
			//	.stencilTestEnable = vk::False
			//};

			/*Color Blending*/
			/*color blending is essentially just for either using whats already on the framebuffer and blending it with the fragment shader output or just taking the framebuffer output directly*/
			vk::PipelineColorBlendAttachmentState colorBlendAttachment{ .blendEnable = vk::False, .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };
			mColorBlending = vk::PipelineColorBlendStateCreateInfo{ .logicOpEnable = vk::False, .logicOp = vk::LogicOp::eCopy, .attachmentCount = 1, .pAttachments = &colorBlendAttachment };

			//vk::PushConstantRange pushConstRange{
			//	.stageFlags = vk::ShaderStageFlagBits::eVertex,
			//	.offset = 0,
			//	.size = sizeof(int),
			//};
		}
	}
	
}