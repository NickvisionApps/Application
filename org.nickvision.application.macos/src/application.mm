#import "application.h"
#include "controllers/main_window_controller.h"
#import "main_window.h"

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

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
	NSString* title{ [NSString stringWithUTF8String:_controller->get_app_info()->get_short_name().c_str()] };
	_mainWindow = [[MainWindow alloc] initWithTitle:title];
	[_mainWindow makeKeyAndOrderFront:nil];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
	return YES;
}

@end
