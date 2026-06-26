#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface MainWindow : NSWindowController
@property(strong) IBOutlet NSToolbarItem* openFolderToolbarItem;
@property(strong) IBOutlet NSToolbarItem* closeFolderToolbarItem;
- (instancetype)initWithServiceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider;
- (IBAction)checkForUpdates:(id)sender;
- (IBAction)openFolder:(id)sender;
- (IBAction)closeFolder:(id)sender;
@end

#endif
