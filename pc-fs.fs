#version 330 core
layout (location = 0) out vec4 frag_color;

in vec3 color;

void main()
{
    //make points round
    
    vec2 transformed_coords = (gl_PointCoord * 2.0) - 1.0;
    if(length(transformed_coords) > 1.0)
    {
      discard;
    }
    frag_color = vec4(color, 1.0);
    
} 