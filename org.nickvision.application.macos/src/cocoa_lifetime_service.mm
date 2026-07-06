#include "cocoa_lifetime_service.h"
#import <AppKit/AppKit.h>
#import "app_delegate.h"

using namespace desktop::app;
using namespace desktop::hosting;
using namespace desktop::services;

namespace application::macos
{
	cocoa_lifetime_service::cocoa_lifetime_service(std::shared_ptr<logger> logger, std::shared_ptr<service_provider> service_provider)
	    : lifetime_service{ service_provider->get_required<app_info>() },
	      m_logger{ std::move(logger) },
	      m_service_provider{ std::move(service_provider) }
	{
	}

	void cocoa_lifetime_service::on_startup_and_run()
	{
		m_logger->info("Starting macOS application lifetime service...");
		m_service_provider->get_required<app_delegate>();
		[[NSApplication sharedApplication] run];
	}

	void cocoa_lifetime_service::on_shutdown() noexcept
	{
		m_logger->info("Shutting down macOS application lifetime service...");
	}

	void cocoa_lifetime_service::on_stop_requested() noexcept
	{
		m_logger->info("Stop requested for macOS application lifetime service.");
		dispatch_async(dispatch_get_main_queue(), ^{
		  [[NSApplication sharedApplication] replyToApplicationShouldTerminate:NO];
		  [[NSApplication sharedApplication] stop:nil];
		  NSEvent* event{ [NSEvent otherEventWithType:NSEventTypeApplicationDefined
				                             location:NSZeroPoint
				                        modifierFlags:0
				                            timestamp:0
				                         windowNumber:0
				                              context:nil
				                              subtype:0
				                                data1:0
				                                data2:0] };
		  [[NSApplication sharedApplication] postEvent:event atStart:YES];
		});
	}
}
