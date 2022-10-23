#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec3 color;
uniform mat4 view;
uniform mat4 proj;

void main()
{    
    color = col;
    if(color.x == 0.0 && color.y == 0.0 && color.z == 1.0)
    {
      gl_PointSize = 16.0f;
    }
    else
    {
      gl_PointSize = 2.0f;
    }
    gl_Position = proj * view * vec4(pos, 1.0);
}