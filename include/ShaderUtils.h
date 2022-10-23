#ifndef SHADERUTILS_H
#define SHADERUTILS_H


#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "glad/glad.h"


inline std::string loadShaderSource(std::string fn)
{
    std::string shader_data = "";
    std::streamoff len;
    std::ifstream file;

    file.open(fn, std::ios::binary);
    if(!file.is_open())
    {
        return "";
    }

    file.seekg(0, std::ios::end);
    len = file.tellg();
    file.seekg(0, std::ios::beg);

    shader_data.resize(len);
    file.read(&shader_data[0], len);

    return shader_data;
}

inline GLuint compileShader(std::string source, GLenum shader_type)
{
    // Create an empty vertex shader handle
    GLuint shader = glCreateShader(shader_type);

    // Send the shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar *src = (const GLchar *)source.c_str();
    glShaderSource(shader, 1, &src, 0);

    // Compile the vertex shader
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(shader);
        for (char i: infoLog)
            std::cout << i;
        std::cout << std::endl;
        return 0;
    }
    return shader;
}

inline GLuint createProgram(GLuint vertexShader, GLuint fragmentShader)
{
    // Now time to link them together into a program.
    // Get a program object.
    GLuint program = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

         for (char i: infoLog)
            std::cout << i;
        std::cout << std::endl;
        return 0;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glUseProgram(program);
    return program;
}


#endif // SHADERUTILS_H
