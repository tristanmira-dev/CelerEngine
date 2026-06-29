#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "pipelineBuilder.hpp"

namespace Celer {

	namespace Render {

		class Pipeline {

			vk::raii::PipelineLayout mPipelineLayout{ nullptr };
			vk::raii::Pipeline mPipeline{ nullptr };

			

			public:
				
				Pipeline() = default;
				void setPipeline(PipelineBuilder const& pipelineBuilder, vk::raii::Device& device, vk::SurfaceFormatKHR &surfaceFormat);
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

