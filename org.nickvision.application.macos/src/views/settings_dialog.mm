#import "views/settings_dialog.h"
#include "controllers/preferences_view_controller.h"

using namespace application::controllers;
using namespace desktop::app;
using namespace desktop::services;

@implementation SettingsDialog
{
	std::shared_ptr<preferences_view_controller> m_controller;
	std::shared_ptr<translation_service> m_translation_service;
}

- (instancetype)initWithServiceProvider:(std::shared_ptr<service_provider>)serviceProvider
{
	self = [super initWithWindowNibName:@"settings_dialog"];
	if (self)
	{
		m_controller = serviceProvider->get_required<preferences_view_controller>();
		m_translation_service = serviceProvider->get_required<translation_service>();
	}
	return self;
}

- (void)windowDidLoad
{
	[super windowDidLoad];
	self.window.title = @(m_translation_service->_("General"));
	self.generalToolbarItem.label = @(m_translation_service->_("General"));
	self.advancedToolbarItem.label = @(m_translation_service->_("Advanced"));
	self.toolbar.selectedItemIdentifier = self.generalToolbarItem.itemIdentifier;
	[self.window center];
}

- (IBAction)toolbarItemClicked:(id)sender
{
	if (sender == self.generalToolbarItem)
	{
		self.window.title = @(m_translation_service->_("General"));
	}
	else if (sender == self.advancedToolbarItem)
	{
		self.window.title = @(m_translation_service->_("Advanced"));
	}
}

@end
