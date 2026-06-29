#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface MainWindow : NSWindowController
@property(nonatomic, assign) IBOutlet NSToolbarItem* openFolderToolbarItem;
@property(nonatomic, assign) IBOutlet NSToolbarItem* closeFolderToolbarItem;
- (instancetype)initWithServiceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider;
- (IBAction)checkForUpdates:(id)sender;
- (IBAction)openFolder:(id)sender;
- (IBAction)closeFolder:(id)sender;
- (IBAction)viewDebuggingInformation:(id)sender;
@end

#endif
