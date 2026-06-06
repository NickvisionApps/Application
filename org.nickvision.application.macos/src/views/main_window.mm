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
		[self center];
	}
	return self;
}

@end
