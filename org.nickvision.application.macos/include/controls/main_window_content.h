#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface MainWindowContent : NSViewController
@property(nonatomic, assign) IBOutlet NSTabView* tabView;
- (void)selectIndex:(NSInteger)index;
@end

#endif
