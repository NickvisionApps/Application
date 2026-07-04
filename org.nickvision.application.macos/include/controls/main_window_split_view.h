#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface MainWindowSplitView : NSSplitViewController
- (instancetype)initWithTranslationService:(std::shared_ptr<desktop::app::translation_service>)translationService;
@end

#endif