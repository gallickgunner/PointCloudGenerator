#include "LidarSensor.h"
#include "limits"
#include <iostream>
#include "HitInfo.h"

LidarSensor::LidarSensor()
{
    //ctor
}

LidarSensor::~LidarSensor()
{
    //dtor
}


void LidarSensor::init(glm::vec4 pos, glm::vec4 side, glm::vec4 up, glm::vec4 look_at, float fov_x, float fov_y, int rays_x, int rays_y)
{
    this->pos = pos;
    this->side = glm::normalize(side);
    this->up = glm::normalize(up);
    this->look_at = glm::normalize(look_at);
    this->fov_x = glm::radians(fov_x);
    this->fov_y = glm::radians(fov_y);
    this->rays_x = rays_x;
    this->rays_y = rays_y;
    createRays();
}

void LidarSensor::createRays()
{
    float infinity = std::numeric_limits<float>::infinity();
    float focal_length = 1.0f;

    float half_width = glm::tan(fov_x/2.0f) * focal_length;
    float half_height = glm::tan(fov_y/2.0f) * focal_length;
    float aspect = half_width/half_height;

    //rays_x = half_width * 2;
    //rays_y = half_height * 2;
    float increment_x, increment_y;
    increment_x = 2.0f / (rays_x - 1);
    increment_y = 2.0f / (rays_y - 1);

    Ray upper_left(pos, glm::vec4(1.0), infinity), upper_right = upper_left, lower_left = upper_left, lower_right = upper_left;

    upper_left.dir = glm::normalize((-1.0f * side * half_width) + (1.0f * up * half_height) + (focal_length * look_at));
    upper_right.dir = glm::normalize((1.0f * side * half_width) + (1.0f * up * half_height) + (focal_length * look_at));
    lower_left.dir = glm::normalize((-1.0f * side * half_width) + (-1.0f * up * half_height) + (focal_length * look_at));
    lower_right.dir = glm::normalize((1.0f * side * half_width) + (-1.0f * up * half_height) + (focal_length * look_at));

    float x = -1.0f, y = 1.0f; //start from top right corner
    for(int i = 0; i < rays_y; i++)
    {
        for(int j = 0; j < rays_x; j++)
        {
            Ray ray(pos, glm::vec4(1.0), infinity);
            ray.dir = glm::normalize((x * side * half_width) + (y * up * half_height) + (focal_length * look_at));
            rays.push_back(ray);
            x += increment_x;
        }
        y -= increment_y;
        x = -1.0f;
    }
}

void LidarSensor::shootRays(std::vector<std::unique_ptr<Primitive>>& prim_list, std::vector<glm::vec3>& vertex_data)
{
    vertex_data.push_back(pos);
    vertex_data.push_back(glm::vec3(0.0, 0.0, 1.0));
    for(int i = 0; i < rays.size(); i++)
    {
        bool flag = false;
        //Debug visualize rays
       // vertex_data.push_back(glm::vec3(rays[i].origin));
        //vertex_data.push_back(glm::vec3(rays[i].origin + rays[i].dir * 14.1f));
        //std::cout << vertex_data.back().x << ", " << vertex_data.back().y << ", " << vertex_data.back().z << std::endl;
        HitInfo hit;
        for(int x = 0; x < prim_list.size(); x++)
        {
            if(prim_list[x]->intersectRay(rays[i], hit))
            {
                flag = true;
            }
        }
        if(flag)
        {
            vertex_data.push_back(glm::vec3(hit.hitpoint));
            vertex_data.push_back(hit.color);
        }

    }
}
