#import "controls/main_window_content.h"

using namespace application::controllers;
using namespace desktop::app;

@implementation MainWindowContent
{
	std::shared_ptr<main_window_controller> m_controller;
	std::shared_ptr<translation_service> m_translation_service;
}

- (instancetype)initWithController:(std::shared_ptr<main_window_controller>)controller
                translationService:(std::shared_ptr<translation_service>)translationService
{
	self = [super initWithNibName:@"main_window_content" bundle:nil];
	if (self)
	{
		m_controller = controller;
		m_translation_service = translationService;
	}
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