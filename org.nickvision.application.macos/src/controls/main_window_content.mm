#import "controls/main_window_content.h"

@implementation MainWindowContent

- (instancetype)init
{
	self = [super initWithNibName:@"main_window_content" bundle:nil];
	return self;
}

- (void)viewDidLoad
{
	[super viewDidLoad];
}

- (void)selectIndex:(NSInteger)index
{
	if (index < 0 || index >= (NSInteger)self.tabView.numberOfTabViewItems)
	{
		return;
	}
	[self.tabView selectTabViewItemAtIndex:index];
}

@end