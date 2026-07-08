#import "controls/main_window_split_view.h"

@interface MainWindowSplitView () <MainWindowSidebarDelegate>
@end

namespace application::macos::controls
{
	main_window_split_view::main_window_split_view(std::shared_ptr<main_window_sidebar> sidebar, std::shared_ptr<main_window_content> content)
	    : m_split_view{ [[MainWindowSplitView alloc] initWithDependencies:sidebar->objc() content:content->objc()] }
	{
	}

	main_window_split_view::~main_window_split_view()
	{
		[m_split_view release];
	}

	MainWindowSplitView* main_window_split_view::objc() const
	{
		return m_split_view;
	}
}

@implementation MainWindowSplitView
{
	MainWindowSidebar* m_sidebar;
	MainWindowContent* m_content;
}

- (instancetype)initWithDependencies:(MainWindowSidebar*)sidebar content:(MainWindowContent*)content
{
	self = [super initWithNibName:nil bundle:nil];
	if (self)
	{
		m_sidebar = sidebar;
		m_content = content;
		m_sidebar.delegate = self;
	}
	return self;
}

- (void)viewDidLoad
{
	[super viewDidLoad];
	self.splitView.dividerStyle = NSSplitViewDividerStyleThin;
	self.splitView.vertical = YES;
	NSSplitViewItem* sidebarItem{ [NSSplitViewItem sidebarWithViewController:m_sidebar] };
	sidebarItem.minimumThickness = 180;
	sidebarItem.maximumThickness = 320;
	sidebarItem.canCollapse = NO;
	sidebarItem.allowsFullHeightLayout = YES;
	NSSplitViewItem* contentItem{ [NSSplitViewItem splitViewItemWithViewController:m_content] };
	contentItem.minimumThickness = 400;
	[self addSplitViewItem:sidebarItem];
	[self addSplitViewItem:contentItem];
}

- (void)mainWindowSidebar:(MainWindowSidebar*)controller didSelectItemAtIndex:(NSInteger)index
{
	[m_content selectIndex:index];
}

@end