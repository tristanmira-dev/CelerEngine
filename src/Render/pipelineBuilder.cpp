#include "pipelineBuilder.hpp"
#include <fstream>

namespace {

	std::vector<char> readFile(const std::string& fileName);

}

namespace Celer {
	namespace Render {
		PipelineBuilder::PipelineBuilder() {

			/*DYNAMIC STATES---------------*/
			mDynamicStates = std::vector<vk::DynamicState>{ vk::DynamicState::eViewport, vk::DynamicState::eScissor }; //This will cause the configuration of these values to be ignored, and you will be able (and required) to specify the data at drawing time
			mDynamicState = vk::PipelineDynamicStateCreateInfo { .dynamicStateCount = static_cast<uint32_t>(mDynamicStates.size()), .pDynamicStates = mDynamicStates.data() };
			
			/*Vertex input*/
			mVertexInputInfo = vk::PipelineVertexInputStateCreateInfo{};

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

			
			/*Rasterizer*/
			mRasterizer = vk::PipelineRasterizationStateCreateInfo { .depthClampEnable = vk::False, .rasterizerDiscardEnable = vk::False, .polygonMode = vk::PolygonMode::eFill, .cullMode = vk::CullModeFlagBits::eNone,
				.frontFace = vk::FrontFace::eCounterClockwise, .depthBiasEnable = vk::False, .depthBiasSlopeFactor = 1.f, .lineWidth = 1.f
			};

			/*Multisampling*/
			mMultisampling = vk::PipelineMultisampleStateCreateInfo { .rasterizationSamples = vk::SampleCountFlagBits::e1, .sampleShadingEnable = vk::False };

			/*Color Blending*/
			/*color blending is essentially just for either using whats already on the framebuffer and blending it with the fragment shader output or just taking the framebuffer output directly*/
			vk::PipelineColorBlendAttachmentState colorBlendAttachment{ .blendEnable = vk::False, .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };
			mColorBlending = vk::PipelineColorBlendStateCreateInfo{ .logicOpEnable = vk::False, .logicOp = vk::LogicOp::eCopy, .attachmentCount = 1, .pAttachments = &colorBlendAttachment };

		}
		void PipelineBuilder::createShader(std::string const& path, std::vector<vk::PipelineShaderStageCreateInfo> & shaderStages, vk::raii::Device& device) {
		
			mShaderModule = createShaderModule(readFile(path), device);

			std::for_each(shaderStages.begin(), shaderStages.end(), [this](vk::PipelineShaderStageCreateInfo &stageCreateInfo) {
				stageCreateInfo.module = this->mShaderModule;
			});

			mShadersStage = shaderStages;

		}

		vk::raii::ShaderModule PipelineBuilder::createShaderModule(std::vector<char> const& shaderData, vk::raii::Device& device) const {
			vk::ShaderModuleCreateInfo createInfo{ .codeSize = shaderData.size() * sizeof(char), .pCode = reinterpret_cast<const uint32_t*>(shaderData.data()) };
			return vk::raii::ShaderModule{ device, createInfo };
		}
	}
	
}

namespace {
	std::vector<char> readFile(const std::string& fileName) {
		std::ifstream file(fileName, std::ios::ate /*Useful for getting the file size*/ | std::ios::binary);

		if (!file) {
			throw std::runtime_error{ "Failed to load Shader" };
		}

		std::vector<char> buffer(file.tellg());

		file.seekg(0);
		file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

		file.close(); /*file will close automatically but whatevs*/
		return buffer;

	}
}