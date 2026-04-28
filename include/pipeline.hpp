#ifndef PIPELINE_HPP
#define PIPELINE_HPP

namespace Celer {

	namespace Core {

		class Pipeline {

			public:
				vk::raii::PipelineLayout mPipelineLayout{ nullptr };
				vk::raii::Pipeline mPipeline{ nullptr };

		};

	}
}

#endif // PIPELINE_HPP

