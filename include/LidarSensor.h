#ifndef LIDARSENSOR_H
#define LIDARSENSOR_H

#include "Ray.h"
#include "glm/glm.hpp"
#include <Primitive.h>
#include <vector>
#include <memory>

class LidarSensor
{
    public:
        LidarSensor();
        ~LidarSensor();
        void init(glm::vec4 pos, glm::vec4 side, glm::vec4 up, glm::vec4 look_at, float fov_x, float fov_y, int rays_x, int rays_y);
        void shootRays(std::vector<std::unique_ptr<Primitive>>& prim_list, std::vector<glm::vec3>& vertex_data);

    private:
        glm::vec4 pos;
        glm::vec4 side;
        glm::vec4 up;
        glm::vec4 look_at;
        float fov_x, fov_y;
        int rays_x, rays_y;
        std::vector<Ray> rays;
        void createRays();

};

#endif // LIDARSENSOR_H
