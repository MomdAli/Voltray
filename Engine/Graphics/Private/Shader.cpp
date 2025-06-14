#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <glad/gl.h>

#include "Shader.h"

namespace Voltray::Engine
{

    Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
    {
        std::string vertexSource = LoadShaderSource(vertexPath);
        if (vertexSource.empty())
        {
            throw std::runtime_error("Failed to load vertex shader: " + vertexPath);
        }

        std::string fragmentSource = LoadShaderSource(fragmentPath);
        if (fragmentSource.empty())
        {
            throw std::runtime_error("Failed to load fragment shader: " + fragmentPath);
        }

        m_RendererID = CreateShaderProgram(vertexSource, fragmentSource);
        if (m_RendererID == 0)
        {
            throw std::runtime_error("Failed to create shader program from: " + vertexPath + " and " + fragmentPath);
        }
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
        if (source.empty())
        {
            std::cerr << "Shader source is empty" << std::endl;
            return 0;
        }

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
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    unsigned int Shader::CreateShaderProgram(const std::string &vertexSource, const std::string &fragmentSource)
    {
        unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
        if (vertex == 0)
        {
            std::cerr << "Failed to compile vertex shader" << std::endl;
            return 0;
        }

        unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (fragment == 0)
        {
            std::cerr << "Failed to compile fragment shader" << std::endl;
            glDeleteShader(vertex);
            return 0;
        }

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
            glDeleteProgram(program);
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return 0;
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

    void Shader::SetUniform3f(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }

    void Shader::SetUniform1f(const std::string &name, float value) const
    {
        glUniform1f(GetUniformLocation(name), value);
    }

} // namespace Voltray::Engine