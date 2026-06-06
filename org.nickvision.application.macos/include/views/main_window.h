#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface MainWindow : NSWindow
- (instancetype)initWithTitle:(NSString*)title serviceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider;
@end

#endif
