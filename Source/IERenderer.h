// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#define GLFW_INCLUDE_VULKAN
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#if defined (_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32 1
#endif

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include "IEUtils.h"

class IERenderer
{
public:
    using IEWindowCallbackFunc = std::function<void(uint32_t WindowID)>;

public:
    virtual ~IERenderer() = default;

public:
    virtual IEResult Initialize(const std::string& AppName, bool bAllowRunInBackground = false) = 0;
    virtual IEResult PostImGuiContextCreated() = 0;
    virtual void Deinitialize() = 0;
    virtual int32_t FlushGPUCommandsAndWait() = 0;
    
    virtual void CheckAndResizeSwapChain() = 0;
    virtual void NewFrame() = 0;
    virtual void RenderFrame(ImDrawData& DrawData) = 0;
    virtual void PresentFrame() = 0;

public:
    void PostWindowCreated();
    void RequestExit();

    void WaitEvents() const;
    void WaitEventsTimeout(double Timeout) const;
    void PollEvents() const;
    void PostEmptyEvent() const;

    bool IsAppRunning() const;
    bool IsAppWindowOpen() const;
    bool IsAppWindowMinimized() const;
    bool SupportsRunInBackground() const;

    void OnAppWindowCloseRequested();
    void OnAppWindowMinimizeRequested() const;
    void OnAppWindowRestoreRequested() const;

    void CloseAppWindow();
    void MinimizeAppWindow() const;
    void RestoreAppWindow() const;
    void NotifyOSRunInBackground() const;

    void AddOnWindowCloseCallbackFunc(uint32_t WindowID, const IEWindowCallbackFunc& Func);
    void AddOnWindowMinimizeCallbackFunc(uint32_t WindowID, const IEWindowCallbackFunc& Func);
    void AddOnWindowRestoreCallbackFunc(uint32_t WindowID, const IEWindowCallbackFunc& Func);

public:
    GLFWwindow* GetAppGLFWwindow() const { return m_AppWindow; }
    const std::string& GetAppName() const { return m_AppName; }
    uint32_t GetAppWindowID() const;
    std::string GetIELogoPathString() const;
    void DrawTelemetry() const;

private:
    void InitializeOSApp();
    void BroadcastOnWindowClosed() const;
    void BroadcastOnWindowMinimized() const;
    void BroadcastOnWindowRestored() const;
    
protected:
    GLFWwindow* m_AppWindow = nullptr;
    std::string m_AppName;
    int32_t m_DefaultAppWindowWidth = 1280;
    int32_t m_DefaultAppWindowHeight = 720;
    bool m_bAllowRunInBackground = false;

private:
    std::vector<std::pair<uint32_t, IEWindowCallbackFunc>> m_OnWindowCloseCallbackFunc;
    std::vector<std::pair<uint32_t, IEWindowCallbackFunc>> m_OnWindowMinimizeCallbackFunc;
    std::vector<std::pair<uint32_t, IEWindowCallbackFunc>> m_OnWindowRestoreCallbackFunc;

private:
    bool m_ExitRequested = false; 
};

class IERenderer_Vulkan : public IERenderer
{
public:
    /* Begin IERenderer Implementation */
    IEResult Initialize(const std::string& AppName, bool bAllowRunInBackground = false) override;
    IEResult PostImGuiContextCreated() override;
    void Deinitialize() override;
    int32_t FlushGPUCommandsAndWait() override;
    
    void CheckAndResizeSwapChain() override;
    void NewFrame() override;
    void RenderFrame(ImDrawData& DrawData) override;
    void PresentFrame() override;
    /* End IERenderer Implementation */

private:
    static void GlfwErrorCallbackFunc(int ErrorCode, const char* Description);
    static void CheckVkResultFunc(VkResult err);

private:
    IEResult InitializeVulkan();
    IEResult InitializeInstancePhysicalDevice();
    void DinitializeVulkan();

private:
    ImGui_ImplVulkanH_Window m_AppWindowVulkanData = {};

    VkAllocationCallbacks* m_VkAllocationCallback = nullptr;
    VkInstance m_VkInstance = nullptr;
    VkPhysicalDevice m_VkPhysicalDevice = nullptr;
    VkDevice m_VkDevice = nullptr;
    VkQueue m_VkQueue = nullptr;
    VkDebugReportCallbackEXT m_VkDebugReportCallbackEXT = nullptr;
    VkPipelineCache m_VkPipelineCache = nullptr;
    VkDescriptorPool m_VkDescriptorPool = nullptr;

    uint32_t m_QueueFamilyIndex = static_cast<uint32_t>(-1);
    int m_MinImageCount = 2;
    bool m_SwapChainRebuild = false;
};