#ifndef CAMERA_H
#define CAMERA_H

#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"
#include "glad/glad.h"
#include <string>
#include <vector>

class Camera
{
    public:
        Camera(float rot_speed = 0.7f, float mov_speed = 0.3f);
         ~Camera();

        void updateProjMatUniform(GLuint programID, std::string uniform_name);
        void updateViewMatUniform(GLuint programID, std::string uniform_name);
        void setOrientation(float zoom, float zenith, float azimuth);
        void setProjectionMatrix(float fov_y, float width, float height, float near, float far);
        void setViewMatrix(glm::vec4 eye, glm::vec4 side, glm::vec4 up, glm::vec4 lookat_pos);
        void setUBO(std::vector<float>& cam_data);
        bool is_changed;
        glm::vec4 lookat_pos;
        glm::vec4 look_at;
        glm::vec4 side;
        glm::vec4 up;
        glm::vec4 eye;
        glm::mat4 rot_mat;
        glm::mat4 proj_mat;
        static Camera* main_cam;

    private:
        float view_plane_dist, y_FOV,
        rotation_speed, mov_speed, zenith, azimuth, radius, tot_zenith, tot_azimuth, tot2_azimuth;
        glm::mat4 view2world_mat;
        glm::mat4 world2view_mat;



};

#endif // CAMERA_H
