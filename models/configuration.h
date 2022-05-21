#pragma once

#include <mutex>
#include <string>
#include "theme.h"

namespace NickvisionApplication::Models
{
    class Configuration
    {
    public:
        Configuration();
        Theme getTheme() const;
        void setTheme(Theme theme);
        bool getIsFirstTimeOpen() const;
        void setIsFirstTimeOpen(bool isFirstTimeOpen);
        void save() const;

    private:
        mutable std::mutex m_mutex;
        std::string m_configDir;
        Theme m_theme;
        bool m_isFirstTimeOpen;
    };
}