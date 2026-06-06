#import "views/settings_dialog.h"
#include "controllers/preferences_view_controller.h"

using namespace application::controllers;
using namespace desktop::services;

@interface SettingsSidebarItem : NSObject
@property(nonatomic, copy) NSString* title;
@property(nonatomic, copy) NSString* symbolName;
- (instancetype)initWithTitle:(NSString*)title symbolName:(NSString*)symbolName;
@end

@implementation SettingsSidebarItem

- (instancetype)initWithTitle:(NSString*)title symbolName:(NSString*)symbolName
{
	if (self = [super init])
	{
		_title = [title copy];
		_symbolName = [symbolName copy];
	}
	return self;
}

@end

@interface SettingsSidebarCellView : NSTableCellView
@property(nonatomic, strong) NSImageView* iconView;
- (void)configureWithItem:(SettingsSidebarItem*)item;
@end

@implementation SettingsSidebarCellView

- (instancetype)initWithFrame:(NSRect)frame
{
	if (self = [super initWithFrame:frame])
	{
		_iconView = [[NSImageView alloc] init];
		_iconView.translatesAutoresizingMaskIntoConstraints = NO;
		_iconView.imageScaling = NSImageScaleProportionallyDown;
		[self addSubview:_iconView];
		NSTextField* label{ [NSTextField labelWithString:@""] };
		label.translatesAutoresizingMaskIntoConstraints = NO;
		label.font = [NSFont systemFontOfSize:NSFont.systemFontSize];
		label.lineBreakMode = NSLineBreakByTruncatingTail;
		[self addSubview:label];
		self.textField = label;
		[NSLayoutConstraint activateConstraints:@[
			[_iconView.leadingAnchor constraintEqualToAnchor:self.leadingAnchor constant:8.0],
			[_iconView.centerYAnchor constraintEqualToAnchor:self.centerYAnchor],
			[_iconView.widthAnchor constraintEqualToConstant:16.0],
			[_iconView.heightAnchor constraintEqualToConstant:16.0],
			[label.leadingAnchor constraintEqualToAnchor:_iconView.trailingAnchor constant:8.0],
			[label.trailingAnchor constraintEqualToAnchor:self.trailingAnchor constant:-8.0],
			[label.centerYAnchor constraintEqualToAnchor:self.centerYAnchor],
		]];
	}
	return self;
}

- (void)configureWithItem:(SettingsSidebarItem*)item
{
	self.textField.stringValue = item.title;
	if (@available(macOS 11.0, *))
	{
		NSImageSymbolConfiguration* config{ [NSImageSymbolConfiguration configurationWithPointSize:14.0 weight:NSFontWeightRegular] };
		self.iconView.image = [[NSImage imageWithSystemSymbolName:item.symbolName accessibilityDescription:nil] imageWithSymbolConfiguration:config];
	}
}

@end

@interface SettingsDialog () <NSOutlineViewDataSource, NSOutlineViewDelegate>
@end

@implementation SettingsDialog
{
	std::shared_ptr<preferences_view_controller> m_controller;
	NSSplitViewController* m_splitViewController;
	NSOutlineView* m_outlineView;
	NSArray<SettingsSidebarItem*>* m_sidebarItems;
	NSView* m_sidebarGlassView;
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
		m_sidebarItems = @[
			[[SettingsSidebarItem alloc] initWithTitle:@"General" symbolName:@"gearshape"],
		];
		self.title = @"Settings";
		self.titlebarAppearsTransparent = YES;
		self.releasedWhenClosed = NO;
		self.minSize = NSMakeSize(700, 400);
		self.collectionBehavior = NSWindowCollectionBehaviorManaged | NSWindowCollectionBehaviorFullScreenNone;
		if (@available(macOS 12.0, *))
		{
			self.titlebarSeparatorStyle = NSTitlebarSeparatorStyleNone;
		}
		NSToolbar* toolbar{ [[NSToolbar alloc] initWithIdentifier:@"SettingsToolbar"] };
		self.toolbar = toolbar;
		self.toolbarStyle = NSWindowToolbarStyleUnified;
		m_splitViewController = [[NSSplitViewController alloc] init];
		NSViewController* sidebarVC{ [[NSViewController alloc] init] };
		NSVisualEffectView* sidebarBaseView{ [[NSVisualEffectView alloc] init] };
		sidebarBaseView.material = NSVisualEffectMaterialSidebar;
		sidebarBaseView.blendingMode = NSVisualEffectBlendingModeBehindWindow;
		sidebarBaseView.state = NSVisualEffectStateFollowsWindowActiveState;
		if (@available(macOS 26.0, *))
		{
			NSGlassEffectView* glassView{ [[NSGlassEffectView alloc] init] };
			glassView.translatesAutoresizingMaskIntoConstraints = NO;
			glassView.hidden = YES;
			[sidebarBaseView addSubview:glassView];
			[NSLayoutConstraint activateConstraints:@[
				[glassView.topAnchor constraintEqualToAnchor:sidebarBaseView.topAnchor],
				[glassView.leadingAnchor constraintEqualToAnchor:sidebarBaseView.leadingAnchor],
				[glassView.trailingAnchor constraintEqualToAnchor:sidebarBaseView.trailingAnchor],
				[glassView.bottomAnchor constraintEqualToAnchor:sidebarBaseView.bottomAnchor],
			]];
			m_sidebarGlassView = glassView;
			[[NSNotificationCenter defaultCenter] addObserver:self
			                                         selector:@selector(handleWindowKeyChange:)
			                                             name:NSWindowDidBecomeKeyNotification
			                                           object:self];
			[[NSNotificationCenter defaultCenter] addObserver:self
			                                         selector:@selector(handleWindowKeyChange:)
			                                             name:NSWindowDidResignKeyNotification
			                                           object:self];
		}
		NSSearchField* searchField{ [[NSSearchField alloc] init] };
		searchField.translatesAutoresizingMaskIntoConstraints = NO;
		searchField.placeholderString = @"Search";
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
		if (@available(macOS 11.0, *))
		{
			m_outlineView.style = NSTableViewStyleSourceList;
		}
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
		sidebarItem.minimumThickness = 180.0;
		sidebarItem.maximumThickness = 300.0;
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

- (void)handleWindowKeyChange:(NSNotification*)notification
{
	if (@available(macOS 26.0, *))
	{
		m_sidebarGlassView.hidden = !self.isKeyWindow;
	}
}

- (void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end
