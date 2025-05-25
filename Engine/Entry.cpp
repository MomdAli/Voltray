#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>

#include "Graphics/Shader.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Mesh.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Input/Input.h"
#include "Config/EngineSettings.h"

namespace fs = std::filesystem;

std::string ResolveShaderPath(const std::string &relativePath)
{
    fs::path exePath = fs::current_path();
    fs::path rootPath = exePath;

    while (!fs::exists(rootPath / "Shaders") && rootPath.has_parent_path())
    {
        rootPath = rootPath.parent_path();
    }

    return (rootPath / "Shaders" / relativePath).string();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

int main()
{
    // === Init GLFW ===
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Voltray", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // === Vertex and Index Data ===
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // 0
        0.5f, -0.5f, 0.0f,  // 1
        0.0f, 0.75f, 0.0f,  // 2
        -0.25f, 0.0f, 0.0f, // 3
        0.0f, -0.5f, 0.0f,  // 4
        0.25f, 0.0f, 0.0f   // 5
    };

    unsigned int indices[] = {
        0, 4, 3, // Left triangle
        4, 1, 5, // Right triangle
        3, 5, 2  // Top triangle
    };

    // === Create Mesh, Shader, and Renderer ===
    Mesh triangle(vertices, sizeof(vertices), indices, sizeof(indices) / sizeof(unsigned int));
    Shader shader(ResolveShaderPath("simple.vert"), ResolveShaderPath("simple.frag"));
    Shader skyboxShader(ResolveShaderPath("skybox.vert"), ResolveShaderPath("skybox.frag"));
    Renderer renderer;

    Camera camera(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    camera.SetPosition(Vec3(0.0f, 0.0f, EngineSettings::CameraMaxDistance / 10.0f));
    camera.SetTarget(Vec3(0.0f, 0.0f, 0.0f));

    // === Initialize Input ===
    Input::Init(window);
    // double lastFrame = 0.0;

    // === OpenGL Settings ===
    glEnable(GL_DEPTH_TEST); // Enable depth testing for correct 3D rendering
    glDisable(GL_CULL_FACE);

    // === Main Loop ===
    while (!glfwWindowShouldClose(window))
    {
        // Update camera
        // double currentFrame = glfwGetTime();
        // double deltaTime = currentFrame - lastFrame;
        // lastFrame = currentFrame;

        Input::Update();
        camera.Update();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               // Optionally move to EngineSettings if you want
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers

        // === Draw skybox gradient ===
        glDepthMask(GL_FALSE); // Disable depth writes
        skyboxShader.Bind();
        // Pass inverse view-projection matrix
        Mat4 invViewProj = (camera.GetViewProjectionMatrix()).Inverse();
        skyboxShader.SetUniformMat4("u_InverseViewProj", invViewProj.data);
        glDrawArrays(GL_TRIANGLES, 0, 3); // Fullscreen triangle
        glDepthMask(GL_TRUE);             // Re-enable depth writes

        // === Clear depth buffer only ===
        glClear(GL_DEPTH_BUFFER_BIT);

        // Draw the Triangles
        Mat4 viewProj = camera.GetViewProjectionMatrix();
        shader.Bind();
        shader.SetUniformMat4("u_ViewProjection", viewProj.data);
        renderer.Draw(triangle, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
