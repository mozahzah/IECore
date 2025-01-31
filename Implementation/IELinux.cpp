// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "Source/IERenderer.h"

#if defined (__linux__)

#include <iostream>

#include <libnotify/notify.h>

extern void InitializeIELinuxApp(IERenderer* Renderer)
{
}

extern void ShowRunningInBackgroundLinuxNotification(const IERenderer* Renderer)
{
    if (Renderer)
    {
        notify_init("IECore Notification");
        const std::string& Message = std::format("{} is running in the background", Renderer->GetAppName());
        if (NotifyNotification* const Notification = notify_notification_new("Running in background", Message.c_str(),
            Renderer->GetIELogoPathString().c_str()))
        {
            notify_notification_set_timeout(Notification, NOTIFY_EXPIRES_DEFAULT);
            notify_notification_show(Notification, nullptr);
        }
    }
}
#endif