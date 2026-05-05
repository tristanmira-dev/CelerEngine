#ifndef PIPELINEBUILDER_HPP
#define PIPELINEBUILDER_HPP

namespace Celer {
	/*INCLUDES BOILERPLATE DEFAULT PIPELINE BUILDER OBJECTS!*/
	namespace Core {
		class PipelineBuilder {
			public:

				vk::raii::ShaderModule mShaderModule{ nullptr };

				std::vector<vk::PipelineShaderStageCreateInfo> mShadersStage;


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
				vk::PipelineColorBlendStateCreateInfo mColorBlending;


				vk::PushConstantRange mPushConstRange;

				/*Pipeline Layout*/
				vk::PipelineLayoutCreateInfo mLayoutCreateInfo;

				vk::Format mDepthFormat;

				/*Member functions*/

				/**
				 * @brief Initializes pipeline state CreateInfo structs with default values.
				 *
				 * Sets up the following fixed-function pipeline stages:
				 * - **Dynamic State**: Viewport and scissor set as dynamic
				 * - **Input Assembly**: Triangle list topology
				 * - **Viewport State**: Single viewport/scissor (dynamic, no static binding)
				 * - **Rasterizer**: Fill mode, no culling, CCW front face, no depth bias
				 * - **Multisampling**: Single sample, no sample shading
				 * - **Color Blending**: Blending disabled, full RGBA write mask
				 *
				 * @note Vertex input, depth/stencil, and push constants are disabled by default (see commented blocks).
				*/
				PipelineBuilder();

				/**/


				void createShader(std::string const& path, std::vector<vk::PipelineShaderStageCreateInfo>& shaderStages, vk::raii::Device &device);

				vk::raii::ShaderModule createShaderModule(std::vector<char> const& shaderData, vk::raii::Device& device) const;



		};
	}

}



#endif // PIPELINEBUILDER_HPP