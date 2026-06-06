#import "application.h"
#include "controllers/main_window_controller.h"
#import "views/main_window.h"

using namespace application::controllers;

@implementation Application
{
	std::shared_ptr<main_window_controller> _controller;
	MainWindow* _mainWindow;
}

- (instancetype)initWithController:(std::shared_ptr<main_window_controller>)controller
{
	if (self = [super init])
	{
		_controller = std::move(controller);
	}
	return self;
}

- (void)setupMainMenu:(NSString*)appName
{
	NSMenu* menuBar{ [[NSMenu alloc] init] };
	[NSApp setMainMenu:menuBar];
	NSMenuItem* appMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:appMenuItem];
	NSMenu* appMenu{ [[NSMenu alloc] init] };
	[appMenuItem setSubmenu:appMenu];
	[appMenu addItemWithTitle:[NSString stringWithFormat:@"About %@", appName] action:@selector(showAboutPanel:) keyEquivalent:@""];
	[appMenu addItem:[NSMenuItem separatorItem]];
	[appMenu addItemWithTitle:[NSString stringWithFormat:@"Quit %@", appName] action:@selector(terminate:) keyEquivalent:@"q"];
	NSMenuItem* fileMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:fileMenuItem];
	NSMenu* fileMenu{ [[NSMenu alloc] initWithTitle:@"File"] };
	[fileMenuItem setSubmenu:fileMenu];
	[fileMenu addItemWithTitle:@"Close" action:@selector(performClose:) keyEquivalent:@"w"];
	NSMenuItem* editMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:editMenuItem];
	NSMenu* editMenu{ [[NSMenu alloc] initWithTitle:@"Edit"] };
	[editMenuItem setSubmenu:editMenu];
	[editMenu addItemWithTitle:@"Undo" action:@selector(undo:) keyEquivalent:@"z"];
	[editMenu addItemWithTitle:@"Redo" action:@selector(redo:) keyEquivalent:@"Z"];
	[editMenu addItem:[NSMenuItem separatorItem]];
	[editMenu addItemWithTitle:@"Cut" action:@selector(cut:) keyEquivalent:@"x"];
	[editMenu addItemWithTitle:@"Copy" action:@selector(copy:) keyEquivalent:@"c"];
	[editMenu addItemWithTitle:@"Paste" action:@selector(paste:) keyEquivalent:@"v"];
	[editMenu addItemWithTitle:@"Select All" action:@selector(selectAll:) keyEquivalent:@"a"];
	NSMenuItem* windowMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:windowMenuItem];
	NSMenu* windowMenu{ [[NSMenu alloc] initWithTitle:@"Window"] };
	[windowMenuItem setSubmenu:windowMenu];
	[windowMenu addItemWithTitle:@"Minimize" action:@selector(performMiniaturize:) keyEquivalent:@"m"];
	[windowMenu addItemWithTitle:@"Zoom" action:@selector(performZoom:) keyEquivalent:@""];
	[NSApp setWindowsMenu:windowMenu];
}

- (void)showAboutPanel:(id)sender
{
	auto info{ _controller->get_app_info() };
	NSMutableAttributedString* credits{ [[NSMutableAttributedString alloc] init] };
	NSDictionary* boldAttrs
	{
		@{NSFontAttributeName : [NSFont boldSystemFontOfSize:NSFont.smallSystemFontSize]}
	};
	NSDictionary* normalAttrs
	{
		@{NSFontAttributeName : [NSFont systemFontOfSize:NSFont.smallSystemFontSize]}
	};
	auto appendPeople = [&](NSString* title, const std::unordered_map<std::string, std::string>& people)
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
	};
	auto appendLink = [&](NSString* label, const std::string& url)
	{
		if (url.empty())
		{
			return;
		}
		NSMutableDictionary* linkAttrs{ [normalAttrs mutableCopy] };
		linkAttrs[NSLinkAttributeName] = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:[label stringByAppendingString:@"\n"] attributes:linkAttrs]];
	};
	if (!info->get_description().empty())
	{
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:[NSString stringWithUTF8String:info->get_description().c_str()]
		                                                                attributes:normalAttrs]];
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n\n" attributes:normalAttrs]];
	}
	appendPeople(@"Developers", info->get_developers());
	appendPeople(@"Designers", info->get_designers());
	appendPeople(@"Artists", info->get_artists());
	if (!info->get_translation_credits().empty() && info->get_translation_credits() != "translation-credits")
	{
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:@"Translators\n" attributes:boldAttrs]];
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:[NSString stringWithUTF8String:info->get_translation_credits().c_str()]
		                                                                attributes:normalAttrs]];
		[credits appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n\n" attributes:normalAttrs]];
	}
	appendLink(@"Source Code", info->get_source_url());
	appendLink(@"Report an Issue", info->get_issues_url());
	appendLink(@"Discussions", info->get_discussions_url());
	for (const auto& [name, url] : info->get_extra_links())
	{
		appendLink([NSString stringWithUTF8String:name.c_str()], url);
	}
	NSString* versionStr{ info->get_version().empty() ? @"" : [NSString stringWithUTF8String:info->get_version().str().c_str()] };
	[NSApp orderFrontStandardAboutPanelWithOptions:@{
		NSAboutPanelOptionApplicationName : [NSString stringWithUTF8String:info->get_name().c_str()],
		NSAboutPanelOptionApplicationVersion : versionStr,
		NSAboutPanelOptionVersion : @"",
		NSAboutPanelOptionCredits : credits
	}];
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
	NSString* title{ [NSString stringWithUTF8String:_controller->get_app_info()->get_short_name().c_str()] };
	[self setupMainMenu:title];
	_mainWindow = [[MainWindow alloc] initWithTitle:title];
	[_mainWindow makeKeyAndOrderFront:nil];
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
		[_mainWindow makeKeyAndOrderFront:nil];
		[NSApp activateIgnoringOtherApps:YES];
	}
	return YES;
}

@end
