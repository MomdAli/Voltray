#pragma once

#include <string>

namespace Voltray::Engine
{
    struct EngineSettings
    {
        // Camera controls
        static float CameraOrbitSpeed;
        static float CameraPanSpeed;
        static float CameraZoomSpeed;
        static float CameraMinDistance;
        static float CameraMaxDistance;
        static float MouseClampDelta;

        static float ClearColor[4]; // RGBA

        // Renderer, input, audio... (later)

        static void Load(const std::string &filename);
        static void Save(const std::string &filename);
    };
}
