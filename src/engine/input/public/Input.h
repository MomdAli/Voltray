#pragma once

struct GLFWwindow;

namespace Voltray::Engine
{
    /**
     * @class Input
     * @brief Handles keyboard input for the application using GLFW.
     *
     * This class provides static methods to initialize input handling,
     * update input states, and query the state of specific keys.
     *
     * Usage:
     * - Call Init() once with the GLFW window pointer to set up input handling.
     * - Call Update() every frame to refresh input states.
     * - Use IsKeyPressed() and IsKeyReleased() to check the state of keys.
     */
    class Input
    {
    public:
        static void Init(GLFWwindow *window);
        static void Shutdown(); // Add proper shutdown method
        static void Update();

        static bool IsKeyPressed(int key);
        static bool IsKeyReleased(int key);
        static bool IsMouseButtonPressed(int button);

        static double GetMouseDeltaX();
        static double GetMouseDeltaY();
        static double GetScrollDeltaY();

        static GLFWwindow *s_Window;
        static void ResetMouseDelta();

        static void HideCursor();
        static void ShowCursor();

    private:
        static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

        static double s_LastMouseX;
        static double s_LastMouseY;
        static double s_MouseDeltaX;
        static double s_MouseDeltaY;
        static double s_ScrollDeltaY;
        static bool s_FirstMouse;
    };
}
