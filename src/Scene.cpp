#include "Scene.h"
#include "glm/gtc/matrix_access.hpp"
#include "Ray.h"
#include <iomanip>
Scene::Scene()
{
    //ctor
}

Scene::~Scene()
{
    //dtor
}

void Scene::createShaderProgram()
{
    vs_text = loadShaderSource("./pc-vs.vs");
    fs_text = loadShaderSource("./pc-fs.fs");

    vs = compileShader(vs_text, GL_VERTEX_SHADER);
    fs = compileShader(fs_text, GL_FRAGMENT_SHADER);

    //Link program object
    shader_program = createProgram(vs, fs);
    glUseProgram(shader_program);

}


void Scene::render()
{
    glUseProgram(shader_program);
    if(main_cam.is_changed)
        main_cam.updateViewMatUniform(shader_program, "view");

    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, pc_vertex_data.size()/2);
    //glDrawArrays(GL_LINES, 0, pc_vertex_data.size());
    glBindVertexArray(0);


}

void Scene::createScene()
{
    //Setup Camera
    glm::vec4 eye = glm::vec4(0.0, 0.0, 11.0, 1.0);
    glm::vec4 side = glm::vec4(1.0, 0.0, 0.0, 0.0);
    glm::vec4 lookat_pos = glm::vec4(0.0, 0.0, -5.0, 1.0);
    glm::vec4 up = glm::vec4(0.0, 1.0, 0.0, 0.0);

    main_cam.setProjectionMatrix(60.0f, 1280.0f, 720.0f, 0.1f, 1000.0f);
    main_cam.setViewMatrix(eye, side, up, lookat_pos);
    main_cam.updateProjMatUniform(shader_program, "proj");
    main_cam.updateViewMatUniform(shader_program, "view");
    Camera::main_cam = &main_cam;

    //Setup Scene
    //Spheres
    primitive_list.clear();
    primitive_list.push_back(std::make_unique<Sphere>(glm::vec4(1.f, -2.5f, -5.f, 1.f), 1.5f, glm::vec3(0.711f, 0.501f, 0.3215f)));
    primitive_list.push_back(std::unique_ptr<Primitive>(new Sphere({-5.f, -3.f, -8.f, 1.f}, 1.0f, {0.4f, 0.3f, 0.9f})));

    //Walls
    // bottom floor
    primitive_list.push_back(std::unique_ptr<Primitive>(new Quad(
        {-6.f, -4.f, -9.f, 1.f},
        {0.f, 1.f, 0.f, 0.f},
        {12.f, 0.f, 0.f, 0.f},
        {0.f, 0.f, 10.f, 0.f},
        {0.9f, 0.9f, 0.9f}
    )));

     // top wall
    primitive_list.push_back(std::unique_ptr<Primitive>( new Quad(
        {-6.f, 5.f, -9.f, 1.f},
        {0.f, -1.f, 0.f, 0.f},
        {12.f, 0.f, 0.f, 0.f},
        {0.f, 0.f, 10.f, 0.f},
        {0.9f, 0.9f, 0.9f}
    )));

   //right wall
    primitive_list.push_back(std::unique_ptr<Primitive>( new Quad(
        {6.f, 5.f, -9.f, 0.f},
        {-1.f, 0.f, 0.f, 0.f},
        {0.f, -9.f, 0.f, 0.f},
        {0.f, 0.f, 10.f, 0.f},
        {0.8f, 0.f, 0.f}
    )));

    //left wall
    primitive_list.push_back(std::unique_ptr<Primitive>( new Quad(
        {-6.f, 5.f, -9.f, 1.f},
        {1.f, 0.f, 0.f, 0.f},
        {0.f, -9.f, 0.f, 0.f},
        {0.f, 0.f, 10.f, 0.f},
        {0.f, 0.8f, 0.f}
    )));

    //back wall
    primitive_list.push_back(std::unique_ptr<Primitive>( new Quad(
        {-6.f, 5.f, -9.f, 0.f},
        {0.f, 0.f, 1.f, 0.f},
        {0.f, -9.f, 0.f, 0.f},
        {12.f, 0.f, 0.f, 0.f},
        {0.9f, 0.9f, 0.9f}
    )));

    //Setup Sensor
    lidar.init(glm::vec4(0.0, -1.0, 2.0, 1.0), side, up, glm::vec4(0.0, 0.0, -1.0, 0.0), 170.0f, 150.0f, 1400, 500);
    lidar.shootRays(primitive_list, pc_vertex_data);



    //Pass Data to GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * pc_vertex_data.size(), &pc_vertex_data[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Position Data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
		0, // Attribute location
		3, // Number of elements per attribute
		GL_FLOAT, // Type of elements
		GL_FALSE, // Normalized? if gl.byte or gl.short normalized to [-1, 1] range
		6 * sizeof(float), // Size of an individual vertex (stride)
		0 // Offset from the beginning of a single vertex to this attribute
	);

    //Color Data
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
		1, // Attribute location
		3, // Number of elements per attribute
		GL_FLOAT, // Type of elements
		GL_FALSE, // Normalized? if gl.byte or gl.short normalized to [-1, 1] range
		6 * sizeof(float), // Size of an individual vertex (stride)
		(void*)(3 * sizeof(float)) // Offset from the beginning of a single vertex to this attribute
	);

    glUseProgram(shader_program);
    glBindVertexArray(0);
}

