#pragma once

namespace berialdraw
{
	/**
	 * @brief Centralized JSON key names used in style serialization/unserialization
	 */
	class StyleNames
	{
	public:
		// Generic keys
		static constexpr const char* KEY_NAME = "name";
		static constexpr const char* KEY_KEY = "key";
		static constexpr const char* KEY_ID = "id";
		static constexpr const char* KEY_TO = "to";
		static constexpr const char* KEY_MAPPING = "mapping";
		static constexpr const char* KEY_MAPPINGS = "mappings";

		// StyleItem
		static constexpr const char* STYLE_STYLES = "styles";
		static constexpr const char* STYLE_NAME = "name";
		static constexpr const char* STYLE_PROPERTIES = "properties";

		// Border style
		static constexpr const char* BORDER_RADIUS = "radius_";
		static constexpr const char* BORDER_THICKNESS = "thickness_";
		static constexpr const char* BORDER_COLOR = "border_color";
		static constexpr const char* BORDER_FOCUS_COLOR = "focus_color";
		static constexpr const char* BORDER_FOCUS_GAP = "focus_gap_";
		static constexpr const char* BORDER_FOCUS_THICKNESS = "focus_thickness";

		// Checkbox style
		static constexpr const char* CHECKBOX_PADDING = "check_padding_";
		static constexpr const char* CHECKBOX_COLOR = "check_color";
		static constexpr const char* CHECKBOX_SKETCH = "check_sketch";
		static constexpr const char* CHECKBOX_SIZE = "checkbox_size";
		static constexpr const char* CHECKBOX_TEXT_PADDING = "text_padding_";

		// Common style
		static constexpr const char* COMMON_COLOR = "color";
		static constexpr const char* COMMON_LIGHT = "light";
		static constexpr const char* COMMON_SATURATION = "saturation";
		static constexpr const char* COMMON_HIDDEN = "hidden";
		static constexpr const char* COMMON_ANGLE = "angle_";
		static constexpr const char* COMMON_POSITION = "position";
		static constexpr const char* COMMON_SIZE = "size";
		static constexpr const char* COMMON_MARGIN = "margin";
		static constexpr const char* COMMON_CENTER = "center";
		static constexpr const char* COMMON_ALIGN = "align";

		// Edit style
		static constexpr const char* EDIT_MAX_LINES = "max_lines";
		static constexpr const char* EDIT_MAX_COLUMNS = "max_columns";
		static constexpr const char* EDIT_SELECT_COLOR = "select_color";
		static constexpr const char* EDIT_CURSOR_COLOR = "cursor_color";
		static constexpr const char* EDIT_PLACEHOLDER_COLOR = "place_holder_color";
		static constexpr const char* EDIT_PASSWORD = "password";
		static constexpr const char* EDIT_PLACEHOLDER = "place_holder";

		// Icon style
		static constexpr const char* ICON_FILENAME = "filename";
		static constexpr const char* ICON_COLOR = "icon_color";
		static constexpr const char* ICON_FRAME_SIZE = "icon_frame_size";
		static constexpr const char* ICON_PADDING = "icon_padding_";
		static constexpr const char* ICON_TEXT_PADDING = "text_padding_";

		// Line style
		static constexpr const char* LINE_POINT1 = "point1";
		static constexpr const char* LINE_POINT2 = "point2";

		// Pie style
		static constexpr const char* PIE_SWEEP_ANGLE = "sweep_angle_";
		static constexpr const char* PIE_START_ANGLE = "start_angle_";
		static constexpr const char* PIE_ROPE = "rope";

		// Range style (used by both Progress Bar and Slider)
		static constexpr const char* RANGE_VALUE = "value";
		static constexpr const char* RANGE_MIN_VALUE = "min_value";
		static constexpr const char* RANGE_MAX_VALUE = "max_value";
		static constexpr const char* RANGE_STEP_VALUE = "step_value";

		// Progress bar style
		static constexpr const char* PROGRESSBAR_TRACK_COLOR = "track_color";
		static constexpr const char* PROGRESSBAR_FILL_COLOR = "fill_color";
		static constexpr const char* PROGRESSBAR_TRACK_SIZE = "track_size_";
		static constexpr const char* PROGRESSBAR_FILL_SIZE = "fill_size_";

		// Radio style
		static constexpr const char* RADIO_PADDING = "radio_padding_";
		static constexpr const char* RADIO_COLOR = "radio_color";
		static constexpr const char* RADIO_SKETCH = "radio_sketch";
		static constexpr const char* RADIO_GROUP = "group";
		static constexpr const char* RADIO_SIZE = "radio_size";
		static constexpr const char* RADIO_TEXT_PADDING = "text_padding_";

		// Scroll view style
		static constexpr const char* SCROLLVIEW_SIZE = "scroll_size";
		static constexpr const char* SCROLLVIEW_POSITION = "scroll_position";
		static constexpr const char* SCROLLVIEW_DIRECTION = "scroll_direction";

		// Slider style
		static constexpr const char* SLIDER_TRACK_COLOR = "track_color";
		static constexpr const char* SLIDER_HANDLE_COLOR = "handle_color";
		static constexpr const char* SLIDER_TRACK_SIZE = "track_size_";
		static constexpr const char* SLIDER_HANDLE_SIZE = "handle_size_";

		// Switch style
		static constexpr const char* SWITCH_THUMB_PADDING = "thumb_padding_";
		static constexpr const char* SWITCH_ON_TRACK_COLOR = "on_track_color";
		static constexpr const char* SWITCH_OFF_TRACK_COLOR = "off_track_color";
		static constexpr const char* SWITCH_THUMB_COLOR = "thumb_color";
		static constexpr const char* SWITCH_SIZE = "switch_size";
		static constexpr const char* SWITCH_TEXT_PADDING = "text_padding_";

		// Table view style
		static constexpr const char* TABLEVIEW_ALTERNATING_ROW_COLOR_1 = "alternating_row_color_1";
		static constexpr const char* TABLEVIEW_ALTERNATING_ROW_COLOR_2 = "alternating_row_color_2";

		// Grid style
		static constexpr const char* GRIDSTYLE_GRID_COLOR = "grid_color";
		static constexpr const char* GRIDSTYLE_GRID_VISIBLE = "grid_visible";
		static constexpr const char* GRIDSTYLE_HORIZONTAL_THICKNESS = "horizontal_thickness_";
		static constexpr const char* GRIDSTYLE_VERTICAL_THICKNESS = "vertical_thickness_";

		// Cell style
		static constexpr const char* CELLSTYLE_ROW_SELECTOR = "row_selector";
		static constexpr const char* CELLSTYLE_COLUMN_SELECTOR = "column_selector";

		// Cells style
		static constexpr const char* CELLSTYLES_CELLS = "cells";

		// Colors style
		static constexpr const char* COLORS_COLORS = "colors";

		// Text style
		static constexpr const char* TEXT_CONTENT = "text";
		static constexpr const char* TEXT_FONT_FAMILY = "font_familly";
		static constexpr const char* TEXT_COLOR = "text_color";
		static constexpr const char* TEXT_FONT_SIZE = "font_size";
		static constexpr const char* TEXT_ALIGN = "text_align";
		static constexpr const char* TEXT_PADDING = "padding";

		// Scrollbar style
		static constexpr const char* SCROLLBAR_VISIBLE     = "scrollbar_visible";
		static constexpr const char* SCROLLBAR_THUMB_COLOR = "scrollbar_thumb_color";
		static constexpr const char* SCROLLBAR_WIDTH       = "scrollbar_width_";
		static constexpr const char* SCROLLBAR_RADIUS      = "scrollbar_radius_";
		static constexpr const char* SCROLLBAR_MARGIN      = "scrollbar_margin_";

		// List style
		static constexpr const char* LISTSTYLE_SELECTION_MODE = "selection_mode";

		// Widget style
		static constexpr const char* WIDGET_CELL = "cell";
		static constexpr const char* WIDGET_ROW = "row";
		static constexpr const char* WIDGET_COLUMN = "column";
		static constexpr const char* WIDGET_PRESSED = "pressed";
		static constexpr const char* WIDGET_CHECKED = "checked";
		static constexpr const char* WIDGET_FOCUSABLE = "focusable";
		static constexpr const char* WIDGET_PARENT_FOCUSABLE = "parent_focusable";
		static constexpr const char* WIDGET_SELECTABLE = "selectable";
		static constexpr const char* WIDGET_FOCUSED = "focused";
		static constexpr const char* WIDGET_SELECTED = "selected";
		static constexpr const char* WIDGET_PRESSABLE = "pressable";
		static constexpr const char* WIDGET_FLOW = "flow";
		static constexpr const char* WIDGET_MIN_SIZE = "min_size";
		static constexpr const char* WIDGET_MAX_SIZE = "max_size";
		static constexpr const char* WIDGET_STYLE = "style";
		static constexpr const char* WIDGET_BORDERS = "borders";
		static constexpr const char* WIDGET_INHERITED_FOCUS_COLOR = "inherited_focus_color";
		static constexpr const char* WIDGET_ENABLED = "enabled";
		
		// List item style
		static constexpr const char* LIST_ITEM_LEADING = "leading";
		static constexpr const char* LIST_ITEM_TRAILING = "trailing";
		static constexpr const char* LIST_ITEM_SELECTED_COLOR = "selected_color";
		static constexpr const char* LIST_ITEM_SELECTED_TEXT_COLOR = "selected_text_color";


		// Timer style
		static constexpr const char* TIMER_INTERVAL = "interval";
		static constexpr const char* TIMER_RECURRING = "recurring";
		static constexpr const char* TIMER_ACTIVE = "active";

		// Extend (used in CommonStyle and WidgetStyle)
		static constexpr const char* EXTEND = "extend";

		// Size Policy (used in WidgetStyle)
		static constexpr const char* SIZE_POLICY = "size_policy";

		// Picture style
		static constexpr const char* PICTURE_FILENAME = "filename";
		static constexpr const char* PICTURE_ALPHA = "alpha";
		static constexpr const char* PICTURE_FIT_MODE = "fit_mode";
	};
}
