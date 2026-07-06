#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>
#include <tuple>
#include "services/app_config_service.h"
#include "views/main_window.h"
#include "views/settings_dialog.h"

@class MainWindow;

@interface AppDelegate : NSObject <NSApplicationDelegate>
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithDependencies:(std::shared_ptr<desktop::app::app_info>)appInfo
                     lifetimeService:(std::shared_ptr<desktop::hosting::lifetime_service>)lifetimeService
                  translationService:(std::shared_ptr<desktop::app::translation_service>)translationService
                    appConfigService:(std::shared_ptr<application::services::app_config_service>)appConfigService
                     serviceProvider:(std::shared_ptr<desktop::services::service_provider>)serviceProvider
                          mainWindow:(MainWindow*)mainWindow;
- (IBAction)about:(id)sender;
- (IBAction)checkForUpdates:(id)sender;
- (IBAction)closeFolder:(id)sender;
- (IBAction)openBugReport:(id)sender;
- (IBAction)openDiscussions:(id)sender;
- (IBAction)openFolder:(id)sender;
- (IBAction)openGitHubRepository:(id)sender;
- (IBAction)quit:(id)sender;
- (IBAction)settings:(id)sender;
- (IBAction)viewDebuggingInformation:(id)sender;
@end

namespace application::macos
{
	class app_delegate
	{
	public:
		using dependencies = std::tuple<desktop::app::app_info, desktop::hosting::lifetime_service, desktop::app::translation_service,
		                                application::services::app_config_service, desktop::services::service_provider, application::macos::views::main_window>;
		app_delegate(std::shared_ptr<desktop::app::app_info> app_info, std::shared_ptr<desktop::hosting::lifetime_service> lifetime_service,
		             std::shared_ptr<desktop::app::translation_service> translation_service,
		             std::shared_ptr<application::services::app_config_service> app_config_service,
		             std::shared_ptr<desktop::services::service_provider> service_provider,
		             std::shared_ptr<application::macos::views::main_window> main_window);
		~app_delegate();
		app_delegate(const app_delegate&) = delete;
		app_delegate(app_delegate&&) = delete;
		AppDelegate* objc() const;
		app_delegate& operator=(const app_delegate&) = delete;
		app_delegate& operator=(app_delegate&&) = delete;

	private:
		AppDelegate* m_delegate;
	};
}

#endif
