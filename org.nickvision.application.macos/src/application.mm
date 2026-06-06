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

	// App menu
	NSMenuItem* appMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:appMenuItem];
	NSMenu* appMenu{ [[NSMenu alloc] init] };
	[appMenuItem setSubmenu:appMenu];
	[appMenu addItemWithTitle:[NSString stringWithFormat:@"About %@", appName] action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];
	[appMenu addItem:[NSMenuItem separatorItem]];
	[appMenu addItemWithTitle:[NSString stringWithFormat:@"Quit %@", appName] action:@selector(terminate:) keyEquivalent:@"q"];

	// File menu
	NSMenuItem* fileMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:fileMenuItem];
	NSMenu* fileMenu{ [[NSMenu alloc] initWithTitle:@"File"] };
	[fileMenuItem setSubmenu:fileMenu];
	[fileMenu addItemWithTitle:@"Close" action:@selector(performClose:) keyEquivalent:@"w"];

	// Edit menu
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

	// Window menu
	NSMenuItem* windowMenuItem{ [[NSMenuItem alloc] init] };
	[menuBar addItem:windowMenuItem];
	NSMenu* windowMenu{ [[NSMenu alloc] initWithTitle:@"Window"] };
	[windowMenuItem setSubmenu:windowMenu];
	[windowMenu addItemWithTitle:@"Minimize" action:@selector(performMiniaturize:) keyEquivalent:@"m"];
	[windowMenu addItemWithTitle:@"Zoom" action:@selector(performZoom:) keyEquivalent:@""];
	[NSApp setWindowsMenu:windowMenu];
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
