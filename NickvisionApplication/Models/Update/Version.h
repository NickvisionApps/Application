#pragma once

#include <string>

namespace NickvisionApplication::Models::Update
{
    class Version
    {
    public:
        Version(const std::string& version);
        std::string ToString() const;
        bool operator<(const Version& toCompare) const;
        bool operator==(const Version& toCompare) const;
        bool operator!=(const Version& toCompare) const;
        bool operator>(const Version& toCompare) const;

    private:
        int m_major;
        int m_minor;
        int m_build;
    };
}


