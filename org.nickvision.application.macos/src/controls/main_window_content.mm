#import "controls/main_window_content.h"

using namespace application::controllers;
using namespace desktop::app;

namespace application::macos::controls
{
	main_window_content::main_window_content(std::shared_ptr<application::controllers::main_window_controller> controller,
	                                         std::shared_ptr<desktop::app::translation_service> translation_service)
	    : m_content{ [[MainWindowContent alloc] initWithDependencies:std::move(controller) translationService:std::move(translation_service)] }
	{
	}

	main_window_content::~main_window_content()
	{
		[m_content release];
	}

	MainWindowContent* main_window_content::objc() const
	{
		return m_content;
	}
}

@implementation MainWindowContent
{
	std::shared_ptr<main_window_controller> m_controller;
	std::shared_ptr<translation_service> m_translation_service;
}

- (instancetype)initWithDependencies:(std::shared_ptr<main_window_controller>)controller
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