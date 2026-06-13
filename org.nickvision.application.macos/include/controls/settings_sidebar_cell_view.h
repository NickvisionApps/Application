#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#import "settings_sidebar_item.h"

@interface SettingsSidebarCellView : NSTableCellView
@property(nonatomic, strong) NSImageView* iconView;
- (void)configureWithItem:(SettingsSidebarItem*)item;
@end

#endif
