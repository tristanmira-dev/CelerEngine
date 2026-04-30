#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "pipelineBuilder.hpp"

namespace Celer {

	namespace Core {

		class Pipeline {

			vk::raii::PipelineLayout mPipelineLayout{ nullptr };
			vk::raii::Pipeline mPipeline{ nullptr };

			public:
				

				Pipeline(PipelineBuilder const& pipelineBuilder, vk::raii::Device& device);

		};

	}
}

#endif // PIPELINE_HPP

