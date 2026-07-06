#pragma once

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#include <libdesktop.h>
#include <memory>
#include <tuple>
#include "controls/main_window_content.h"
#include "controls/main_window_sidebar.h"

@interface MainWindowSplitView : NSSplitViewController
- (instancetype)initWithDependencies:(MainWindowSidebar*)sidebar content:(MainWindowContent*)content;
@end

namespace application::macos::controls
{
	class main_window_split_view
	{
	public:
		using dependencies = std::tuple<main_window_sidebar, main_window_content>;
		main_window_split_view(std::shared_ptr<main_window_sidebar> sidebar, std::shared_ptr<main_window_content> content);
		~main_window_split_view();
		main_window_split_view(const main_window_split_view&) = delete;
		main_window_split_view(main_window_split_view&&) = delete;
		MainWindowSplitView* objc() const;
		main_window_split_view& operator=(const main_window_split_view&) = delete;
		main_window_split_view& operator=(main_window_split_view&&) = delete;

	private:
		MainWindowSplitView* m_split_view;
	};
}

#endif