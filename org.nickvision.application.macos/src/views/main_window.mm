#import "views/main_window.h"

@implementation MainWindow

- (instancetype)initWithTitle:(NSString*)title
{
	NSRect frame{ NSMakeRect(0, 0, 800, 600) };
	NSWindowStyleMask style{ NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable |
		                     NSWindowStyleMaskFullSizeContentView };
	self = [super initWithContentRect:frame styleMask:style backing:NSBackingStoreBuffered defer:NO];
	if (self)
	{
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
