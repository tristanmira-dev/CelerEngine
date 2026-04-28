#ifndef PIPELINEBUILDER_HPP
#define PIPELINEBUILDER_HPP

namespace Celer {
	/*INCLUDES BOILERPLATE DEFAULT PIPELINE BUILDER OBJECTS!*/
	namespace Core {
		class PipelineBuilder {
			public:

				vk::raii::ShaderModule mShaderModule{ nullptr };

				vk::PipelineShaderStageCreateInfo mVertShaderStageInfo;
				vk::PipelineShaderStageCreateInfo mFragShaderStageInfo;

				vk::PipelineShaderStageCreateInfo mShadersStage;


				/*DYNAMIC STATES---------------*/
				std::vector<vk::DynamicState> mDynamicStates;
				vk::PipelineDynamicStateCreateInfo mDynamicState;


				/*Vertex input----------------*/
				vk::PipelineVertexInputStateCreateInfo mVertexInputInfo;


				/*Input Assembly*/
				vk::PipelineInputAssemblyStateCreateInfo mInputAssemblyInfo;


				vk::PipelineViewportStateCreateInfo mViewportState;

				/*Rasterizer*/
				vk::PipelineRasterizationStateCreateInfo mRasterizer;


				/*Multisampling*/
				vk::PipelineMultisampleStateCreateInfo mMultisampling;


				/*Depth and stencil testing*/
				vk::PipelineDepthStencilStateCreateInfo mDepthStencil;

				/*Color Blending*/
				vk::PipelineColorBlendAttachmentState mColorBlendAttachment;
				vk::PipelineColorBlendStateCreateInfo mColorBlending;


				vk::PushConstantRange mPushConstRange;

				/*Pipeline Layout*/
				vk::PipelineLayoutCreateInfo mLayoutCreateInfo;

				vk::Format mDepthFormat;

				/*Member functions*/

				PipelineBuilder();

				/**/



		};
	}

}



#endif // PIPELINEBUILDER_HPP