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
		[[NSBundle mainBundle] loadNibNamed:@"main_menu" owner:self topLevelObjects:nil];
	}
	return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
	NSMenu* mainMenu{ [[NSApplication sharedApplication] mainMenu] };
	mainMenu.itemArray[1].title = @(m_translation_service->_("File"));
	mainMenu.itemArray[2].title = @(m_translation_service->_("Edit"));
	mainMenu.itemArray[3].title = @(m_translation_service->_("View"));
	mainMenu.itemArray[4].title = @(m_translation_service->_("Window"));
	mainMenu.itemArray[5].title = @(m_translation_service->_("Help"));
	NSMenu* appMenu{ mainMenu.itemArray[0].submenu };
	appMenu.itemArray[0].title = @(m_translation_service->_("About {}", m_app_info->get_short_name()).c_str());
	appMenu.itemArray[1].title = @(m_translation_service->_("Check for Updates\u2026"));
	appMenu.itemArray[3].title = @(m_translation_service->_("Settings\u2026"));
	appMenu.itemArray[5].title = @(m_translation_service->_("Services"));
	appMenu.itemArray[7].title = @(m_translation_service->_("Hide {}", m_app_info->get_short_name()).c_str());
	appMenu.itemArray[8].title = @(m_translation_service->_("Hide Others"));
	appMenu.itemArray[9].title = @(m_translation_service->_("Show All"));
	appMenu.itemArray[11].title = @(m_translation_service->_("Quit {}", m_app_info->get_short_name()).c_str());
	NSMenu* fileMenu{ mainMenu.itemArray[1].submenu };
	fileMenu.title = @(m_translation_service->_("File"));
	fileMenu.itemArray[0].title = @(m_translation_service->_("Open Folder\u2026"));
	fileMenu.itemArray[1].title = @(m_translation_service->_("Close Folder"));
	fileMenu.itemArray[3].title = @(m_translation_service->_("Close Window"));
	NSMenu* editMenu{ mainMenu.itemArray[2].submenu };
	editMenu.title = @(m_translation_service->_("Edit"));
	editMenu.itemArray[0].title = @(m_translation_service->_("Undo"));
	editMenu.itemArray[1].title = @(m_translation_service->_("Redo"));
	editMenu.itemArray[3].title = @(m_translation_service->_("Cut"));
	editMenu.itemArray[4].title = @(m_translation_service->_("Copy"));
	editMenu.itemArray[5].title = @(m_translation_service->_("Paste"));
	editMenu.itemArray[6].title = @(m_translation_service->_("Select All"));
	NSMenu* viewMenu{ mainMenu.itemArray[3].submenu };
	viewMenu.title = @(m_translation_service->_("View"));
	viewMenu.itemArray[0].title = @(m_translation_service->_("Enter Full Screen"));
	NSMenu* windowMenu{ mainMenu.itemArray[4].submenu };
	windowMenu.title = @(m_translation_service->_("Window"));
	windowMenu.itemArray[0].title = @(m_translation_service->_("Minimize"));
	windowMenu.itemArray[1].title = @(m_translation_service->_("Zoom"));
	windowMenu.itemArray[3].title = @(m_translation_service->_("Bring All to Front"));
	NSMenu* helpMenu{ mainMenu.itemArray[5].submenu };
	helpMenu.title = @(m_translation_service->_("Help"));
	helpMenu.itemArray[0].title = @(m_translation_service->_("GitHub Repo"));
	helpMenu.itemArray[1].title = @(m_translation_service->_("Report a Bug"));
	helpMenu.itemArray[2].title = @(m_translation_service->_("Discussions"));
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
