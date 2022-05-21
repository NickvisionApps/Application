#pragma once

#include <string>

namespace NickvisionApplication::Helpers::CurlHelpers
{
    bool downloadFile(const std::string& url, const std::string& path);
}