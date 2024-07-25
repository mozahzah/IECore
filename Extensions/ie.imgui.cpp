// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Immortal Echoes. All rights reserved.
// Author: mozahzah

#include "ie.imgui.h"

#include "IECore.h"
#include "IEUtils.h"

static std::optional<uint8_t> DefaultFontIndex;
static std::optional<uint8_t> BoldFontIndex;
static std::optional<uint8_t> SubtitleFontIndex;
static std::optional<uint8_t> TitleFontIndex;

namespace ImGui
{
    void SetSmartCursorPosX(float X)
    {
        if (X >= ImGui::GetCursorPosX())
        {
            ImGui::SetCursorPosX(X);
        }
    }

    void SetSmartCursorPosY(float Y)
    {
        if (Y >= ImGui::GetCursorPosY())
        {
            ImGui::SetCursorPosY(Y);
        }
    }

    void SetSmartCursorPos(const ImVec2& Pos)
    {
        SetSmartCursorPosX(Pos.x);
        SetSmartCursorPosY(Pos.y);
    }

    void SetSmartCursorPosXRelative(float XMultiplier)
    {
        if (std::fabs(XMultiplier - 0.0f) >= std::numeric_limits<float>::epsilon())
        {
            const float PositionX = (ImGui::GetWindowSize().x + ImGui::GetStyle().WindowPadding.x) * std::clamp(XMultiplier, 0.0f, 1.0f);
            SetSmartCursorPosX(PositionX);
        }
    }

    void SetSmartCursorPosYRelative(float YMultiplier)
    {
        if (std::fabs(YMultiplier - 0.0f) >= std::numeric_limits<float>::epsilon())
        {
            const float PositionY = (ImGui::GetWindowSize().y + ImGui::GetStyle().WindowPadding.y) * std::clamp(YMultiplier, 0.0f, 1.0f);
            SetSmartCursorPosY(PositionY);
        }
    }

    void SetSmartCursorPosRelative(const ImVec2& XYMultiplier)
    {
        SetSmartCursorPosXRelative(XYMultiplier.x);
        SetSmartCursorPosYRelative(XYMultiplier.y);
    }

    void WindowPositionedTextV(float XMultiplier, float YMultiplier, const char* Fmt, va_list Args)
    {
        char Buffer[256];
        vsnprintf(Buffer, sizeof(Buffer), Fmt, Args);

        const ImVec2 TextSize = ImGui::CalcTextSize(Buffer);
        float TextPositionX = (ImGui::GetWindowSize().x + ImGui::GetStyle().WindowPadding.x * 2.0f - TextSize.x - ImGui::GetFontSize()) * std::clamp(XMultiplier, 0.0f, 1.0f);
        float TextPositionY = (ImGui::GetWindowSize().y + ImGui::GetStyle().WindowPadding.y * 2.0f) * std::clamp(YMultiplier, 0.0f, 1.0f);

        TextPositionX = std::fabs(TextPositionX) <= std::numeric_limits<float>::epsilon() ? ImGui::GetCursorPosX() : TextPositionX;
        TextPositionY = std::fabs(TextPositionY) <= std::numeric_limits<float>::epsilon() ? ImGui::GetCursorPosY() : TextPositionY;

        ImGui::SetSmartCursorPos(ImVec2(TextPositionX, TextPositionY));
        ImGui::Text("%s", Buffer);
    }
    
    void WindowPositionedText(float XMultiplier, float YMultiplier, const char* Fmt, ...)
    {
        va_list Args;
        va_start(Args, Fmt);
        WindowPositionedTextV(XMultiplier, YMultiplier, Fmt, Args);
        va_end(Args);
    }

    void CenteredText(const char* Fmt, ...)
    {
        va_list Args;
        va_start(Args, Fmt);
        WindowPositionedTextV(0.5f, 0.0f, Fmt, Args);
        va_end(Args);
    }

    

    void FileFinder(const char* Label, int Depth, std::string& SelectedFile)
    {
        static bool bFileFinderOpen = false;
        static std::filesystem::path RootFileFinderSearchPath = std::filesystem::current_path();

        auto DrawFileFinderTree = [](const std::filesystem::path& CurrentPath, int Depth, std::string& SelectedFile, auto&& DrawFileFinderTree)
            {
                if (!CurrentPath.empty())
                {
                    if (Depth <= 0)
                    {
                        RootFileFinderSearchPath = CurrentPath;
                        ImGui::ClearCurrentWindowStoredStates();
                        return;
                    }

                    for (const std::filesystem::directory_entry& Entry : std::filesystem::directory_iterator(CurrentPath))
                    {
                        const std::filesystem::path& SubPath = Entry.path();
                        if (!IEUtils::IsFileHidden(SubPath))
                        {
                            if (std::filesystem::is_directory(SubPath))
                            {
                                if (ImGui::TreeNodeEx(SubPath.filename().string().c_str()))
                                {
                                    DrawFileFinderTree(SubPath, Depth - 1, SelectedFile, DrawFileFinderTree);
                                    ImGui::TreePop();
                                }
                            }
                            else
                            {
                                ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.0f, 0.5f));
                                if (ImGui::Selectable(SubPath.filename().string().c_str()))
                                {
                                    SelectedFile = SubPath.string();
                                }
                                ImGui::PopStyleVar();
                            }
                        }
                    }
                }
            };

        const char* FileFinderPopupLabel = Label ? Label : "File Finder";
        if (ImGui::Selectable("...##Outer", &bFileFinderOpen))
        {
            ImGui::OpenPopup(FileFinderPopupLabel);
        }

        static constexpr uint32_t WindowFlags = ImGuiWindowFlags_NoCollapse;

        ImGuiIO& IO = ImGui::GetIO();
        static const float WindowWidth = IO.DisplaySize.x * 0.5f;
        static float WindowHeight = IO.DisplaySize.y * 0.7f;

        static float WindowPosX = (IO.DisplaySize.x - WindowWidth) * 0.5f;
        static float WindowPosY = (IO.DisplaySize.y - WindowHeight) * 0.5f;

        ImGui::SetNextWindowSize(ImVec2(WindowWidth, WindowHeight), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(WindowPosX, WindowPosY), ImGuiCond_Once);

        if (ImGui::BeginPopupModal(FileFinderPopupLabel, &bFileFinderOpen, WindowFlags))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.0f, 0.5f));
            if (ImGui::Selectable("...", false, ImGuiSelectableFlags_DontClosePopups, ImVec2(ImGui::CalcTextSize("..."))))
            {
                RootFileFinderSearchPath = RootFileFinderSearchPath.parent_path();
            }
            ImGui::SameLine();
            ImGui::Text("%s", RootFileFinderSearchPath.string().c_str());
            ImGui::PopStyleVar();

            DrawFileFinderTree(RootFileFinderSearchPath, Depth, SelectedFile, DrawFileFinderTree);

            ImGui::EndPopup();
        }
    }

    void ClearCurrentWindowStoredStates()
    {
        if (const ImGuiContext* const CurrentContext = ImGui::GetCurrentContext())
        {
            if (const ImGuiWindow* const CurrentWindow = CurrentContext->CurrentWindow)
            {
                const ImGuiWindowTempData CurrentWindowTempData = CurrentWindow->DC;
                if (ImGuiStorage* const CurrentWindowStateStorage = CurrentWindowTempData.StateStorage)
                {
                    CurrentWindowStateStorage->Clear();
                }
            }
        }
    }

    namespace IEStyle
    {
        ImFont* GetBoldFont()
        {
            ImGuiIO& IO = ImGui::GetIO();
            if (BoldFontIndex.has_value() && IO.Fonts->Fonts.size() >= BoldFontIndex)
            {
                return IO.Fonts->Fonts[BoldFontIndex.value()];
            }
            return nullptr;
        }

        ImFont* GetSubtitleFont()
        {
            ImGuiIO& IO = ImGui::GetIO();
            if (SubtitleFontIndex.has_value() && IO.Fonts->Fonts.size() >= SubtitleFontIndex)
            {
                return IO.Fonts->Fonts[SubtitleFontIndex.value()];
            }
            return nullptr;
        }

        ImFont* GetTitleFont()
        {
            ImGuiIO& IO = ImGui::GetIO();
            if (TitleFontIndex.has_value() && IO.Fonts->Fonts.size() >= TitleFontIndex)
            {
                return IO.Fonts->Fonts[TitleFontIndex.value()];
            }
            return nullptr;
        }

        ImVec2 GetDefaultButtonSize()
        {
            return ImVec2(75.0f, ImGui::GetTextLineHeightWithSpacing());
        }

        ImVec2 GetSquareButtonSize()
        {
            return ImVec2(ImGui::GetTextLineHeightWithSpacing() - 10.0f, ImGui::GetTextLineHeightWithSpacing() - 10.0f);
        }
        
        bool DefaultButton(const char* Label)
        {
            return ImGui::Button(Label, GetDefaultButtonSize());
        }

        bool SquareButton(const char* Label)
        {
            return ImGui::Button(Label, GetSquareButtonSize());
        }

        bool RedButton(const char* Label)
        {
            bool bToReturn = false;
            ImGui::PushStyleColor(ImGuiCol_Button, IEStyle::Colors::RedButtonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IEStyle::Colors::RedButtonHoveredColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, IEStyle::Colors::RedButtonHoveredColor);
            bToReturn = ImGui::Button(Label);
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            return bToReturn;
        }

        bool GreenButton(const char* Label)
        {
            bool bToReturn = false;
            ImGui::PushStyleColor(ImGuiCol_Button, IEStyle::Colors::GreenButtonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IEStyle::Colors::GreenButtonHoveredColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, IEStyle::Colors::GreenButtonHoveredColor);
            bToReturn = ImGui::Button(Label);
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            return bToReturn;
        }

        void StyleIE(ImGuiStyle* StyleDestination)
        {
            ImGuiIO& IO = ImGui::GetIO();
        
            const std::filesystem::path ResourcesDirectory = IEUtils::FindFolderPathUpwards(std::filesystem::current_path(), "Resources");
            IO.IniFilename = nullptr;

            const std::filesystem::path SpaceGroteskFontPath = ResourcesDirectory / "Fonts/Space_Grotesk/static/SpaceGrotesk-Medium.ttf";
            const std::filesystem::path SpaceGroteskSemiBoldFontPath = ResourcesDirectory / "Fonts/Space_Grotesk/static/SpaceGrotesk-SemiBold.ttf";
            const std::filesystem::path SpaceGroteskBoldFontPath = ResourcesDirectory / "Fonts/Space_Grotesk/static/SpaceGrotesk-Bold.ttf";

            if (std::filesystem::exists(SpaceGroteskFontPath) && !DefaultFontIndex.has_value())
            {
                IO.Fonts->AddFontFromFileTTF(IEUtils::StringCast<char>(SpaceGroteskFontPath.c_str()).c_str(), DefaultTextSize);
                DefaultFontIndex = IO.Fonts->Fonts.size() - 1;
            }

            if (std::filesystem::exists(SpaceGroteskSemiBoldFontPath) && !BoldFontIndex.has_value())
            {
                IO.Fonts->AddFontFromFileTTF(IEUtils::StringCast<char>(SpaceGroteskSemiBoldFontPath.c_str()).c_str(), DefaultTextSize);
                BoldFontIndex = IO.Fonts->Fonts.size() - 1;
            }

            if (std::filesystem::exists(SpaceGroteskSemiBoldFontPath) && !SubtitleFontIndex.has_value())
            {
                IO.Fonts->AddFontFromFileTTF(IEUtils::StringCast<char>(SpaceGroteskSemiBoldFontPath.c_str()).c_str(), SubtitleTextSize);
                SubtitleFontIndex = IO.Fonts->Fonts.size() - 1;
            }

            if (std::filesystem::exists(SpaceGroteskBoldFontPath) && !TitleFontIndex.has_value())
            {
                IO.Fonts->AddFontFromFileTTF(IEUtils::StringCast<char>(SpaceGroteskBoldFontPath.c_str()).c_str(), TitleTextSize);
                TitleFontIndex = IO.Fonts->Fonts.size() - 1;
            }

            IO.Fonts->Build();
            IO.FontGlobalScale = 1.0f;

            if (ImGuiStyle* const Style = StyleDestination ? StyleDestination : &ImGui::GetStyle())
            {
                /* Main */
                Style->WindowPadding = ImVec2(10.0f, 10.0f);
                Style->FramePadding = ImVec2(5.0f, 5.0f);
                Style->ItemSpacing = ImVec2(10.0f, 5.0f);
                Style->ItemInnerSpacing = ImVec2(10.0f, 5.0f);
                Style->IndentSpacing = 15.0f;
                Style->ScrollbarSize = 15.0f;
                Style->GrabMinSize = 10.0f;

                /* Borders */
                Style->WindowBorderSize = 0.0f;
                Style->ChildBorderSize = 0.0f;
                Style->PopupBorderSize = 1.0f;
                Style->FrameBorderSize = 0.0f;
                Style->TabBorderSize = 0.0f;
                Style->TabBarBorderSize = 0.0f;

                /* Rounding */
                Style->WindowRounding = DefaultWindowRounding;
                Style->ChildRounding = DefaultWindowRounding;
                Style->FrameRounding = 1.0f;
                Style->PopupRounding = 1.0f;
                Style->ScrollbarRounding = 12.0f;
                Style->GrabRounding = 12.0f;
                Style->TabRounding = 2.0f;

                /* Tables */
                Style->CellPadding = ImVec2(7.0f, 7.0f);
                Style->TableAngledHeadersAngle = 35.0f;
                Style->TableAngledHeadersTextAlign = ImVec2(0.5f, 0.0f);

                /* Widgets */
                Style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
                Style->WindowMenuButtonPosition = ImGuiDir_Left;
                Style->ColorButtonPosition = ImGuiDir_Left;
                Style->ButtonTextAlign = ImVec2(0.5f, 0.5f);
                Style->SelectableTextAlign = ImVec2(0.5f, 0.5f);
                Style->SeparatorTextBorderSize = 1.0f;
                Style->SeparatorTextAlign = ImVec2(0.5f, 0.5f);
                Style->SeparatorTextPadding = ImVec2(20.0f, 4.0f);

                if (ImVec4* const Colors = Style->Colors)
                {
                    Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

                    Colors[ImGuiCol_WindowBg] = IEStyle::Colors::DefaultWindowBgColor;
                    Colors[ImGuiCol_ChildBg] = IEStyle::Colors::DefaultWindowBgColor;
                    Colors[ImGuiCol_PopupBg] = IEStyle::Colors::DefaultWindowBgColor;

                    Colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

                    Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
                    Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
                    Colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

                    Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

                    Colors[ImGuiCol_MenuBarBg] = ImVec4(0.065f, 0.065f, 0.065f, 1.000f);

                    Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
                    Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.400f, 0.310f, 0.310f, 1.000f);
                    Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.500f, 0.310f, 0.310f, 1.000f);
                    Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.500f, 0.310f, 0.310f, 1.000f);

                    Colors[ImGuiCol_CheckMark] = ImVec4(0.816f, 0.821f, 1.000f, 1.000f);
                    Colors[ImGuiCol_SliderGrab] = ImVec4(0.816f, 0.821f, 1.000f, 1.000f);
                    Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.700f, 0.821f, 1.000f, 1.000f);

                    Colors[ImGuiCol_Button] = IEStyle::Colors::DefaultButtonColor;
                    Colors[ImGuiCol_ButtonHovered] = IEStyle::Colors::DefaultButtonHoveredColor;
                    Colors[ImGuiCol_ButtonActive] = IEStyle::Colors::DefaultButtonHoveredColor;

                    Colors[ImGuiCol_Header] = ImVec4(0.000f, 0.000f, 0.000f, 0.355f);
                    Colors[ImGuiCol_HeaderHovered] = ImVec4(0.000f, 0.000f, 0.000f, 0.500f);
                    Colors[ImGuiCol_HeaderActive] = ImVec4(0.000f, 0.000f, 0.000f, 0.500f);

                    Colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

                    Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

                    Colors[ImGuiCol_TabHovered] = ImVec4(0.000f, 0.000f, 0.000f, 0.500f);
                    Colors[ImGuiCol_Tab] = ImVec4(0.000f, 0.000f, 0.000f, 0.355f);
                    Colors[ImGuiCol_TabSelected] = ImVec4(0.000f, 0.000f, 0.000f, 0.500f);
                    Colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.000f, 0.000f, 0.000f, 0.500f);
                    Colors[ImGuiCol_TabDimmed] = ImVec4(0.000f, 0.000f, 0.000f, 0.355f);
                    Colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.000f, 0.000f, 0.000f, 0.500f);
                    Colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.000f, 0.000f, 0.000f, 0.500f);

                    Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                    Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

                    Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
                    Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
                    Colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
                    Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
                    Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

                    Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

                    Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

                    Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
                    Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
                    Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

                    Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
                }
            }
        }
    }
}