#pragma once

#include <libdesktop.h>
#include <memory>
#include <tuple>

namespace application::macos
{
	class cocoa_lifetime_service : public desktop::hosting::lifetime_service
	{
	public:
		using dependencies = std::tuple<desktop::services::service_provider>;
		cocoa_lifetime_service(const std::shared_ptr<desktop::services::service_provider>& service_provider);
		~cocoa_lifetime_service() override = default;
		cocoa_lifetime_service(const cocoa_lifetime_service&) = delete;
		cocoa_lifetime_service(cocoa_lifetime_service&&) = delete;
		cocoa_lifetime_service& operator=(const cocoa_lifetime_service&) = delete;
		cocoa_lifetime_service& operator=(cocoa_lifetime_service&&) = delete;

	protected:
		void on_startup_and_run() override;
		void on_shutdown() noexcept override;
		void on_stop_requested() noexcept override;

	private:
		std::shared_ptr<desktop::services::service_provider> m_service_provider;
	};
}
