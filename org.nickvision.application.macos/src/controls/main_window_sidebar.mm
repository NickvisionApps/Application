#import "controls/main_window_sidebar.h"

using namespace desktop::app;

namespace application::macos::controls
{
	main_window_sidebar::main_window_sidebar(std::shared_ptr<desktop::app::translation_service> translation_service)
	    : m_sidebar{ [[MainWindowSidebar alloc] initWithDependencies:std::move(translation_service)] }
	{
	}

	main_window_sidebar::~main_window_sidebar()
	{
		[m_sidebar release];
	}

	MainWindowSidebar* main_window_sidebar::objc() const
	{
		return m_sidebar;
	}
}

@implementation MainWindowSidebar
{
	std::shared_ptr<translation_service> m_translation_service;
	NSArray<NSString*>* m_items;
	NSArray<NSString*>* m_icons;
}

- (instancetype)initWithDependencies:(std::shared_ptr<translation_service>)translationService
{
	self = [super initWithNibName:@"main_window_sidebar" bundle:nil];
	if (self)
	{
		m_translation_service = translationService;
		m_items = @[ @(m_translation_service->_("Home")), @(m_translation_service->_("Files")) ];
		m_icons = @[ @"house", @"folder" ];
	}
	return self;
}

- (void)awakeFromNib
{
	[super awakeFromNib];
	self.glassEffectView.contentView = self.tableView.enclosingScrollView;
	self.glassEffectView.style = NSGlassEffectViewStyleRegular;
}

- (void)viewDidLoad
{
	[super viewDidLoad];
	self.tableView.rowSizeStyle = NSTableViewRowSizeStyleMedium;
	[self.tableView reloadData];
	[self.tableView selectRowIndexes:[NSIndexSet indexSetWithIndex:0] byExtendingSelection:NO];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView*)tableView
{
	return m_items.count;
}

- (NSView*)tableView:(NSTableView*)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row
{
	NSTableCellView* cell{ [tableView makeViewWithIdentifier:tableColumn.identifier owner:self] };
	cell.textField.stringValue = m_items[row];
	cell.imageView.image = [NSImage imageWithSystemSymbolName:m_icons[row] accessibilityDescription:m_items[row]];
	return cell;
}

- (void)tableViewSelectionDidChange:(NSNotification*)notification
{
	NSInteger row{ self.tableView.selectedRow };
	if (row < 0)
	{
		return;
	}
	[self.delegate mainWindowSidebar:self didSelectItemAtIndex:row];
}

@end