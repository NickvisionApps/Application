#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>
#include <tuple>
#include "controllers/main_window_controller.h"

@interface MainWindowContent : NSViewController
@property(nonatomic, assign) IBOutlet NSTabView* tabView;
- (instancetype)initWithDependencies:(std::shared_ptr<application::controllers::main_window_controller>)controller
                  translationService:(std::shared_ptr<desktop::app::translation_service>)translationService;
- (void)selectIndex:(NSInteger)index;
@end

namespace application::macos::controls
{
	class main_window_content
	{
	public:
		using dependencies = std::tuple<application::controllers::main_window_controller, desktop::app::translation_service>;
		main_window_content(std::shared_ptr<application::controllers::main_window_controller> controller,
		                    std::shared_ptr<desktop::app::translation_service> translation_service);
		~main_window_content();
		main_window_content(const main_window_content&) = delete;
		main_window_content(main_window_content&&) = delete;
		MainWindowContent* objc() const;
		main_window_content& operator=(const main_window_content&) = delete;
		main_window_content& operator=(main_window_content&&) = delete;

	private:
		MainWindowContent* m_content;
	};
}

#endif
