#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "pipelineBuilder.hpp"
#include "deviceMemManager.hpp"


namespace Celer {

	namespace Render {

		class Pipeline {

			const int MAX_FRAMES_IN_FLIGHT{ 2 };

			vk::raii::PipelineLayout mPipelineLayout{ nullptr };
			vk::raii::Pipeline mPipeline{ nullptr };

			vk::raii::DescriptorSetLayout mDescriptorSetLayout{ nullptr };
			vk::raii::DescriptorPool mDescriptorPool{ nullptr };
			void createDescriptorSet(vk::raii::Device& device);


			public:

				std::vector<vk::raii::DescriptorSet> mDescriptorSets;

				void updateDescriptorImage(vk::raii::Device& device, vk::raii::ImageView& imageView, vk::raii::Sampler& sampler, uint32_t idx);


				template<typename T>
				void updateBufferDescriptors(vk::raii::Device& device, std::vector<Core::Memory>& descriptors, vk::Buffer buffer, vk::DescriptorType type, uint32_t binding) {

					for (int i{}; i < MAX_FRAMES_IN_FLIGHT; ++i) {

						vk::DescriptorBufferInfo bufferInfo{ .buffer = buffer, .offset = descriptors[i].getOffset(),.range = sizeof(T) };

						vk::WriteDescriptorSet descriptorWrites{ .dstSet = mDescriptorSets[i], .dstBinding = binding, .dstArrayElement = 0, .descriptorCount = 1, .descriptorType = type, .pBufferInfo = &bufferInfo };

						device.updateDescriptorSets(descriptorWrites, {});

					}

				}

				
				Pipeline() = default;
				void setPipeline(PipelineBuilder& pipelineBuilder, vk::raii::Device& device, vk::SurfaceFormatKHR &surfaceFormat);
				inline vk::raii::Pipeline& getPipeline() {
					return mPipeline;
				}
				
				inline vk::raii::PipelineLayout& getLayout() {
					return mPipelineLayout;
				}

		};

	}
}

#endif // PIPELINE_HPP

