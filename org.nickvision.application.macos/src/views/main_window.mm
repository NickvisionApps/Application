#import "views/main_window.h"
#include <format>
#include "controllers/main_window_controller.h"
#include "services/events_service.h"

using namespace application::controllers;
using namespace application::events;
using namespace application::models;
using namespace application::services;
using namespace desktop::app;
using namespace desktop::notifications;
using namespace desktop::services;

@implementation MainWindow
{
	std::shared_ptr<main_window_controller> m_controller;
	std::shared_ptr<app_info> m_app_info;
	std::shared_ptr<events_service> m_events_service;
	std::shared_ptr<translation_service> m_translation_service;
}

- (instancetype)initWithServiceProvider:(std::shared_ptr<service_provider>)serviceProvider
{
	self = [super initWithWindowNibName:@"main_window"];
	if (self)
	{
		m_controller = serviceProvider->get_required<main_window_controller>();
		m_app_info = serviceProvider->get_required<app_info>();
		m_events_service = serviceProvider->get_required<events_service>();
		m_translation_service = serviceProvider->get_required<translation_service>();
		m_events_service->get_app_notification_sent_event() += [self](const notification_service& /* unused */, const app_notification_sent_event_args& args)
		{
			if ([NSThread isMainThread])
			{
				[self onAppNotificationSent:args];
				return;
			}
			app_notification_sent_event_args captured{ args };
			dispatch_async(dispatch_get_main_queue(), ^{ [self onAppNotificationSent:captured]; });
		};
		m_events_service->get_folder_changed_event() += [self](const folder_service& /* unused */, const folder_changed_event_args& args)
		{
			[self onFolderChanged:args];
		};
	}
	return self;
}

- (void)windowDidLoad
{
	[super windowDidLoad];
	self.window.title = [NSString stringWithUTF8String:m_app_info->get_short_name().c_str()];
	if (m_app_info->get_version().is_preview())
	{
		self.window.subtitle = @(m_translation_service->_("Preview"));
	}
	self.closeFolderToolbarItem.label = @(m_translation_service->_("Close"));
	self.closeFolderToolbarItem.toolTip = @(m_translation_service->_("Close Folder"));
	self.openFolderToolbarItem.label = @(m_translation_service->_("Open"));
	self.openFolderToolbarItem.toolTip = @(m_translation_service->_("Open Folder"));
	[self.window center];
}

- (IBAction)checkForUpdates:(id)sender
{
}

- (IBAction)openFolder:(id)sender
{
	NSOpenPanel* openPanel{ [NSOpenPanel openPanel] };
	openPanel.canChooseFiles = NO;
	openPanel.canChooseDirectories = YES;
	openPanel.allowsMultipleSelection = NO;
	if ([openPanel runModal] == NSModalResponseOK)
	{
		m_controller->open_folder({ [openPanel.URLs[0].path UTF8String] });
	}
}

- (IBAction)closeFolder:(id)sender
{
	m_controller->close_folder();
}

- (void)onAppNotificationSent:(const app_notification_sent_event_args&)args
{
	NSAlert* alert{ [[NSAlert alloc] init] };
	switch (args.get_notification().get_severity())
	{
	case notification_severity::success:
		alert.messageText = @(m_translation_service->_("Success"));
		alert.alertStyle = NSAlertStyleInformational;
		break;
	case notification_severity::warning:
		alert.messageText = @(m_translation_service->_("Warning"));
		alert.alertStyle = NSAlertStyleWarning;
		break;
	case notification_severity::error:
		alert.messageText = @(m_translation_service->_("Error"));
		alert.alertStyle = NSAlertStyleCritical;
		break;
	default:
		alert.messageText = @(m_translation_service->_("Information"));
		alert.alertStyle = NSAlertStyleInformational;
		break;
	}
	alert.informativeText = @(args.get_notification().get_message().c_str());
	[alert addButtonWithTitle:@(m_translation_service->_("OK"))];
	if (args.get_notification().get_action() == "close")
	{
		[alert addButtonWithTitle:@(m_translation_service->_("Close"))];
	}
	NSModalResponse response{ [alert runModal] };
	if (response == NSAlertSecondButtonReturn && args.get_notification().get_action() == "close")
	{
		[self closeFolder:nil];
	}
}

- (void)onFolderChanged:(const folder_changed_event_args&)args
{
}

- (IBAction)viewDebuggingInformation:(id)sender
{
	std::string extra{ std::format("AppKit {}", NSAppKitVersionNumber) };
	std::string info{ m_controller->get_debugging_information(extra) };
	NSPasteboard* pasteboard{ [NSPasteboard generalPasteboard] };
	[pasteboard clearContents];
	[pasteboard setString:@(info.c_str()) forType:NSPasteboardTypeString];
	NSAlert* alert{ [[NSAlert alloc] init] };
	alert.messageText = @(m_translation_service->_("Debugging Information"));
	alert.informativeText = @(m_translation_service->_("The following information has been copied:\n\n{}", info).c_str());
	[alert addButtonWithTitle:@(m_translation_service->_("OK"))];
	[alert runModal];
}

@end
