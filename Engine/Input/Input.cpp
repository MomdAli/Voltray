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
    s_ScrollDeltaY += yoffset;
}

void Input::Init(GLFWwindow *window)
{
    s_Window = window;
    glfwSetScrollCallback(window, ScrollCallback);
}

bool Input::IsKeyPressed(int key)
{
    return glfwGetKey(s_Window, key) == GLFW_PRESS;
}

bool Input::IsKeyReleased(int key)
{
    return glfwGetKey(s_Window, key) == GLFW_RELEASE;
}

bool Input::IsMouseButtonPressed(int button)
{
    return glfwGetMouseButton(s_Window, button) == GLFW_PRESS;
}

void Input::Update()
{
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

double Input::GetMouseDeltaX() { return s_MouseDeltaX; }
double Input::GetMouseDeltaY() { return s_MouseDeltaY; }

double Input::GetScrollDeltaY()
{
    double delta = s_ScrollDeltaY;
    s_ScrollDeltaY = 0.0;
    return delta;
}

void Input::ResetMouseDelta()
{
    s_MouseDeltaX = 0.0;
    s_MouseDeltaY = 0.0;
}
