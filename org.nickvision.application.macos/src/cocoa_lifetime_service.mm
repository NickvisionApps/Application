#include "cocoa_lifetime_service.h"
#import <AppKit/AppKit.h>
#import "application.h"

namespace application::macos
{
cocoa_lifetime_service::cocoa_lifetime_service(const std::shared_ptr<desktop::app::app_info>& app_info,
                                               std::shared_ptr<application::controllers::main_window_controller> controller)
    : lifetime_service{ app_info }, m_controller{ std::move(controller) }
{
}

void cocoa_lifetime_service::on_startup_and_run()
{
	@autoreleasepool
	{
		NSApplication* app{ [NSApplication sharedApplication] };
		Application* delegate{ [[Application alloc] initWithController:m_controller] };
		[app setDelegate:delegate];
		[app run];
	}
}

void cocoa_lifetime_service::on_shutdown() noexcept
{
}

void cocoa_lifetime_service::on_stop_requested() noexcept
{
	[NSApp performSelectorOnMainThread:@selector(terminate:) withObject:nil waitUntilDone:NO];
}
}
