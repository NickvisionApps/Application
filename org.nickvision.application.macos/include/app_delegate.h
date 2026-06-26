#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface AppDelegate : NSObject <NSApplicationDelegate>
- (instancetype)initWithServiceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider;
- (IBAction)about:(id)sender;
- (IBAction)checkForUpdates:(id)sender;
- (IBAction)closeFolder:(id)sender;
- (IBAction)openBugReport:(id)sender;
- (IBAction)openDiscussions:(id)sender;
- (IBAction)openFolder:(id)sender;
- (IBAction)openGitHubRepository:(id)sender;
- (IBAction)quit:(id)sender;
- (IBAction)settings:(id)sender;
@end

#endif
