#import "app_delegate.h"
#import "views/main_window.h"
#import "views/settings_dialog.h"

using namespace desktop::app;
using namespace desktop::services;

@implementation AppDelegate
{
	std::shared_ptr<service_provider> m_service_provider;
	std::shared_ptr<translation_service> m_translation_service;
	std::shared_ptr<app_info> m_app_info;
	MainWindow* m_main_window;
	SettingsDialog* m_settings_dialog;
}

- (instancetype)initWithServiceProvider:(std::shared_ptr<service_provider>)serviceProvider
{
	if (self = [super init])
	{
		m_service_provider = std::move(serviceProvider);
		m_translation_service = m_service_provider->get_required<translation_service>();
		m_app_info = m_service_provider->get_required<app_info>();
	}
	return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
	NSMenu* menuBar{ [[NSMenu alloc] init] };
	[NSApp setMainMenu:menuBar];
	NSMenuItem* appMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:appMenuItem];
	NSMenu* appMenu{ [[NSMenu alloc] init] };
	[appMenuItem setSubmenu:appMenu];
	[appMenu addItemWithTitle:@(m_translation_service->_("About {}", m_app_info->get_short_name()).c_str())
	                   action:@selector(showAboutPanel:)
	            keyEquivalent:@""];
	[appMenu addItemWithTitle:@(m_translation_service->_("Check for Updates\u2026")) action:@selector(checkForUpdates:) keyEquivalent:@""];
	[appMenu addItem:[NSMenuItem separatorItem]];
	[appMenu addItemWithTitle:@(m_translation_service->_("Settings\u2026")) action:@selector(showSettingsDialog:) keyEquivalent:@","];
	[appMenu addItem:[NSMenuItem separatorItem]];
	[appMenu addItemWithTitle:@(m_translation_service->_("Quit {}", m_app_info->get_short_name()).c_str()) action:@selector(terminate:) keyEquivalent:@"q"];
	NSMenuItem* fileMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:fileMenuItem];
	NSMenu* fileMenu
	{
		[[NSMenu alloc] initWithTitle:@(m_translation_service->_("File"))]
	};
	[fileMenuItem setSubmenu:fileMenu];
	[fileMenu addItemWithTitle:@(m_translation_service->_("Open Folder\u2026")) action:@selector(openFolder:) keyEquivalent:@"o"];
	[fileMenu addItemWithTitle:@(m_translation_service->_("Close Folder")) action:@selector(closeFolder:) keyEquivalent:@"w"];
	[fileMenu addItem:[NSMenuItem separatorItem]];
	[fileMenu addItemWithTitle:@(m_translation_service->_("Close")) action:@selector(performClose:) keyEquivalent:@"w"];
	NSMenuItem* windowMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:windowMenuItem];
	NSMenu* windowMenu
	{
		[[NSMenu alloc] initWithTitle:@(m_translation_service->_("Window"))]
	};
	[windowMenuItem setSubmenu:windowMenu];
	[windowMenu addItemWithTitle:@(m_translation_service->_("Minimize")) action:@selector(performMiniaturize:) keyEquivalent:@"m"];
	[windowMenu addItemWithTitle:@(m_translation_service->_("Zoom")) action:@selector(performZoom:) keyEquivalent:@""];
	[NSApp setWindowsMenu:windowMenu];
	NSMenuItem* helpMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:helpMenuItem];
	NSMenu* helpMenu
	{
		[[NSMenu alloc] initWithTitle:@(m_translation_service->_("Help"))]
	};
	[helpMenuItem setSubmenu:helpMenu];
	[helpMenu addItemWithTitle:@(m_translation_service->_("GitHub Repo")) action:@selector(openGitHubRepository:) keyEquivalent:@""];
	[helpMenu addItemWithTitle:@(m_translation_service->_("Report a Bug")) action:@selector(openBugReport:) keyEquivalent:@""];
	[helpMenu addItemWithTitle:@(m_translation_service->_("Discussions")) action:@selector(openDiscussions:) keyEquivalent:@""];
	[helpMenu addItem:[NSMenuItem separatorItem]];
	[helpMenu addItemWithTitle:@(m_translation_service->_("Credits")) action:@selector(showCreditsDialog:) keyEquivalent:@""];
	m_main_window = [[MainWindow alloc] initWithServiceProvider:m_service_provider];
	[m_main_window makeKeyAndOrderFront:nil];
	[NSApp activateIgnoringOtherApps:YES];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
	return NO;
}

- (BOOL)applicationShouldHandleReopen:(NSApplication*)sender hasVisibleWindows:(BOOL)flag
{
	if (!flag)
	{
		[m_main_window makeKeyAndOrderFront:nil];
		[NSApp activateIgnoringOtherApps:YES];
	}
	return YES;
}

- (void)showAboutPanel:(id)sender
{
	[NSApp orderFrontStandardAboutPanelWithOptions:@{
		NSAboutPanelOptionApplicationName : [NSString stringWithUTF8String:m_app_info->get_name().c_str()],
		NSAboutPanelOptionApplicationVersion : [NSString stringWithUTF8String:m_app_info->get_version().str().c_str()],
		NSAboutPanelOptionVersion : @""
	}];
}

- (void)checkForUpdates:(id)sender
{
}

- (void)showSettingsDialog:(id)sender
{
	if (!m_settings_dialog)
	{
		m_settings_dialog = [[SettingsDialog alloc] initWithServiceProvider:m_service_provider];
	}
	[m_settings_dialog show];
}

- (void)openFolder:(id)sender
{
}

- (void)closeFolder:(id)sender
{
}

- (void)openGitHubRepository:(id)sender
{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@(m_app_info->get_source_url().c_str())]];
}

- (void)openBugReport:(id)sender
{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@(m_app_info->get_issues_url().c_str())]];
}

- (void)openDiscussions:(id)sender
{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@(m_app_info->get_discussions_url().c_str())]];
}

- (void)showCreditsDialog:(id)sender
{
}

@end
