#ifndef PIPELINEBUILDER_HPP
#define PIPELINEBUILDER_HPP

namespace Celer {
	/*INCLUDES BOILERPLATE DEFAULT PIPELINE BUILDER OBJECTS!*/
	namespace Render {
		class PipelineBuilder {
			constexpr static int MAX_FRAMES_IN_FLIGHT{ 2 };

			private:

				void createDescriptorPool();

				void createDescriptorLayout();

				void createDescriptorSet();

				void createDescriptorWriteSet();

			public:

				vk::raii::ShaderModule mShaderModule{ nullptr };

				std::vector<vk::PipelineShaderStageCreateInfo> mShadersStage;

				/*DYNAMIC STATES---------------*/
				std::vector<vk::DynamicState> mDynamicStates;
				vk::PipelineDynamicStateCreateInfo mDynamicState;

				/*Push Constants--------------*/
				vk::PushConstantRange pushConsts;


				/*Vertex input----------------*/
				vk::PipelineVertexInputStateCreateInfo mVertexInputInfo;
				std::vector<vk::VertexInputAttributeDescription> mVertexAttrDesc;
				vk::VertexInputBindingDescription mVertexBindingDesc;
				

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
				vk::PipelineColorBlendAttachmentState mColorBlendAttachment;

				vk::PushConstantRange mPushConstRange;

				/*Pipeline Layout*/
				vk::PipelineLayoutCreateInfo mLayoutCreateInfo;

				vk::Format mDepthFormat;

				/*Descriptors*/
				vk::DescriptorSetLayoutCreateInfo mDescriptorSetLayoutCreateInfo;
				std::vector<vk::DescriptorPoolSize> mDescriptorPoolSize;
				std::vector<vk::DescriptorSetLayoutBinding> mDescriptorSetLayoutBinding;
				std::vector<vk::WriteDescriptorSet> mDescriptorWriteSet;


				vk::DescriptorPoolCreateInfo mDescriptorPoolCreateInfo;

				/*Member functions*/

				/**
				 * @brief Initializes pipeline state CreateInfo structs with default values.
				 *
				 *
				*/
				PipelineBuilder();

				/**/

				void addShaderStage(vk::ShaderStageFlagBits stage, const char *name);


				void createShader(std::string const& path, vk::raii::Device &device);

				void addVertexBufferInfo();


				vk::raii::ShaderModule createShaderModule(std::vector<char> const& shaderData, vk::raii::Device& device) const;



		};
	}

}



#endif // PIPELINEBUILDER_HPP