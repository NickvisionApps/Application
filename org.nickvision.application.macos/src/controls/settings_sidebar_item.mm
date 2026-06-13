#import "controls/settings_sidebar_item.h"

@implementation SettingsSidebarItem

- (instancetype)initWithTitle:(NSString*)title symbolName:(NSString*)symbolName
{
	if (self = [super init])
	{
		_title = [title copy];
		_symbolName = [symbolName copy];
	}
	return self;
}

@end