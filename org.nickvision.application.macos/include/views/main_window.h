#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>
#include <tuple>
#include "controllers/main_window_controller.h"
#include "controls/main_window_split_view.h"
#include "events/folder_changed_event_args.h"
#include "services/events_service.h"

@interface MainWindow : NSWindowController
@property(nonatomic, assign) IBOutlet NSToolbarItem* closeFolderToolbarItem;
@property(nonatomic, assign) IBOutlet NSToolbarItem* openFolderToolbarItem;
- (instancetype)initWithDependencies:(std::shared_ptr<application::controllers::main_window_controller>)controller
                             appInfo:(std::shared_ptr<desktop::app::app_info>)appInfo
                       eventsService:(std::shared_ptr<application::services::events_service>)eventsService
                  translationService:(std::shared_ptr<desktop::app::translation_service>)translationService
                           splitView:(MainWindowSplitView*)splitView;
- (IBAction)checkForUpdates:(id)sender;
- (IBAction)openFolder:(id)sender;
- (IBAction)closeFolder:(id)sender;
- (void)onAppNotificationSent:(const desktop::notifications::app_notification_sent_event_args&)args;
- (void)onFolderChanged:(const application::events::folder_changed_event_args&)args;
- (IBAction)viewDebuggingInformation:(id)sender;
@end

namespace application::macos::views
{
	class main_window
	{
	public:
		using dependencies = std::tuple<application::controllers::main_window_controller, desktop::app::app_info, application::services::events_service,
		                                desktop::app::translation_service, application::macos::controls::main_window_split_view>;
		main_window(std::shared_ptr<application::controllers::main_window_controller> controller, std::shared_ptr<desktop::app::app_info> app_info,
		            std::shared_ptr<application::services::events_service> events_service,
		            std::shared_ptr<desktop::app::translation_service> translation_service,
		            std::shared_ptr<application::macos::controls::main_window_split_view> split_view);
		~main_window();
		main_window(const main_window&) = delete;
		main_window(main_window&&) = delete;
		MainWindow* objc() const;
		main_window& operator=(const main_window&) = delete;
		main_window& operator=(main_window&&) = delete;

	private:
		MainWindow* m_window;
	};
}

#endif
