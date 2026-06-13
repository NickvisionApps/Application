#import "views/settings_dialog.h"
#include "controllers/preferences_view_controller.h"
#import "controls/settings_sidebar_cell_view.h"
#import "controls/settings_sidebar_item.h"

using namespace application::controllers;
using namespace desktop::app;
using namespace desktop::services;

@implementation SettingsDialog
{
	std::shared_ptr<preferences_view_controller> m_controller;
	std::shared_ptr<translation_service> m_translation_service;
	NSSplitViewController* m_splitViewController;
	NSOutlineView* m_outlineView;
	NSArray<SettingsSidebarItem*>* m_sidebarItems;
}

- (instancetype)initWithServiceProvider:(std::shared_ptr<service_provider>)serviceProvider
{
	NSRect frame{ NSMakeRect(0, 0, 860, 600) };
	NSWindowStyleMask style{ NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable |
		                     NSWindowStyleMaskFullSizeContentView };
	self = [super initWithContentRect:frame styleMask:style backing:NSBackingStoreBuffered defer:NO];
	if (self)
	{
		m_controller = serviceProvider->get_required<preferences_view_controller>();
		m_translation_service = serviceProvider->get_required<translation_service>();
		m_sidebarItems = @[
			[[SettingsSidebarItem alloc] initWithTitle:@(m_translation_service->_("General")) symbolName:@"gear"],
		];
		self.title = @(m_translation_service->_("Settings"));
		self.titlebarAppearsTransparent = YES;
		self.releasedWhenClosed = NO;
		self.minSize = NSMakeSize(700, 400);
		self.collectionBehavior = NSWindowCollectionBehaviorManaged | NSWindowCollectionBehaviorFullScreenNone;
		self.titlebarSeparatorStyle = NSTitlebarSeparatorStyleNone;
		NSToolbar* toolbar{ [[NSToolbar alloc] initWithIdentifier:@"SettingsToolbar"] };
		toolbar.displayMode = NSToolbarDisplayModeIconOnly;
		self.toolbar = toolbar;
		self.toolbarStyle = NSWindowToolbarStyleUnified;
		m_splitViewController = [[NSSplitViewController alloc] init];
		NSViewController* sidebarVC{ [[NSViewController alloc] init] };
		NSVisualEffectView* sidebarBaseView{ [[NSVisualEffectView alloc] init] };
		sidebarBaseView.material = NSVisualEffectMaterialSidebar;
		sidebarBaseView.blendingMode = NSVisualEffectBlendingModeBehindWindow;
		sidebarBaseView.state = NSVisualEffectStateFollowsWindowActiveState;
		NSSearchField* searchField{ [[NSSearchField alloc] init] };
		searchField.translatesAutoresizingMaskIntoConstraints = NO;
		searchField.placeholderString = @(m_translation_service->_("Search"));
		[sidebarBaseView addSubview:searchField];
		NSTableColumn* column{ [[NSTableColumn alloc] initWithIdentifier:@"SidebarColumn"] };
		column.editable = NO;
		m_outlineView = [[NSOutlineView alloc] init];
		[m_outlineView addTableColumn:column];
		m_outlineView.outlineTableColumn = column;
		m_outlineView.headerView = nil;
		m_outlineView.rowSizeStyle = NSTableViewRowSizeStyleCustom;
		m_outlineView.dataSource = self;
		m_outlineView.delegate = self;
		m_outlineView.style = NSTableViewStyleSourceList;
		NSScrollView* scrollView{ [[NSScrollView alloc] init] };
		scrollView.translatesAutoresizingMaskIntoConstraints = NO;
		scrollView.hasVerticalScroller = YES;
		scrollView.autohidesScrollers = YES;
		scrollView.drawsBackground = NO;
		scrollView.automaticallyAdjustsContentInsets = NO;
		scrollView.documentView = m_outlineView;
		[sidebarBaseView addSubview:scrollView];
		[NSLayoutConstraint activateConstraints:@[
			[searchField.topAnchor constraintEqualToAnchor:sidebarBaseView.topAnchor constant:46.0],
			[searchField.leadingAnchor constraintEqualToAnchor:sidebarBaseView.leadingAnchor constant:12.0],
			[searchField.trailingAnchor constraintEqualToAnchor:sidebarBaseView.trailingAnchor constant:-12.0],
			[scrollView.topAnchor constraintEqualToAnchor:searchField.bottomAnchor constant:8.0],
			[scrollView.leadingAnchor constraintEqualToAnchor:sidebarBaseView.leadingAnchor],
			[scrollView.trailingAnchor constraintEqualToAnchor:sidebarBaseView.trailingAnchor],
			[scrollView.bottomAnchor constraintEqualToAnchor:sidebarBaseView.bottomAnchor],
		]];
		sidebarVC.view = sidebarBaseView;
		NSSplitViewItem* sidebarItem{ [NSSplitViewItem sidebarWithViewController:sidebarVC] };
		sidebarItem.minimumThickness = 200.0;
		sidebarItem.maximumThickness = 300.0;
		sidebarItem.canCollapse = NO;
		[m_splitViewController addSplitViewItem:sidebarItem];
		NSViewController* contentVC{ [[NSViewController alloc] init] };
		NSView* contentView{ [[NSView alloc] init] };
		contentVC.view = contentView;
		NSSplitViewItem* contentItem{ [NSSplitViewItem splitViewItemWithViewController:contentVC] };
		[m_splitViewController addSplitViewItem:contentItem];
		self.contentViewController = m_splitViewController;
		[m_outlineView reloadData];
		[m_outlineView selectRowIndexes:[NSIndexSet indexSetWithIndex:0] byExtendingSelection:NO];
		self.initialFirstResponder = m_outlineView;
		[self center];
	}
	return self;
}

- (void)show
{
	[self makeKeyAndOrderFront:nil];
	[NSApp activateIgnoringOtherApps:YES];
}

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
{
	return item == nil ? (NSInteger)m_sidebarItems.count : 0;
}

- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item
{
	return item == nil ? m_sidebarItems[(NSUInteger)index] : nil;
}

- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item
{
	return NO;
}

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item
{
	SettingsSidebarItem* sidebarItem{ (SettingsSidebarItem*)item };
	static NSString* const cellID{ @"SettingsSidebarCell" };
	SettingsSidebarCellView* cell{ [outlineView makeViewWithIdentifier:cellID owner:self] };
	if (!cell)
	{
		cell = [[SettingsSidebarCellView alloc] initWithFrame:NSZeroRect];
		cell.identifier = cellID;
	}
	[cell configureWithItem:sidebarItem];
	return cell;
}

- (CGFloat)outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item
{
	return 28.0;
}

- (BOOL)outlineView:(NSOutlineView*)outlineView shouldSelectItem:(id)item
{
	return YES;
}

- (BOOL)selectionShouldChangeInOutlineView:(NSOutlineView*)outlineView
{
	return outlineView.selectedRow != -1;
}

@end