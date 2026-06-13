#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>

@interface SettingsSidebarItem : NSObject
@property(nonatomic, copy) NSString* title;
@property(nonatomic, copy) NSString* symbolName;
- (instancetype)initWithTitle:(NSString*)title symbolName:(NSString*)symbolName;
@end

#endif
