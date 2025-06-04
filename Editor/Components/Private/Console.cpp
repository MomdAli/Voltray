#include "Console.h"
#include <imgui.h>
#include <iomanip>
#include <sstream>

namespace Voltray::Editor::Components
{
    // Static instance initialization
    std::unique_ptr<Console> Console::s_instance = nullptr;

    Console::Console()
        : m_autoScroll(true), m_showTimestamps(true),
          m_showInfo(true), m_showWarning(true), m_showError(true),
          m_showDebug(true), m_showSuccess(true)
    {
        // Initialize the console with a welcome message
        AddMessage("Console initialized", MessageType::Success);
    }

    Console::~Console() = default;
    Console &Console::GetInstance()
    {
        if (!s_instance)
        {
            s_instance = std::make_unique<Console>();
        }
        return *s_instance;
    }

    void Console::Draw()
    {
        ImGui::Begin("Console");

        // Control buttons
        if (ImGui::Button("Clear"))
        {
            Clear();
        }
        ImGui::SameLine();
        ImGui::Checkbox("Auto-scroll", &m_autoScroll);
        ImGui::SameLine();
        ImGui::Checkbox("Timestamps", &m_showTimestamps);

        ImGui::Separator(); // Filter checkboxes
        ImGui::TextWrapped("Filters:");
        ImGui::SameLine();
        ImGui::Checkbox("Info", &m_showInfo);
        ImGui::SameLine();
        ImGui::Checkbox("Warning", &m_showWarning);
        ImGui::SameLine();
        ImGui::Checkbox("Error", &m_showError);
        ImGui::SameLine();
        ImGui::Checkbox("Debug", &m_showDebug);
        ImGui::SameLine();
        ImGui::Checkbox("Success", &m_showSuccess);

        ImGui::Separator();

        // Messages area
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto &message : m_messages)
        {
            // Apply filters
            bool showMessage = false;
            switch (message.type)
            {
            case MessageType::Info:
                showMessage = m_showInfo;
                break;
            case MessageType::Warning:
                showMessage = m_showWarning;
                break;
            case MessageType::Error:
                showMessage = m_showError;
                break;
            case MessageType::Debug:
                showMessage = m_showDebug;
                break;
            case MessageType::Success:
                showMessage = m_showSuccess;
                break;
            }

            if (!showMessage)
                continue;

            // Set color for message type
            ImVec4 color = GetMessageColor(message.type);
            ImGui::PushStyleColor(ImGuiCol_Text, color);

            // Format and display message
            std::string displayText;

            if (m_showTimestamps)
            {
                displayText += "[" + FormatTimestamp(message.timestamp) + "] ";
            }

            displayText += GetMessagePrefix(message.type);
            displayText += message.text;

            ImGui::TextWrapped("%s", displayText.c_str());
            ImGui::PopStyleColor();
        }

        // Auto-scroll to bottom
        if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
        ImGui::End();
    }

    void Console::AddMessage(const std::string &message, MessageType type)
    {
        m_messages.emplace_back(message, type);

        // Limit message history to prevent memory issues
        if (m_messages.size() > 1000)
        {
            m_messages.erase(m_messages.begin(), m_messages.begin() + 100);
        }
    }
    void Console::Clear()
    {
        m_messages.clear();
    }

    ImVec4 Console::GetMessageColor(MessageType type) const
    {
        switch (type)
        {
        case MessageType::Info:
            return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
        case MessageType::Warning:
            return ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
        case MessageType::Error:
            return ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
        case MessageType::Debug:
            return ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // Gray
        case MessageType::Success:
            return ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
        default:
            return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
        }
    }

    const char *Console::GetMessagePrefix(MessageType type) const
    {
        switch (type)
        {
        case MessageType::Info:
            return "[INFO] ";
        case MessageType::Warning:
            return "[WARN] ";
        case MessageType::Error:
            return "[ERROR] ";
        case MessageType::Debug:
            return "[DEBUG] ";
        case MessageType::Success:
            return "[SUCCESS] ";
        default:
            return "[INFO] ";
        }
    }

    std::string Console::FormatTimestamp(const std::chrono::steady_clock::time_point &timestamp) const
    {
        auto duration = timestamp.time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration - seconds);

        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << (seconds.count() % 3600) / 60 << ":"
           << std::setfill('0') << std::setw(2) << seconds.count() % 60 << "."
           << std::setfill('0') << std::setw(3) << milliseconds.count() % 1000;
        return ss.str();
    }

    // Static convenience methods
    void Console::Print(const std::string &message)
    {
        GetInstance().AddMessage(message, MessageType::Info);
    }

    void Console::PrintWarning(const std::string &message)
    {
        GetInstance().AddMessage(message, MessageType::Warning);
    }

    void Console::PrintError(const std::string &message)
    {
        GetInstance().AddMessage(message, MessageType::Error);
    }

    void Console::PrintDebug(const std::string &message)
    {
        GetInstance().AddMessage(message, MessageType::Debug);
    }

    void Console::PrintSuccess(const std::string &message)
    {
        GetInstance().AddMessage(message, MessageType::Success);
    }

    // Global convenience functions
    void DebugPrint(const std::string &message)
    {
        Console::Print(message);
    }

    void DebugPrintWarning(const std::string &message)
    {
        Console::PrintWarning(message);
    }

    void DebugPrintError(const std::string &message)
    {
        Console::PrintError(message);
    }

    void DebugPrintDebug(const std::string &message)
    {
        Console::PrintDebug(message);
    }

    void DebugPrintSuccess(const std::string &message)
    {
        Console::PrintSuccess(message);
    }
}
