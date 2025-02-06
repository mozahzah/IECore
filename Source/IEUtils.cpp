// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEUtils.h"

namespace IEUtils
{
    /* Path Search Algorithms */

    std::filesystem::path FindFolderPathDownwards(const std::filesystem::path& SearchDirectory, const std::filesystem::path& FolderName)
    {
        if (!std::filesystem::is_directory(SearchDirectory))
        {
            return std::filesystem::path();
        }

        const std::filesystem::path& TargetPath = SearchDirectory / FolderName;
        if (std::filesystem::is_directory(TargetPath))
        {
            return TargetPath;
        }

        for (const std::filesystem::directory_entry& Entry : std::filesystem::directory_iterator(SearchDirectory))
        {
            const std::filesystem::path& ChildPath = Entry.path();
            if (std::filesystem::is_directory(ChildPath))
            {
                const std::filesystem::path& TargetPath = FindFolderPathDownwards(ChildPath, FolderName);
                if (!TargetPath.empty())
                {
                    return TargetPath;
                }
            }
        }

        return std::filesystem::path();
    }

    std::filesystem::path FindFolderPathUpwards(const std::filesystem::path& SearchDirectory, const std::filesystem::path& FolderName)
    {
        if (!std::filesystem::is_directory(SearchDirectory))
        {
            return std::filesystem::path();
        }

        const std::filesystem::path& TargetPath = SearchDirectory / FolderName;
        if (std::filesystem::is_directory(TargetPath))
        {
            return TargetPath;
        }

        const std::filesystem::path& ParentPath = SearchDirectory.parent_path();
        if (ParentPath == SearchDirectory)
        {
            return std::filesystem::path();
        }

        return FindFolderPathUpwards(ParentPath, FolderName);
    }

    std::filesystem::path GetIEConfigFolderPath()
    {
        static constexpr char IEConfigFolderName[] = "IE";
#if defined (_WIN32)
        PWSTR AppDataFolderPath = NULL;
        const HRESULT Result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &AppDataFolderPath);
        if (SUCCEEDED(Result))
        {
            const _bstr_t BstrAppDataFolderPath(AppDataFolderPath);
            const std::wstring StrAppDataFolderPath(BstrAppDataFolderPath, SysStringLen(BstrAppDataFolderPath));

            const std::filesystem::path IEConfigFolderPath = std::filesystem::path(StrAppDataFolderPath) / IEConfigFolderName;
            if (std::filesystem::exists(IEConfigFolderPath) || std::filesystem::create_directory(IEConfigFolderPath))
            {
                return IEConfigFolderPath;
            }
        }
#elif defined(__APPLE__) || defined(__linux__)
        const char* HomeFolderPath;
        if ((HomeFolderPath = getenv("HOME")) == NULL)
        {
            HomeFolderPath = getpwuid(getuid())->pw_dir;
        }

        const std::filesystem::path& IEConfigFolderPath = std::filesystem::path(HomeFolderPath) / IEConfigFolderName;
        if (std::filesystem::exists(IEConfigFolderPath) || std::filesystem::create_directory(IEConfigFolderPath))
        {
            return IEConfigFolderPath;
        }
#endif  

        IELOG_ERROR("Failed to create IEConfig folder");
        return std::filesystem::path();
    }

    bool IsFileHidden(const std::filesystem::path& Path)
    {
        bool bIsHidden = false;
        if (std::filesystem::exists(Path))
        {
#if defined (_WIN32)
            const DWORD Attributes = GetFileAttributes(Path.string().c_str());
            if ((Attributes & FILE_ATTRIBUTE_HIDDEN) != 0)
            {
                bIsHidden = true;
            }
#elif defined(__APPLE__) || defined(__linux__)
            const std::string& FileName = Path.filename().string();
            if (!FileName.empty() && FileName[0] == '.')
            {
                bIsHidden = true;
            }
#endif
        }
        return bIsHidden;
    }
}