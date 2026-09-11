#ifndef GEOMETRY_DESCRIPTORS_HPP
#define GEOMETRY_DESCRIPTORS_HPP

namespace Celer {
    namespace Geometry {
        struct GameObjectProperties {
            glm::mat4 transform;
            uint32_t textureIdx;
        };

        struct ViewProperties {
            glm::mat4 projection;
            glm::mat4 model;
            glm::mat4 view;
        };
    }

}



#endif