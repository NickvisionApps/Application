#import "views/main_window.h"
#include "controllers/main_window_controller.h"

using namespace application::controllers;
using namespace desktop::services;

@implementation MainWindow
{
	std::shared_ptr<main_window_controller> m_controller;
}

- (instancetype)initWithTitle:(NSString*)title serviceProvider:(std::shared_ptr<service_provider>)serviceProvider
{
	NSRect frame{ NSMakeRect(0, 0, 800, 600) };
	NSWindowStyleMask style{ NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable |
		                     NSWindowStyleMaskFullSizeContentView };
	self = [super initWithContentRect:frame styleMask:style backing:NSBackingStoreBuffered defer:NO];
	if (self)
	{
		m_controller = serviceProvider->get_required<main_window_controller>();
		[self setTitle:title];
		self.titlebarAppearsTransparent = YES;
		self.releasedWhenClosed = NO;
		self.minSize = NSMakeSize(600, 400);
		if (@available(macOS 12.0, *))
		{
			self.titlebarSeparatorStyle = NSTitlebarSeparatorStyleNone;
		}
		NSToolbar* toolbar{ [[NSToolbar alloc] initWithIdentifier:@"MainToolbar"] };
		toolbar.displayMode = NSToolbarDisplayModeIconOnly;
		self.toolbar = toolbar;
		self.toolbarStyle = NSWindowToolbarStyleUnified;
		[self center];
	}
	return self;
}

@end
