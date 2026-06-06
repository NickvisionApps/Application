#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>

@interface MainWindow : NSWindow
- (instancetype)initWithTitle:(NSString*)title;
@end

#endif
