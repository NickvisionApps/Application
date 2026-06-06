#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface Application : NSObject <NSApplicationDelegate>
- (instancetype)initWithServiceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider;
@end

#endif
