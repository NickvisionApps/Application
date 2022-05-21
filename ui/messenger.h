#ifndef MESSENGER_H
#define MESSENGER_H

#include <string>
#include <functional>
#include <unordered_map>

namespace NickvisionApplication::UI
{
    class Messenger
    {
    public:
        Messenger();
        void registerMessage(const std::string& messageName, const std::function<void(const std::string& parameter)>& callback);
        void deregisterMessage(const std::string& messageName);
        bool sendMessage(const std::string& messageName, const std::string& parameter) const;

    private:
        std::unordered_map<std::string, std::function<void(const std::string& parameter)>> m_callbacks;
    };
}

#endif // MESSENGER_H
