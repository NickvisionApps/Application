#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface SettingsDialog : NSWindowController
@property(nonatomic, assign) IBOutlet NSToolbar* toolbar;
@property(nonatomic, assign) IBOutlet NSToolbarItem* generalToolbarItem;
@property(nonatomic, assign) IBOutlet NSTabView* tabView;
@property(nonatomic, assign) IBOutlet NSTextField* themeLabel;
@property(nonatomic, assign) IBOutlet NSPopUpButton* themePopupButton;
@property(nonatomic, assign) IBOutlet NSTextField* languageLabel;
@property(nonatomic, assign) IBOutlet NSPopUpButton* languagePopupButton;
- (instancetype)initWithServiceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider;
- (IBAction)toolbarItemClicked:(id)sender;
@end

#endif
