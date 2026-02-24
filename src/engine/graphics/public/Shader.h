#pragma once

#include <string>
#include <unordered_map>

namespace Voltray::Engine
{

    /**
     * @class Shader
     * @brief Encapsulates an OpenGL shader program, handling compilation, linking, and usage.
     *
     * The Shader class provides functionality to load, compile, and link vertex and fragment shaders,
     * as well as to bind and unbind the resulting shader program for rendering.
     */
    class Shader
    {
    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        ~Shader();
        void Bind() const;
        void Unbind() const;

        void SetUniformMat4(const std::string &name, const float *matrix) const;
        void SetUniform3f(const std::string &name, float x, float y, float z) const;
        void SetUniform1f(const std::string &name, float value) const;

    private:
        unsigned int m_RendererID;
        mutable std::unordered_map<std::string, int> m_UniformLocationCache;

        std::string LoadShaderSource(const std::string &filepath);
        unsigned int CompileShader(unsigned int type, const std::string &source);
        unsigned int CreateShaderProgram(const std::string &vertexSource, const std::string &fragmentSource);
        int GetUniformLocation(const std::string &name) const;
    };

} // namespace Voltray::Engine
