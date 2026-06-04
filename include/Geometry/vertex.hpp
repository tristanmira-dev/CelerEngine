#ifndef VERTEX_HPP
#define VERTEX_HPP

namespace Celer {
	namespace Geometry {

		struct Vertex {
			glm::vec3 position;
			glm::vec3 color;
			//glm::vec2 uv{ 0.f, 0.f };

			static vk::VertexInputBindingDescription getBindingDescription() {
				return { 0, sizeof(Vertex), vk::VertexInputRate::eVertex };
			}

			static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescription() {
				return {
					vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) },
					vk::VertexInputAttributeDescription{ 1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color) },
					//vk::VertexInputAttributeDescription{ 2, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, uv) }
				};
			}
		};
	}
}


#endif // VERTEX_HPP