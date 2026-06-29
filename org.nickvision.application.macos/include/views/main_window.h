#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>
#include "events/folder_changed_event_args.h"

@interface MainWindow : NSWindowController
@property(nonatomic, assign) IBOutlet NSToolbarItem* openFolderToolbarItem;
@property(nonatomic, assign) IBOutlet NSToolbarItem* closeFolderToolbarItem;
- (instancetype)initWithServiceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider;
- (IBAction)checkForUpdates:(id)sender;
- (IBAction)openFolder:(id)sender;
- (IBAction)closeFolder:(id)sender;
- (void)onAppNotificationSent:(const desktop::notifications::app_notification_sent_event_args&)args;
- (void)onFolderChanged:(const application::events::folder_changed_event_args&)args;
- (IBAction)viewDebuggingInformation:(id)sender;
@end

#endif
