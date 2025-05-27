#include <fstream>
#include <iostream>

#include "EngineSettings.h"

// Default values
float EngineSettings::CameraOrbitSpeed = 0.3f;
float EngineSettings::CameraPanSpeed = 0.002f;
float EngineSettings::CameraZoomSpeed = 1.0f;
float EngineSettings::CameraMinDistance = 0.5f;
float EngineSettings::CameraMaxDistance = 100.0f;
float EngineSettings::MouseClampDelta = 22.0f;
float EngineSettings::ClearColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};

void EngineSettings::Load(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    file >> CameraOrbitSpeed;
    file >> CameraPanSpeed;
    file >> CameraZoomSpeed;
    file >> CameraMinDistance;
    file >> CameraMaxDistance;
    file >> MouseClampDelta;
    for (int i = 0; i < 4; ++i)
    {
        file >> ClearColor[i];
    }
    file.close();
}
void EngineSettings::Save(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    file << CameraOrbitSpeed << "\n";
    file << CameraPanSpeed << "\n";
    file << CameraZoomSpeed << "\n";
    file << CameraMinDistance << "\n";
    file << CameraMaxDistance << "\n";
    file << MouseClampDelta << "\n";
    for (int i = 0; i < 4; ++i)
    {
        file << ClearColor[i] << " ";
    }
    file.close();
}