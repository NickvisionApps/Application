#include "services/events_service.h"

using namespace application::events;
using namespace desktop::app;
using namespace desktop::events;
using namespace desktop::notifications;

namespace application::services
{
	events_service::events_service(std::shared_ptr<configuration_service> configuration_service, std::shared_ptr<folder_service> folder_service,
	                               std::shared_ptr<notification_service> notification_service)
	    : m_configuration_service{ std::move(configuration_service) },
	      m_folder_service{ std::move(folder_service) },
	      m_notification_service{ std::move(notification_service) }
	{
	}

	const event<notification_service, app_notification_sent_event_args>& events_service::get_app_notification_sent_event() const
	{
		return m_notification_service->get_app_notification_sent_event();
	}

	const event<configuration_service, configuration_saved_event_args>& events_service::get_configuration_saved_event() const
	{
		return m_configuration_service->get_saved_event();
	}

	const event<folder_service, folder_changed_event_args>& events_service::get_folder_changed_event() const
	{
		return m_folder_service->get_folder_changed_event();
	}
}