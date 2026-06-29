#import "views/main_window.h"
#include <format>
#include "controllers/main_window_controller.h"

using namespace application::controllers;
using namespace desktop::app;
using namespace desktop::services;

@implementation MainWindow
{
	std::shared_ptr<main_window_controller> m_controller;
	std::shared_ptr<app_info> m_app_info;
	std::shared_ptr<translation_service> m_translation_service;
}

- (instancetype)initWithServiceProvider:(std::shared_ptr<service_provider>)serviceProvider
{
	self = [super initWithWindowNibName:@"main_window"];
	if (self)
	{
		m_controller = serviceProvider->get_required<main_window_controller>();
		m_app_info = serviceProvider->get_required<app_info>();
		m_translation_service = serviceProvider->get_required<translation_service>();
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
	self.openFolderToolbarItem.label = @(m_translation_service->_("Open"));
	self.openFolderToolbarItem.toolTip = @(m_translation_service->_("Open Folder"));
	self.closeFolderToolbarItem.label = @(m_translation_service->_("Close"));
	self.closeFolderToolbarItem.toolTip = @(m_translation_service->_("Close Folder"));
	[self.window center];
}

- (IBAction)checkForUpdates:(id)sender
{
}

- (IBAction)openFolder:(id)sender
{
}

- (IBAction)closeFolder:(id)sender
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
