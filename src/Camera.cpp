#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/constants.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>

Camera* Camera::main_cam = nullptr;
Camera::Camera(float rot_speed, float mov_speed) : rotation_speed(rot_speed), mov_speed(mov_speed)
{

    //view_plane_dist =  1/tan(y_FOV * glm::pi<float>()/360);
    proj_mat = glm::mat4(1.0);
    world2view_mat = glm::mat4(1.0);
    is_changed = true;
    zenith = glm::pi<float>()/2.0;
    azimuth = 0;
    tot_zenith = 0;
    tot2_azimuth = tot_azimuth =0;
    rot_mat = glm::mat4(1.0f);
    //ctor
}

Camera::~Camera()
{

}

void Camera::updateProjMatUniform(GLuint programID, std::string uniform_name)
{
    glUniformMatrix4fv(glGetUniformLocation(programID, uniform_name.c_str()), 1, GL_FALSE, &proj_mat[0][0]);
}

void Camera::updateViewMatUniform(GLuint programID, std::string uniform_name)
{
    glUniformMatrix4fv(glGetUniformLocation(programID, uniform_name.c_str()), 1, GL_FALSE, &world2view_mat[0][0]);
    is_changed = false;
}




void Camera::setProjectionMatrix(float fov_y, float width, float height, float znear, float zfar)
{
    proj_mat = glm::perspective(glm::radians(fov_y), width/height, znear, zfar);
}

void Camera::setViewMatrix(glm::vec4 eye, glm::vec4 side, glm::vec4 up, glm::vec4 lookat_pos)
{
    this->eye = eye;
    this->side = glm::normalize(side);
    this->up = glm::normalize(up);
    this->lookat_pos = lookat_pos;
    this->look_at = glm::normalize(lookat_pos - eye);

    /* Always Remember that for Right Handed Coordinate Systems, the Camera (initially aligned with World Reference frame)
     * has the look direction negative to that of the Z axis. Hence to get the basis vector in Z we have to invert the look vector.
     */
    world2view_mat = glm::lookAt(glm::vec3(eye), glm::vec3(lookat_pos), glm::vec3(up));
    view2world_mat = glm::mat4(side, up, -look_at, eye);
    radius = glm::length(eye - lookat_pos);
}

void Camera::setOrientation(float zoom , float zenith, float azimuth)
{
    if(zenith == 0 && azimuth == 0)
    {
        if(zoom > 0)
            eye += look_at;
        else
            eye -= look_at;
        radius =  glm::length(eye - lookat_pos);
        setViewMatrix(eye, side, up, lookat_pos);
        is_changed = true;
        return;
    }
    float new_zenith, new_azimuth;
    float pi2 = glm::pi<float>() * 2;

    new_zenith = glm::clamp(this->zenith + zenith * rotation_speed, 0.0f, glm::pi<float>());
    new_azimuth = this->azimuth + azimuth * rotation_speed;

    //Wrap Azimuth angle around 360 degrees.
    if(new_azimuth < 0)
        new_azimuth = pi2 - new_azimuth;
    else if(new_azimuth > pi2)
        new_azimuth = new_azimuth - pi2;

    //If no change in angle return
    if(new_zenith == this->zenith && new_azimuth == this->azimuth)
        return;

    this->zenith = new_zenith;
    this->azimuth = new_azimuth;

    /* Spherical to Cartesian. The Cartesian Coordinate Reference Frame used is the one used in Computer Graphics ( Z points forward, Y upwards)
     * rather than normal Mathematics where Z points upwards.
     *
     *  y = r * cos(zenith)
     *  z = r * sin(zenith) * cos(azimuth)
     *  x = r * sin(zenith) * sin(azimuth)
     */
    eye.x = this->radius * glm::sin(this->zenith) * glm::sin(this->azimuth);
    eye.y = this->radius * glm::cos(this->zenith);
    eye.z = this->radius * glm::sin(this->zenith) * glm::cos(this->azimuth);
    eye.w = 1;

    eye +=  lookat_pos; // new


    //look_at = glm::vec4(-eye);
    look_at =  lookat_pos - eye;
    look_at.w = 0;
    look_at = glm::normalize(look_at);

    /* When Zenith is 0 or PI, the look at vector collapses on to the world up.
     * We compute the side vector as (1,0,0) and rotate it around the world up based on
     * azimuth angle.
     */
    if(this->zenith == 0 || this->zenith == glm::pi<float>() )
    {
        side = glm::vec4(1,0,0,0);
        glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0), this->azimuth, glm::vec3(0,1,0));
        side = rot_mat * side;
    }
    else
        side = glm::vec4(glm::cross(glm::vec3(look_at.x, look_at.y, look_at.z), glm::vec3(0,1,0)), 0);

    up = glm::vec4(glm::cross(glm::vec3(side.x, side.y, side.z), glm::vec3(look_at.x, look_at.y, look_at.z)), 0);
    side = glm::normalize(side);
    up = glm::normalize(up);

    setViewMatrix(eye, side, up, lookat_pos);
    is_changed = true;
}
