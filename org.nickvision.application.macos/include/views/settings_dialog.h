#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>

@interface SettingsDialog : NSWindowController
@property(nonatomic, assign) IBOutlet NSTextField* themeLabel;
@property(nonatomic, assign) IBOutlet NSComboBox* themeComboBox;
@property(nonatomic, assign) IBOutlet NSTextField* translationLanguageLabel;
@property(nonatomic, assign) IBOutlet NSComboBox* translationLanguageComboBox;
- (instancetype)initWithServiceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider;
@end

#endif
