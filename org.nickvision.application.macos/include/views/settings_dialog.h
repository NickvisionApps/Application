#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface SettingsDialog : NSWindow
- (instancetype)initWithServiceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider;
- (void)show;
@end

@interface SettingsDialog () <NSOutlineViewDataSource, NSOutlineViewDelegate>
@end

#endif
