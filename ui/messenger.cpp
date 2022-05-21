#include "messenger.h"
#include <stdexcept>

using namespace NickvisionApplication::UI;

Messenger::Messenger()
{

}

void Messenger::registerMessage(const std::string& messageName, const std::function<void(const std::string& parameter)>& callback)
{
    if(m_callbacks.contains(messageName))
    {
        throw std::invalid_argument("A message with this name already exists.");
    }
    m_callbacks.insert({ messageName, callback });
}

void Messenger::deregisterMessage(const std::string& messageName)
{
    if(!m_callbacks.contains(messageName))
    {
        throw std::invalid_argument("No message with this name exists.");
    }
    m_callbacks.erase(messageName);
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
