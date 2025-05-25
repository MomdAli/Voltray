#include "Input/Input.h"
#include <GLFW/glfw3.h>

GLFWwindow *Input::s_Window = nullptr;

double Input::s_LastMouseX = 0.0;
double Input::s_LastMouseY = 0.0;
double Input::s_MouseDeltaX = 0.0;
double Input::s_MouseDeltaY = 0.0;
double Input::s_ScrollDeltaY = 0.0;
bool Input::s_FirstMouse = true;

void Input::ScrollCallback(GLFWwindow * /*window*/, double /*xoffset*/, double yoffset)
{
    // Make sure we only process scroll events while the window is valid
    if (!s_Window)
        return;

    s_ScrollDeltaY += yoffset;
}

void Input::Init(GLFWwindow *window)
{
    s_Window = window;
    glfwSetScrollCallback(window, ScrollCallback);
}

void Input::Shutdown()
{
    // Unregister callbacks if needed
    if (s_Window)
    {
        // Reset to default callback or nullptr if needed
        glfwSetScrollCallback(s_Window, nullptr);
    }

    // Clear member variables
    s_Window = nullptr;
    s_MouseDeltaX = 0.0;
    s_MouseDeltaY = 0.0;
    s_ScrollDeltaY = 0.0;
    s_FirstMouse = true;
}

bool Input::IsKeyPressed(int key)
{
    if (!s_Window)
        return false;
    return glfwGetKey(s_Window, key) == GLFW_PRESS;
}

bool Input::IsKeyReleased(int key)
{
    if (!s_Window)
        return true;
    return glfwGetKey(s_Window, key) == GLFW_RELEASE;
}

bool Input::IsMouseButtonPressed(int button)
{
    if (!s_Window)
        return false;
    return glfwGetMouseButton(s_Window, button) == GLFW_PRESS;
}

void Input::Update()
{
    if (!s_Window)
        return;

    double x, y;
    glfwGetCursorPos(s_Window, &x, &y);

    if (s_FirstMouse)
    {
        s_LastMouseX = x;
        s_LastMouseY = y;
        s_FirstMouse = false;
    }

    s_MouseDeltaX = x - s_LastMouseX;
    s_MouseDeltaY = s_LastMouseY - y; // Y is inverted

    s_LastMouseX = x;
    s_LastMouseY = y;
}

double Input::GetMouseDeltaX()
{
    if (!s_Window)
        return 0.0;
    return s_MouseDeltaX;
}

double Input::GetMouseDeltaY()
{
    if (!s_Window)
        return 0.0;
    return s_MouseDeltaY;
}

double Input::GetScrollDeltaY()
{
    if (!s_Window)
        return 0.0;
    double delta = s_ScrollDeltaY;
    s_ScrollDeltaY = 0.0;
    return delta;
}

void Input::ResetMouseDelta()
{
    // Add null check to prevent crashes if called after window is destroyed
    if (!s_Window)
        return;

    s_MouseDeltaX = 0.0;
    s_MouseDeltaY = 0.0;
}
