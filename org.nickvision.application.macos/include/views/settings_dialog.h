#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface SettingsDialog : NSWindowController
@property(nonatomic, assign) IBOutlet NSToolbar* toolbar;
@property(nonatomic, assign) IBOutlet NSToolbarItem* generalToolbarItem;
@property(nonatomic, assign) IBOutlet NSToolbarItem* advancedToolbarItem;
- (instancetype)initWithServiceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider;
- (IBAction)toolbarItemClicked:(id)sender;
@end

#endif
