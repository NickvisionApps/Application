#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include "theme.h"

namespace NickvisionApplication::Models
{
    class Configuration
    {
    public:
        Configuration();
        bool getIsFirstTimeOpen() const;
        void setIsFirstTimeOpen(bool isFirstTimeOpen);
        Theme getTheme() const;
        void setTheme(Theme theme);
        void save() const;

    private:
        std::string m_configDir;
        bool m_isFirstTimeOpen;
        Theme m_theme;
    };
}

#endif // CONFIGURATION_H
