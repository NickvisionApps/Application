#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <mutex>
#include <string>
#include "theme.h"

namespace NickvisionApplication::Models
{
    class Configuration
    {
    public:
        static Configuration& getInstance();
        Configuration(const Configuration&) = delete;
        Configuration(Configuration&&) = delete;
        void operator=(const Configuration&) = delete;
        void operator=(Configuration&&) = delete;
        Theme getTheme() const;
        void setTheme(Theme theme);
        bool getIsFirstTimeOpen() const;
        void setIsFirstTimeOpen(bool isFirstTimeOpen);
        void save() const;

    private:
        Configuration();
        mutable std::mutex m_mutex;
        std::string m_configDir;
        Theme m_theme;
        bool m_isFirstTimeOpen;
    };
}

#endif // CONFIGURATION_H
