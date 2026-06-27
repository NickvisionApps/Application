#pragma once

#include <adwaita.h>
#include <libdesktop.h>
#include <memory>
#include <tuple>

#undef linux

namespace application::linux
{
	class gtk_lifetime_service : public desktop::hosting::lifetime_service
	{
	public:
		using dependencies = std::tuple<desktop::app::logger, desktop::services::service_provider>;
		gtk_lifetime_service(std::shared_ptr<desktop::app::logger> logger, std::shared_ptr<desktop::services::service_provider> service_provider);
		~gtk_lifetime_service() override;
		gtk_lifetime_service(const gtk_lifetime_service&) = delete;
		gtk_lifetime_service(gtk_lifetime_service&&) = delete;
		gtk_lifetime_service& operator=(const gtk_lifetime_service&) = delete;
		gtk_lifetime_service& operator=(gtk_lifetime_service&&) = delete;

	protected:
		void on_startup_and_run() override;
		void on_shutdown() noexcept override;
		void on_stop_requested() noexcept override;

	private:
		std::shared_ptr<desktop::app::logger> m_logger;
		std::shared_ptr<desktop::services::service_provider> m_service_provider;
		AdwApplication* m_application{ nullptr };
	};
}
