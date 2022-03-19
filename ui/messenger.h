#ifndef MESSENGER_H
#define MESSENGER_H

#include <string>
#include <functional>
#include <map>

namespace NickvisionApplication::UI
{
    class Messenger
    {
    public:
        static Messenger& getInstance();
        Messenger(const Messenger&) = delete;
        void operator=(const Messenger&) = delete;
        void registerMessage(const std::string& messageName, const std::function<void(const std::string& parameter)>& callback);
        void deregisterMessage(const std::string& messageName);
        bool sendMessage(const std::string& messageName, const std::string& parameter);

    private:
        Messenger();
        std::map<std::string, std::function<void(const std::string& parameter)>> m_callbacks;
    };
}

#endif // MESSENGER_H
