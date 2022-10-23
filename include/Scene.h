#ifndef SCENE_H
#define SCENE_H

#include "Primitive.h"
#include "Quad.h"
#include "Sphere.h"
#include "Camera.h"
#include "ShaderUtils.h"
#include "LidarSensor.h"
#include "glm/glm.hpp"

#include <vector>
#include <memory>

class Scene
{
    public:
        Scene();
        ~Scene();
        void createShaderProgram();
        void render();
        void createScene();

    private:
        unsigned int vao, vbo, vs, fs, shader_program;
        std::string vs_text, fs_text;
        std::vector<std::unique_ptr<Primitive>> primitive_list;
        std::vector<glm::vec3> pc_vertex_data;
        Camera main_cam;
        LidarSensor lidar;
};

#endif // SCENE_H
