#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/gl.h>

#include "Shader.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexSource = LoadShaderSource(vertexPath);
    std::string fragmentSource = LoadShaderSource(fragmentPath);
    m_RendererID = CreateShaderProgram(vertexSource, fragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

std::string Shader::LoadShaderSource(const std::string &filepath)
{
    std::ifstream stream(filepath);
    if (!stream.is_open())
    {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(id, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Shader compilation error:\n"
                  << infoLog << std::endl;
    }

    return id;
}

unsigned int Shader::CreateShaderProgram(const std::string &vertexSource, const std::string &fragmentSource)
{
    unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Shader linking error:\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

int Shader::GetUniformLocation(const std::string &name) const
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
    {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist or wasn't used!\n";
    }

    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::SetUniformMat4(const std::string &name, const float *matrix) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix);
}