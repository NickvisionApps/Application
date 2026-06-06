#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libdesktop.h>
#include <stdexcept>

using namespace desktop::app;
using namespace desktop::hosting;
using namespace desktop::services;

class testing_lifetime_service : public lifetime_service
{
public:
	using dependencies = std::tuple<app_info, arguments_service>;
	testing_lifetime_service(const std::shared_ptr<app_info>& info, std::shared_ptr<arguments_service> arguments_service)
	    : lifetime_service{ info },
	      m_arguments_service{ std::move(arguments_service) }
	{
	}
	~testing_lifetime_service() override = default;

protected:
	void on_startup_and_run() override
	{
		int argc{ static_cast<int>(m_arguments_service->get_count()) };
		testing::InitGoogleTest(&argc, m_arguments_service->argv());
		testing::InitGoogleMock(&argc, m_arguments_service->argv());
		if (RUN_ALL_TESTS() == 1)
		{
			throw std::runtime_error("Running tests failed");
		}
	}

	void on_shutdown() noexcept override
	{
	}

	void on_stop_requested() noexcept override
	{
	}

private:
	std::shared_ptr<arguments_service> m_arguments_service;
};

int main(int argc, char* argv[])
{
	host host{ std::make_shared<app_info>("libdesktop.test", "Test", "Test", false), std::span<char*>{ argv, static_cast<size_t>(argc) } };
	host.get_services()->add<lifetime_service, testing_lifetime_service>(service_scope::singleton);
	return host.run() ? 1 : 0;
}