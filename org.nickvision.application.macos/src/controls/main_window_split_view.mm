#import "controls/main_window_split_view.h"
#import "controls/main_window_content.h"
#import "controls/main_window_sidebar.h"

using namespace desktop::app;

@interface MainWindowSplitView () <MainWindowSidebarDelegate>
@end

@implementation MainWindowSplitView
{
	std::shared_ptr<translation_service> m_translation_service;
	MainWindowSidebar* m_sidebar;
	MainWindowContent* m_content;
}

- (instancetype)initWithTranslationService:(std::shared_ptr<translation_service>)translationService
{
	self = [super initWithNibName:nil bundle:nil];
	if (self)
	{
		m_translation_service = translationService;
		m_sidebar = [[MainWindowSidebar alloc] initWithTranslationService:m_translation_service];
		m_content = [[MainWindowContent alloc] init];
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
	sidebarItem.canCollapse = YES;
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