#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>

namespace NickvisionApplication::Models
{
    class Configuration
    {
    public:
        Configuration();
        bool isFirstTimeOpen() const;
        void setIsFirstTimeOpen(bool isFirstTimeOpen);
        void save() const;

    private:
        std::string m_configDir;
        bool m_isFirstTimeOpen;
    };
}

#endif // CONFIGURATION_H
