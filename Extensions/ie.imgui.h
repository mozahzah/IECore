// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "Source/IEUtils.h"

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
        static constexpr float DefaultWindowRounding = 8.0f;
        static constexpr float ChildWindowRounding = 8.0f;

        static constexpr float DefaultTextSize = 24.0f;
        static constexpr float SubtitleTextSize = 30.0f;
        static constexpr float TitleTextSize = 36.0f;
        
        namespace Colors
        {
            /* Window */
            static constexpr ImVec4 DefaultWindowBgColor = ImVec4(0.064f, 0.065f, 0.074f, 1.00f);
            static constexpr ImVec4 SideBarBgColor = ImVec4(0.025f, 0.026f, 0.037f, 1.00f);
            
            /* Text */
            static constexpr ImVec4 DefaultTextColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            static constexpr ImVec4 SecondaryTextColor = ImVec4(0.584f, 0.686f, 0.898f, 1.00f);

            /* Button */
            static constexpr ImVec4 DefaultButtonColor = ImVec4(0.584f, 0.686f, 0.898f, 0.2f);
            static constexpr ImVec4 DefaultButtonHoveredColor = ImVec4(0.584f, 0.686f, 0.898f, 0.4f);
            static constexpr ImVec4 RedButtonColor = ImVec4(0.25f, 0.051f, 0.051f, 1.00f);
            static constexpr ImVec4 RedButtonHoveredColor = ImVec4(0.435f, 0.039f, 0.039f, 1.00f);
            static constexpr ImVec4 GreenButtonColor = ImVec4(0.054f, 0.157f, 0.054f, 1.00f);
            static constexpr ImVec4 GreenButtonHoveredColor = ImVec4(0.051f, 0.251f, 0.051f, 1.00f);

            /* Other */
            static constexpr ImVec4 FrameBgColor = ImVec4(0.025f, 0.026f, 0.037f, 1.00f);
            static constexpr ImVec4 ScrollBarColor = ImVec4(0.400f, 0.310f, 0.310f, 1.00f);
            static constexpr ImVec4 ScrollBarActiveColor = ImVec4(0.500f, 0.310f, 0.310f, 1.000f);
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