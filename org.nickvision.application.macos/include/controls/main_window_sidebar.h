#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>
#include <tuple>

@class MainWindowSidebar;

@protocol MainWindowSidebarDelegate <NSObject>
- (void)mainWindowSidebar:(MainWindowSidebar*)sidebar didSelectItemAtIndex:(NSInteger)index;
@end

@interface MainWindowSidebar : NSViewController <NSTableViewDataSource, NSTableViewDelegate>
@property(nonatomic, assign) id<MainWindowSidebarDelegate> delegate;
@property(nonatomic, assign) IBOutlet NSGlassEffectView* glassEffectView;
@property(nonatomic, assign) IBOutlet NSTableView* tableView;
- (instancetype)initWithDependencies:(std::shared_ptr<desktop::app::translation_service>)translationService;
@end

namespace application::macos::controls
{
	class main_window_sidebar
	{
	public:
		using dependencies = std::tuple<desktop::app::translation_service>;
		main_window_sidebar(std::shared_ptr<desktop::app::translation_service> translation_service);
		~main_window_sidebar();
		main_window_sidebar(const main_window_sidebar&) = delete;
		main_window_sidebar(main_window_sidebar&&) = delete;
		MainWindowSidebar* objc() const;
		main_window_sidebar& operator=(const main_window_sidebar&) = delete;
		main_window_sidebar& operator=(main_window_sidebar&&) = delete;

	private:
		MainWindowSidebar* m_sidebar;
	};
}

#endif