#include "messenger.h"

using namespace NickvisionApplication::UI;

Messenger::Messenger()
{

}

bool Messenger::registerMessage(const std::string& messageName, const std::function<void(const std::string& parameter)>& callback)
{
    if(m_callbacks.contains(messageName))
    {
        return false;
    }
    m_callbacks.insert({ messageName, callback });
    return true;
}

bool Messenger::deregisterMessage(const std::string& messageName)
{
    if(!m_callbacks.contains(messageName))
    {
        return false;
    }
    m_callbacks.erase(messageName);
    return true;
}

bool Messenger::sendMessage(const std::string& messageName, const std::string& parameter) const
{
    if(!m_callbacks.contains(messageName))
    {
        return false;
    }
    const std::function<void(const std::string& parameter)>& callback = m_callbacks.at(messageName);
    callback(parameter);
    return true;
}
