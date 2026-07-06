#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>
#include <tuple>
#include "controllers/preferences_view_controller.h"

@interface SettingsDialog : NSWindowController
@property(nonatomic, assign) IBOutlet NSToolbarItem* generalToolbarItem;
@property(nonatomic, assign) IBOutlet NSTextField* languageLabel;
@property(nonatomic, assign) IBOutlet NSPopUpButton* languagePopupButton;
@property(nonatomic, assign) IBOutlet NSTabView* tabView;
@property(nonatomic, assign) IBOutlet NSTextField* themeLabel;
@property(nonatomic, assign) IBOutlet NSPopUpButton* themePopupButton;
@property(nonatomic, assign) IBOutlet NSToolbar* toolbar;
- (instancetype)initWithDependencies:(std::shared_ptr<application::controllers::preferences_view_controller>)controller
                  translationService:(std::shared_ptr<desktop::app::translation_service>)translationService;
- (IBAction)toolbarItemClicked:(id)sender;
- (IBAction)popupButtonChanged:(id)sender;
@end

namespace application::macos::views
{
	class settings_dialog
	{
	public:
		using dependencies = std::tuple<application::controllers::preferences_view_controller, desktop::app::translation_service>;
		settings_dialog(std::shared_ptr<application::controllers::preferences_view_controller> controller,
		                std::shared_ptr<desktop::app::translation_service> translation_service);
		~settings_dialog();
		settings_dialog(const settings_dialog&) = delete;
		settings_dialog(settings_dialog&&) = delete;
		SettingsDialog* objc() const;
		settings_dialog& operator=(const settings_dialog&) = delete;
		settings_dialog& operator=(settings_dialog&&) = delete;

	private:
		SettingsDialog* m_dialog;
	};
}

#endif
