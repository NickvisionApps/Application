#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>
#include "controllers/main_window_controller.h"

@interface MainWindowContent : NSViewController
@property(nonatomic, assign) IBOutlet NSTabView* tabView;
- (instancetype)initWithController:(std::shared_ptr<application::controllers::main_window_controller>)controller
                translationService:(std::shared_ptr<desktop::app::translation_service>)translationService;
- (void)selectIndex:(NSInteger)index;
@end

#endif
