#import "app_delegate.h"
#import "views/main_window.h"

using namespace desktop::app;
using namespace desktop::hosting;
using namespace desktop::services;

static void appendPeople(NSMutableAttributedString* credits, NSDictionary* boldAttrs, NSDictionary* normalAttrs, NSString* title,
                         const std::unordered_map<std::string, std::string>& people)
{
	if (people.empty())
	{
		return;
	}
	[credits appendAttributedString:[[NSAttributedString alloc] initWithString:[title stringByAppendingString:@"\n"] attributes:boldAttrs]];
	for (const auto& [name, url] : people)
	{
		NSString* nsName{ [NSString stringWithUTF8String:name.c_str()] };
		if (!url.empty())
		{
			NSMutableDictionary* linkAttrs{ [normalAttrs mutableCopy] };
			linkAttrs[NSLinkAttributeName] = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
			[credits appendAttributedString:[[NSAttributedString alloc] initWithString:[@"  " stringByAppendingString:nsName] attributes:linkAttrs]];
		}
		else
		{
			[credits appendAttributedString:[[NSAttributedString alloc] initWithString:[@"  " stringByAppendingString:nsName] attributes:normalAttrs]];
		}
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n" attributes:normalAttrs]];
	}
	[credits appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n" attributes:normalAttrs]];
}

@implementation AppDelegate
{
	std::shared_ptr<service_provider> m_service_provider;
	std::shared_ptr<app_info> m_app_info;
	std::shared_ptr<lifetime_service> m_lifetime_service;
	std::shared_ptr<translation_service> m_translation_service;
	MainWindow* m_main_window;
}

- (instancetype)initWithServiceProvider:(std::shared_ptr<service_provider>)serviceProvider
{
	if (self = [super init])
	{
		m_service_provider = std::move(serviceProvider);
		m_app_info = m_service_provider->get_required<app_info>();
		m_lifetime_service = m_service_provider->get_required<lifetime_service>();
		m_translation_service = m_service_provider->get_required<translation_service>();
	}
	return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
	NSMenu* appMenu{ [[NSMenu alloc] init] };
	[appMenu addItemWithTitle:@(m_translation_service->_("About {}", m_app_info->get_short_name()).c_str()) action:@selector(about:) keyEquivalent:@""];
	[appMenu addItemWithTitle:@(m_translation_service->_("Check for Updates\u2026")) action:@selector(checkForUpdates:) keyEquivalent:@""];
	[appMenu addItem:[NSMenuItem separatorItem]];
	[appMenu addItemWithTitle:@(m_translation_service->_("Settings\u2026")) action:@selector(settings:) keyEquivalent:@","];
	[appMenu addItem:[NSMenuItem separatorItem]];
	NSMenuItem* servicesItem{ [[NSMenuItem alloc] initWithTitle:@"Services" action:nil keyEquivalent:@""] };
	NSMenu* servicesMenu{ [[NSMenu alloc] initWithTitle:@"Services"] };
	[servicesItem setSubmenu:servicesMenu];
	[appMenu addItem:servicesItem];
	[appMenu addItem:[NSMenuItem separatorItem]];
	[appMenu addItemWithTitle:@(m_translation_service->_("Hide {}", m_app_info->get_short_name()).c_str()) action:@selector(hide:) keyEquivalent:@"h"];
	NSMenuItem* hideOthersItem
	{
		[[NSMenuItem alloc] initWithTitle:@(m_translation_service->_("Hide Others")) action:@selector(hideOtherApplications:) keyEquivalent:@"h"]
	};
	hideOthersItem.keyEquivalentModifierMask = NSEventModifierFlagOption | NSEventModifierFlagCommand;
	[appMenu addItem:hideOthersItem];
	[appMenu addItemWithTitle:@(m_translation_service->_("Show All")) action:@selector(unhideAllApplications:) keyEquivalent:@""];
	[appMenu addItem:[NSMenuItem separatorItem]];
	[appMenu addItemWithTitle:@(m_translation_service->_("Quit {}", m_app_info->get_short_name()).c_str()) action:@selector(quit:) keyEquivalent:@"q"];
	NSMenu* fileMenu
	{
		[[NSMenu alloc] initWithTitle:@(m_translation_service->_("File"))]
	};
	[fileMenu addItemWithTitle:@(m_translation_service->_("Open Folder\u2026")) action:@selector(openFolder:) keyEquivalent:@"o"];
	[fileMenu addItemWithTitle:@(m_translation_service->_("Close Folder")) action:@selector(closeFolder:) keyEquivalent:@"W"];
	[fileMenu addItem:[NSMenuItem separatorItem]];
	[fileMenu addItemWithTitle:@(m_translation_service->_("Close Window")) action:@selector(performClose:) keyEquivalent:@"w"];
	NSMenu* windowMenu
	{
		[[NSMenu alloc] initWithTitle:@(m_translation_service->_("Window"))]
	};
	[windowMenu addItemWithTitle:@(m_translation_service->_("Minimize")) action:@selector(performMiniaturize:) keyEquivalent:@"m"];
	[windowMenu addItemWithTitle:@(m_translation_service->_("Zoom")) action:@selector(performZoom:) keyEquivalent:@""];
	[windowMenu addItem:[NSMenuItem separatorItem]];
	[windowMenu addItemWithTitle:@(m_translation_service->_("Bring All to Front")) action:@selector(arrangeInFront:) keyEquivalent:@""];
	NSMenu* helpMenu
	{
		[[NSMenu alloc] initWithTitle:@(m_translation_service->_("Help"))]
	};
	[helpMenu addItemWithTitle:@(m_translation_service->_("GitHub Repo")) action:@selector(openGitHubRepository:) keyEquivalent:@""];
	[helpMenu addItemWithTitle:@(m_translation_service->_("Report a Bug")) action:@selector(openBugReport:) keyEquivalent:@""];
	[helpMenu addItemWithTitle:@(m_translation_service->_("Discussions")) action:@selector(openDiscussions:) keyEquivalent:@""];
	NSMenu* menuBar{ [[NSMenu alloc] init] };
	[menuBar addItem:[[NSMenuItem alloc] init]];
	[menuBar.itemArray.lastObject setSubmenu:appMenu];
	[menuBar addItem:[[NSMenuItem alloc] init]];
	[menuBar.itemArray.lastObject setSubmenu:fileMenu];
	[menuBar addItem:[[NSMenuItem alloc] init]];
	[menuBar.itemArray.lastObject setSubmenu:windowMenu];
	[menuBar addItem:[[NSMenuItem alloc] init]];
	[menuBar.itemArray.lastObject setSubmenu:helpMenu];
	[[NSApplication sharedApplication] setMainMenu:menuBar];
	[[NSApplication sharedApplication] setServicesMenu:servicesMenu];
	[[NSApplication sharedApplication] setWindowsMenu:windowMenu];
	m_main_window = [[MainWindow alloc] initWithServiceProvider:m_service_provider];
	[m_main_window showWindow:nil];
	[[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender
{
	[self quit:sender];
	return NSTerminateLater;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
	return NO;
}

- (BOOL)applicationShouldHandleReopen:(NSApplication*)sender hasVisibleWindows:(BOOL)flag
{
	if (!flag)
	{
		[m_main_window showWindow:nil];
		[[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
	}
	return YES;
}

- (IBAction)about:(id)sender
{
	NSMutableAttributedString* credits{ [[NSMutableAttributedString alloc] init] };
	NSDictionary* boldAttrs
	{
		@{NSFontAttributeName : [NSFont boldSystemFontOfSize:NSFont.smallSystemFontSize]}
	};
	NSDictionary* normalAttrs
	{
		@{NSFontAttributeName : [NSFont systemFontOfSize:NSFont.smallSystemFontSize]}
	};
	if (!m_app_info->get_description().empty())
	{
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:[NSString stringWithUTF8String:m_app_info->get_description().c_str()]
		                                                                attributes:normalAttrs]];
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n\n" attributes:normalAttrs]];
	}
	appendPeople(credits, boldAttrs, normalAttrs, @"Developers", m_app_info->get_developers());
	appendPeople(credits, boldAttrs, normalAttrs, @"Designers", m_app_info->get_designers());
	appendPeople(credits, boldAttrs, normalAttrs, @"Artists", m_app_info->get_artists());
	if (!m_app_info->get_translation_credits().empty() && m_app_info->get_translation_credits() != "translation-credits")
	{
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:@"Translators\n" attributes:boldAttrs]];
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:[NSString stringWithUTF8String:m_app_info->get_translation_credits().c_str()]
		                                                                attributes:normalAttrs]];
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n\n" attributes:normalAttrs]];
	}
	[[NSApplication sharedApplication] orderFrontStandardAboutPanelWithOptions:@{
		NSAboutPanelOptionApplicationName : [NSString stringWithUTF8String:m_app_info->get_name().c_str()],
		NSAboutPanelOptionApplicationVersion : [NSString stringWithUTF8String:m_app_info->get_version().str().c_str()],
		NSAboutPanelOptionVersion : @"",
		NSAboutPanelOptionCredits : credits
	}];
}

- (IBAction)checkForUpdates:(id)sender
{
	if (!m_main_window)
	{
		return;
	}
	[m_main_window showWindow:nil];
	[m_main_window closeFolder];
}

- (IBAction)closeFolder:(id)sender
{
	if (!m_main_window)
	{
		return;
	}
	[m_main_window showWindow:nil];
	[m_main_window closeFolder];
}

- (IBAction)openBugReport:(id)sender
{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@(m_app_info->get_issues_url().c_str())]];
}

- (IBAction)openDiscussions:(id)sender
{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@(m_app_info->get_discussions_url().c_str())]];
}

- (IBAction)openFolder:(id)sender
{
	if (!m_main_window)
	{
		return;
	}
	[m_main_window showWindow:nil];
	[m_main_window openFolder];
}

- (IBAction)openGitHubRepository:(id)sender
{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@(m_app_info->get_source_url().c_str())]];
}

- (IBAction)quit:(id)sender
{
	m_lifetime_service->request_stop();
}

- (IBAction)settings:(id)sender
{
}

@end
