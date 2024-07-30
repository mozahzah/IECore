// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IERenderer.h"
#include "IECore.h"
#include "Extensions/ie.imgui.h"

int main()
{
    std::shared_ptr<IERenderer> Renderer = std::make_shared<IERenderer_Vulkan>();
    if (Renderer->Initialize())
    {
        if (ImGuiContext* const CreatedImGuiContext = ImGui::CreateContext())
        {
            ImGuiIO& IO = ImGui::GetIO();
            IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_IsSRGB;
            if (Renderer->PostImGuiContextCreated())
            {
                ImGui::IEStyle::StyleIE();
                IO.IniFilename = nullptr;
                IO.LogFilename = nullptr;

                IEClock::time_point StartFrameTime = IEClock::now();
                IEDurationMs CapturedDeltaTime = IEDurationMs::zero();

                while (Renderer->IsAppRunning())
                {
                    StartFrameTime = IEClock::now();

                    Renderer->CheckAndResizeSwapChain();
                    Renderer->NewFrame();
                    ImGui::NewFrame();

                    /*
                    * App Code Goes Here
                    */
                    ImGui::ShowDemoWindow();
                    Renderer->DrawTelemetry();
                    
                    ImGui::Render();
                    Renderer->RenderFrame(*ImGui::GetDrawData());
                    Renderer->PresentFrame();

                    /*
                    * Post-Frame App Code Goes Here
                    */

                    CapturedDeltaTime = std::chrono::duration_cast<IEDurationMs>(IEClock::now() - StartFrameTime);
                    if (Renderer->IsAppWindowOpen())
                    {
                        Renderer->WaitEventsTimeout(0.1f);
                    }
                    else
                    {
                        Renderer->WaitEvents();
                    }
                }
            }
        }

        Renderer->Deinitialize();
    }

    return 0;
}