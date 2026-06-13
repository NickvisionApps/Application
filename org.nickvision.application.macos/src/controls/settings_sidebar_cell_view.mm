#import "controls/settings_sidebar_cell_view.h"

@implementation SettingsSidebarCellView

- (instancetype)initWithFrame:(NSRect)frame
{
	if (self = [super initWithFrame:frame])
	{
		_iconView = [[NSImageView alloc] init];
		_iconView.translatesAutoresizingMaskIntoConstraints = NO;
		_iconView.imageScaling = NSImageScaleProportionallyDown;
		[self addSubview:_iconView];
		NSTextField* label{ [NSTextField labelWithString:@""] };
		label.translatesAutoresizingMaskIntoConstraints = NO;
		label.font = [NSFont systemFontOfSize:NSFont.systemFontSize];
		label.lineBreakMode = NSLineBreakByTruncatingTail;
		[self addSubview:label];
		self.textField = label;
		[NSLayoutConstraint activateConstraints:@[
			[_iconView.leadingAnchor constraintEqualToAnchor:self.leadingAnchor constant:8.0],
			[_iconView.centerYAnchor constraintEqualToAnchor:self.centerYAnchor],
			[_iconView.widthAnchor constraintEqualToConstant:16.0],
			[_iconView.heightAnchor constraintEqualToConstant:16.0],
			[label.leadingAnchor constraintEqualToAnchor:_iconView.trailingAnchor constant:8.0],
			[label.trailingAnchor constraintEqualToAnchor:self.trailingAnchor constant:-8.0],
			[label.centerYAnchor constraintEqualToAnchor:self.centerYAnchor],
		]];
	}
	return self;
}

- (void)configureWithItem:(SettingsSidebarItem*)item
{
	self.textField.stringValue = item.title;
	NSImageSymbolConfiguration* config{ [NSImageSymbolConfiguration configurationWithPointSize:14.0 weight:NSFontWeightRegular] };
	self.iconView.image = [[NSImage imageWithSystemSymbolName:item.symbolName accessibilityDescription:nil] imageWithSymbolConfiguration:config];
}

@end