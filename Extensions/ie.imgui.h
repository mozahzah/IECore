// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Immortal Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "IECore.h"

namespace ImGui
{
    /* Formating */
    void SetSmartCursorPosX(float X);
    void SetSmartCursorPosY(float Y);
    void SetSmartCursorPos(const ImVec2& Pos);
    void SetSmartCursorPosXRelative(float XMultiplier);
    void SetSmartCursorPosYRelative(float YMultiplier);
    void SetSmartCursorPosRelative(const ImVec2& XYMultiplier);

    /* Text */
    void WindowPositionedText(float XMultiplier, float YMultiplier, const char* Fmt, ...);
    void CenteredText(const char* Fmt, ...);

    /* Widgets */
    void FileFinder(const char* Label, int Depth, std::string& SelectedFile);
    
    /* Other */
    void ClearCurrentWindowStoredStates();

    namespace IEStyle
    {
        static constexpr float DefaultWindowRounding = 0.0f;
        static constexpr float ChildWindowRounding = 5.0f;

        static constexpr float DefaultTextSize = 18.0f;
        static constexpr float SubtitleTextSize = 24.0f;
        static constexpr float TitleTextSize = 32.0f;
        
        namespace Colors
        {
            /* Window */
            static constexpr ImVec4 DefaultWindowBgColor = ImVec4(0.061f, 0.061f, 0.061f, 1.00f);
            static constexpr ImVec4 SideBarBgColor = ImVec4(0.0305f, 0.0305f, 0.0305f, 1.00f);
            
            /* Text */
            static constexpr ImVec4 DefaultTextColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            static constexpr ImVec4 SecondaryTextColor = ImVec4(0.584f,0.686f,0.898f, 1.00f);

            /* Button */
            static constexpr ImVec4 DefaultButtonColor = ImVec4(0.4f, 0.515f, 0.750f, 0.20f);
            static constexpr ImVec4 DefaultButtonHoveredColor = ImVec4(0.4f, 0.515f, 0.750f, 0.4f);
            static constexpr ImVec4 RedButtonColor = ImVec4(0.25f, 0.051f, 0.051f, 1.00f);
            static constexpr ImVec4 RedButtonHoveredColor = ImVec4(0.435f, 0.039f, 0.039f, 1.00f);
            static constexpr ImVec4 GreenButtonColor = ImVec4(0.054f, 0.157f, 0.054f, 1.00f);
            static constexpr ImVec4 GreenButtonHoveredColor = ImVec4(0.051f, 0.251f, 0.051f, 1.00f);
        }

        /* Fonts */
        ImFont* GetBoldFont();
        ImFont* GetSubtitleFont();
        ImFont* GetTitleFont();
        
        /* Widgets */
        ImVec2 GetDefaultButtonSize();
        ImVec2 GetSquareButtonSize();
        bool DefaultButton(const char* Label);
        bool SquareButton(const char* Label);
        bool RedButton(const char* Label);
        bool GreenButton(const char* Label);

        void StyleIE(ImGuiStyle* StyleDestination = nullptr);
    }
}