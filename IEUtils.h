// Copyright © 2024 mozahzah (Incus Entertainment). All rights reserved.

#pragma once

#include "IECore.h"

namespace IEUtils
{
    /* String Casting */

    template<typename ToCharType, typename FromCharType>
    struct StringCastImpl 
    {
        static std::basic_string<ToCharType> Cast(const FromCharType* String)
        {
            static_assert(sizeof(ToCharType) != sizeof(ToCharType), "StringCast specialization not implemented for this type combination.");
            return std::basic_string<ToCharType>();
        }
    };

    template<typename CharType>
    struct StringCastImpl<CharType, CharType> 
    {
        static std::basic_string<CharType> Cast(const CharType* String) 
        {
            return std::basic_string<CharType>(String);
        }
    };

    template<>
    struct StringCastImpl<char, wchar_t> 
    {
        static std::string Cast(const wchar_t* String) 
        {
            std::string ReturnString;
            std::mbstate_t State = std::mbstate_t();
            size_t Size = 0;
            if (wcsrtombs_s(&Size, nullptr, 0, &String, static_cast<size_t>(-1), &State) == 0)
            {   
                std::vector<char> Buffer(Size + 1);
                wcsrtombs_s(&Size, Buffer.data(), Buffer.size(), &String, static_cast<size_t>(-1), &State);
                Buffer[Size] = '\0';
                ReturnString = std::string(Buffer.data());
            }
            return ReturnString;
        }
    };

    template<>
    struct StringCastImpl<wchar_t, char>
    {
        static std::wstring Cast(const char* String)
        {
            std::wstring ReturnString;
            std::mbstate_t State = std::mbstate_t();
            size_t Size = 0;
            if (mbsrtowcs_s(&Size, nullptr, 0, &String, static_cast<size_t>(-1), &State) == 0)
            {   
                std::vector<wchar_t> Buffer(Size + 1);
                mbsrtowcs_s(&Size, Buffer.data(), Buffer.size(), &String, static_cast<size_t>(-1), &State);
                Buffer[Size] = L'\0';
                ReturnString = std::wstring(Buffer.data());
            }
            return ReturnString;
        }
    };

    template<typename ToCharType, typename FromCharType>
    std::basic_string<ToCharType> StringCast(const FromCharType* String) 
    {
        return StringCastImpl<ToCharType, FromCharType>::Cast(String);
    }

    /* Path Search Algorithms */
    
    std::filesystem::path FindFolderPathDownwards(const std::filesystem::path& Directory, const std::filesystem::path& FolderName);
    std::filesystem::path FindFolderPathUpwards(const std::filesystem::path& SearchDirectory, const std::filesystem::path& FolderName);
    std::filesystem::path GetIEMidiConfigFolderPath();
    bool IsFileHidden(const std::filesystem::path& Path);
}