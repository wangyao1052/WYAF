///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_FILE_FORMAT_H
#define WYDB_FILE_FORMAT_H

#include <string>
#include <cstdint>
#include <wydbDefs.h>

NS_WYDB_BEG

enum class FileType : unsigned short
{
    Text = 1,
    Binary = 2,
};

struct FileVersion
{
    std::uint8_t major;
    std::uint8_t minor;

    FileVersion() : major(0), minor(0) {}
    FileVersion(std::uint8_t argMajor, std::uint8_t argMinor)
        : major(argMajor), minor(argMinor) {}

    friend bool operator==(const FileVersion& a, const FileVersion& b)
    {
        return a.major == b.major && a.minor == b.minor;
    }
    friend bool operator!=(const FileVersion& a, const FileVersion& b)
    {
        return !(a == b);
    }
    friend bool operator<(const FileVersion& a, const FileVersion& b)
    {
        if (a.major != b.major) return a.major < b.major;
        else return a.minor < b.minor;
    }
    friend bool operator>(const FileVersion& a, const FileVersion& b)
    {
        return b < a;
    }
    friend bool operator<=(const FileVersion& a, const FileVersion& b)
    {
        return !(b < a);
    }
    friend bool operator>=(const FileVersion& a, const FileVersion& b)
    {
        return !(a < b);
    }
};

struct FileMarkers
{
    std::string fileHeader;
    std::string fileEnder;
    std::string elementsHeader;
    std::string elementsEnder;
    std::string elemEnder;
};

struct FileFormatConfig
{
    FileVersion version;
    FileMarkers markers;
};

NS_WYDB_END

#endif // WYDB_FILE_FORMAT_H