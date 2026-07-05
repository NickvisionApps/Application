#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@class MainWindowSidebar;

@protocol MainWindowSidebarDelegate <NSObject>
- (void)mainWindowSidebar:(MainWindowSidebar*)sidebar didSelectItemAtIndex:(NSInteger)index;
@end

@interface MainWindowSidebar : NSViewController <NSTableViewDataSource, NSTableViewDelegate>
@property(nonatomic, assign) id<MainWindowSidebarDelegate> delegate;
@property(nonatomic, assign) IBOutlet NSGlassEffectView* glassEffectView;
@property(nonatomic, assign) IBOutlet NSTableView* tableView;
- (instancetype)initWithTranslationService:(std::shared_ptr<desktop::app::translation_service>)translationService;
@end

#endif