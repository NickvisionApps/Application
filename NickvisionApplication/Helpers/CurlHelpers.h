#pragma once

#include <string>

/// <summary>
/// Functions for working with curl
/// </summary>
namespace NickvisionApplication::Helpers::CurlHelpers
{
    /// <summary>
    /// Downloads a file
    /// </summary>
    /// <param name="url">The url of the file</param>
    /// <param name="path">The path in which to download the file</param>
    /// <returns>True if the download was successful, else false</returns>
    bool downloadFile(const std::string& url, const std::string& path);
}