#import "main_window.h"

@implementation MainWindow

- (instancetype)initWithTitle:(NSString*)title
{
	NSRect frame{ NSMakeRect(0, 0, 800, 600) };
	NSWindowStyleMask style{ NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable };
	self = [super initWithContentRect:frame styleMask:style backing:NSBackingStoreBuffered defer:NO];
	if (self)
	{
		[self setTitle:title];
		[self center];
	}
	return self;
}

@end
