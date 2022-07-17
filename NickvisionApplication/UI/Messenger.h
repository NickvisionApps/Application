#pragma once

#include <string>
#include <functional>
#include <unordered_map>

namespace NickvisionApplication::UI
{
	class Messenger
	{
	public:
		static Messenger& getInstance();
		bool registerMessage(const std::string& name, const std::function<void(void* parameter)>& callback);
		bool deregisterMessage(const std::string& name);
		bool sendMessage(const std::string& name, void* parameter) const;

	private:
		Messenger();
		std::unordered_map<std::string, std::function<void(void* parameter)>> m_messages;
	};
}

