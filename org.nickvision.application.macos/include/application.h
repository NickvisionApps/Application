#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <memory>

namespace application::controllers
{
	class main_window_controller;
}

@interface Application : NSObject <NSApplicationDelegate>
- (instancetype)initWithController:(std::shared_ptr<application::controllers::main_window_controller>)controller;
@end

#endif
