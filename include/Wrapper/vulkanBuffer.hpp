#ifndef VULKANBUFFER_HPP
#define VULKANBUFFER_HPP

namespace Celer {
	namespace Wrapper {

		class Buffer {

			private:
				vk::raii::Buffer vkBuff{ nullptr };
				vk::raii::DeviceMemory vkDeviceMem{ nullptr };

			public:


		};

	}
}

#endif // VULKANBUFFER_HPP