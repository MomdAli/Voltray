#pragma once
#include "Panel.h"
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <iostream>
#include <memory>
#include <imgui.h>

/**
 * @file Console.h
 * @brief Defines the Console panel component for the Voltray editor.
 */

namespace Voltray::Editor::Components
{
    /**
     * @enum MessageType
     * @brief Defines different types of console messages
     */
    enum class MessageType
    {
        Info,
        Warning,
        Error,
        Debug,
        Success
    };

    /**
     * @struct ConsoleMessage
     * @brief Represents a single message in the console
     */
    struct ConsoleMessage
    {
        std::string text;
        MessageType type;
        std::chrono::steady_clock::time_point timestamp;

        ConsoleMessage(const std::string &msg, MessageType msgType)
            : text(msg), type(msgType), timestamp(std::chrono::steady_clock::now()) {}
    };

    /**
     * @class Console
     * @brief A panel component that provides console functionality in the editor.
     * @extends Panel
     */
    class Console : public Panel
    {
    public:
        /**
         * @brief Constructor that sets up the console
         */
        Console();

        /**
         * @brief Destructor that cleans up console resources
         */
        ~Console();

        /**
         * @brief Renders the console panel interface.
         * @override Implements the abstract method from the Panel base class.
         */
        void Draw() override;

        /**
         * @brief Adds a message to the console
         * @param message The message text
         * @param type The type of message
         */
        void AddMessage(const std::string &message, MessageType type = MessageType::Info);

        /**
         * @brief Clears all messages from the console
         */
        void Clear();

        /**
         * @brief Gets the singleton instance of the console
         * @return Reference to the console instance
         */
        static Console &GetInstance();

        // Static convenience functions for easy debug printing
        static void Print(const std::string &message);
        static void PrintWarning(const std::string &message);
        static void PrintError(const std::string &message);
        static void PrintDebug(const std::string &message);
        static void PrintSuccess(const std::string &message);

    private:
        std::vector<ConsoleMessage> m_messages;
        bool m_autoScroll = true;
        bool m_showTimestamps = true;

        // Filter options
        bool m_showInfo = true;
        bool m_showWarning = true;
        bool m_showError = true;
        bool m_showDebug = true;
        bool m_showSuccess = true;

        // Static instance for singleton pattern
        static std::unique_ptr<Console> s_instance;

        /**
         * @brief Gets the color for a message type
         * @param type The message type
         * @return ImVec4 color
         */
        ImVec4 GetMessageColor(MessageType type) const;

        /**
         * @brief Gets the prefix string for a message type
         * @param type The message type
         * @return String prefix
         */
        const char *GetMessagePrefix(MessageType type) const;

        /**
         * @brief Formats timestamp for display
         * @param timestamp The timestamp to format
         * @return Formatted time string
         */
        std::string FormatTimestamp(const std::chrono::steady_clock::time_point &timestamp) const;
    };

    // Global convenience functions (can be used from anywhere)
    void DebugPrint(const std::string &message);
    void DebugPrintWarning(const std::string &message);
    void DebugPrintError(const std::string &message);
    void DebugPrintDebug(const std::string &message);
    void DebugPrintSuccess(const std::string &message);
}
