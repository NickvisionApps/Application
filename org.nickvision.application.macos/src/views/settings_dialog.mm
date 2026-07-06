#import "views/settings_dialog.h"

using namespace application::controllers;
using namespace application::models;
using namespace desktop::app;

namespace application::macos::views
{
	settings_dialog::settings_dialog(std::shared_ptr<application::controllers::preferences_view_controller> controller,
	                                 std::shared_ptr<desktop::app::translation_service> translation_service)
	    : m_dialog{ [[SettingsDialog alloc] initWithDependencies:std::move(controller) translationService:std::move(translation_service)] }
	{
	}

	settings_dialog::~settings_dialog()
	{
		[m_dialog release];
	}

	SettingsDialog* settings_dialog::objc() const
	{
		return m_dialog;
	}
}

@implementation SettingsDialog
{
	std::shared_ptr<preferences_view_controller> m_controller;
	std::shared_ptr<translation_service> m_translation_service;
}

- (instancetype)initWithDependencies:(std::shared_ptr<preferences_view_controller>)controller
                  translationService:(std::shared_ptr<translation_service>)translationService
{
	self = [super initWithWindowNibName:@"settings_dialog"];
	if (self)
	{
		m_controller = std::move(controller);
		m_translation_service = std::move(translationService);
	}
	return self;
}

- (void)windowDidLoad
{
	[super windowDidLoad];
	self.window.title = @(m_translation_service->_("General"));
	self.generalToolbarItem.label = @(m_translation_service->_("General"));
	self.toolbar.selectedItemIdentifier = self.generalToolbarItem.itemIdentifier;
	self.themeLabel.stringValue = @(m_translation_service->_("Theme:"));
	for (const std::string& theme : m_controller->get_available_themes())
	{
		[self.themePopupButton addItemWithTitle:@(theme.c_str())];
	}
	self.languageLabel.stringValue = @(m_translation_service->_("Translation Language:"));
	for (const std::string& language : m_controller->get_available_translation_languages())
	{
		[self.languagePopupButton addItemWithTitle:@(language.c_str())];
	}
	[self.themePopupButton selectItemAtIndex:m_controller->get_theme()];
	[self.languagePopupButton selectItemAtIndex:m_controller->get_translation_language()];
	[self.tabView selectTabViewItemAtIndex:0];
	[self.window center];
}

- (IBAction)toolbarItemClicked:(id)sender
{
	if (sender == self.generalToolbarItem)
	{
		self.window.title = @(m_translation_service->_("General"));
		[self.tabView selectTabViewItemAtIndex:0];
	}
}

- (IBAction)popupButtonChanged:(id)sender
{
	switch (static_cast<theme>(self.themePopupButton.indexOfSelectedItem))
	{
	case theme::light:
		[[NSApplication sharedApplication] setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameAqua]];
		break;
	case theme::dark:
		[[NSApplication sharedApplication] setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameDarkAqua]];
		break;
	default:
		[[NSApplication sharedApplication] setAppearance:nil];
		break;
	}
	m_controller->set_theme(self.themePopupButton.indexOfSelectedItem);
	m_controller->set_translation_language(self.languagePopupButton.indexOfSelectedItem);
}

@end
