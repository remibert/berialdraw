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
		static constexpr char KEY_NAME[] = "name";
		static constexpr char KEY_KEY[] = "key";
		static constexpr char KEY_ID[] = "id";
		static constexpr char KEY_TO[] = "to";
		static constexpr char KEY_MAPPING[] = "mapping";
		static constexpr char KEY_MAPPINGS[] = "mappings";

		// StyleItem
		static constexpr char STYLE_STYLES[] = "styles";
		static constexpr char STYLE_NAME[] = "name";
		static constexpr char STYLE_PROPERTIES[] = "properties";

		// Border style
		static constexpr char BORDER_RADIUS[] = "radius";
		static constexpr char BORDER_THICKNESS[] = "thickness";
		static constexpr char BORDER_COLOR[] = "border_color";
		static constexpr char BORDER_FOCUS_COLOR[] = "focus_color";
		static constexpr char BORDER_FOCUS_GAP[] = "focus_gap";
		static constexpr char BORDER_FOCUS_THICKNESS[] = "focus_thickness";

		// Checkbox style
		static constexpr char CHECKBOX_PADDING[] = "check_padding";
		static constexpr char CHECKBOX_COLOR[] = "check_color";
		static constexpr char CHECKBOX_SKETCH[] = "check_sketch";
		static constexpr char CHECKBOX_SIZE[] = "checkbox_size";
		static constexpr char CHECKBOX_TEXT_PADDING[] = "text_padding";

		// Common style
		static constexpr char COMMON_COLOR[] = "color";
		static constexpr char COMMON_LIGHT[] = "light";
		static constexpr char COMMON_SATURATION[] = "saturation";
		static constexpr char COMMON_HIDDEN[] = "hidden";
		static constexpr char COMMON_ANGLE[] = "angle";
		static constexpr char COMMON_POSITION[] = "position";
		static constexpr char COMMON_SIZE[] = "size";
		static constexpr char COMMON_MARGIN[] = "margin";
		static constexpr char COMMON_CENTER[] = "center";
		static constexpr char COMMON_ALIGN[] = "align";

		// Edit style
		static constexpr char EDIT_MAX_LINES[] = "max_lines";
		static constexpr char EDIT_MAX_COLUMNS[] = "max_columns";
		static constexpr char EDIT_SELECT_COLOR[] = "select_color";
		static constexpr char EDIT_CURSOR_COLOR[] = "cursor_color";
		static constexpr char EDIT_PLACEHOLDER_COLOR[] = "place_holder_color";
		static constexpr char EDIT_PASSWORD[] = "password";
		static constexpr char EDIT_PLACEHOLDER[] = "place_holder";

		// Icon style
		static constexpr char ICON_FILENAME[] = "filename";
		static constexpr char ICON_COLOR[] = "icon_color";
		static constexpr char ICON_FRAME_SIZE[] = "icon_frame_size";
		static constexpr char ICON_PADDING[] = "icon_padding";
		static constexpr char ICON_TEXT_PADDING[] = "text_padding";

		// Line style
		static constexpr char LINE_POINT1[] = "point1";
		static constexpr char LINE_POINT2[] = "point2";

		// Pie style
		static constexpr char PIE_SWEEP_ANGLE[] = "sweep_angle";
		static constexpr char PIE_START_ANGLE[] = "start_angle";
		static constexpr char PIE_ROPE[] = "rope";

		// Range style (used by both Progress Bar and Slider)
		static constexpr char RANGE_VALUE[] = "value";
		static constexpr char RANGE_MIN_VALUE[] = "min_value";
		static constexpr char RANGE_MAX_VALUE[] = "max_value";
		static constexpr char RANGE_STEP_VALUE[] = "step_value";

		// Progress bar style
		static constexpr char PROGRESSBAR_TRACK_COLOR[] = "track_color";
		static constexpr char PROGRESSBAR_FILL_COLOR[] = "fill_color";
		static constexpr char PROGRESSBAR_TRACK_SIZE[] = "track_size";
		static constexpr char PROGRESSBAR_FILL_SIZE[] = "fill_size";

		// Radio style
		static constexpr char RADIO_PADDING[] = "radio_padding";
		static constexpr char RADIO_COLOR[] = "radio_color";
		static constexpr char RADIO_SKETCH[] = "radio_sketch";
		static constexpr char RADIO_GROUP[] = "group";
		static constexpr char RADIO_SIZE[] = "radio_size";
		static constexpr char RADIO_TEXT_PADDING[] = "text_padding";

		// Scroll view style
		static constexpr char SCROLLVIEW_SIZE[] = "scroll_size";
		static constexpr char SCROLLVIEW_POSITION[] = "scroll_position";
		static constexpr char SCROLLVIEW_DIRECTION[] = "scroll_direction";

		// Slider style
		static constexpr char SLIDER_TRACK_COLOR[] = "track_color";
		static constexpr char SLIDER_HANDLE_COLOR[] = "handle_color";
		static constexpr char SLIDER_TRACK_SIZE[] = "track_size";
		static constexpr char SLIDER_HANDLE_SIZE[] = "handle_size";

		// Switch style
		static constexpr char SWITCH_THUMB_PADDING[] = "thumb_padding";
		static constexpr char SWITCH_ON_TRACK_COLOR[] = "on_track_color";
		static constexpr char SWITCH_OFF_TRACK_COLOR[] = "off_track_color";
		static constexpr char SWITCH_THUMB_COLOR[] = "thumb_color";
		static constexpr char SWITCH_SIZE[] = "switch_size";
		static constexpr char SWITCH_TEXT_PADDING[] = "text_padding";

		// Table view style
		static constexpr char TABLEVIEW_ALTERNATING_ROW_COLOR_1[] = "alternating_row_color_1";
		static constexpr char TABLEVIEW_ALTERNATING_ROW_COLOR_2[] = "alternating_row_color_2";

		// Grid style
		static constexpr char GRIDSTYLE_GRID_COLOR[] = "grid_color";
		static constexpr char GRIDSTYLE_GRID_VISIBLE[] = "grid_visible";
		static constexpr char GRIDSTYLE_HORIZONTAL_THICKNESS[] = "horizontal_thickness";
		static constexpr char GRIDSTYLE_VERTICAL_THICKNESS[] = "vertical_thickness";

		// Cell style
		static constexpr char CELLSTYLE_ROW_SELECTOR[] = "row_selector";
		static constexpr char CELLSTYLE_COLUMN_SELECTOR[] = "column_selector";

		// Cells style
		static constexpr char CELLSTYLES_CELLS[] = "cells";

		// Colors style
		static constexpr char COLORS_COLORS[] = "colors";

		// Text style
		static constexpr char TEXT_CONTENT[] = "text";
		static constexpr char TEXT_FONT_FAMILY[] = "font_familly";
		static constexpr char TEXT_COLOR[] = "text_color";
		static constexpr char TEXT_FONT_SIZE[] = "font_size";
		static constexpr char TEXT_ALIGN[] = "text_align";
		static constexpr char TEXT_PADDING[] = "padding";

		// Scrollbar style
		static constexpr char SCROLLBAR_VISIBLE    [] = "scrollbar_visible";
		static constexpr char SCROLLBAR_THUMB_COLOR[] = "scrollbar_thumb_color";
		static constexpr char SCROLLBAR_WIDTH[]       = "scrollbar_width";
		static constexpr char SCROLLBAR_RADIUS[]      = "scrollbar_radius";
		static constexpr char SCROLLBAR_MARGIN[]      = "scrollbar_margin";

		// List style
		static constexpr char LISTSTYLE_SELECTION_MODE[] = "selection_mode";

		// Widget style
		static constexpr char WIDGET_CELL[] = "cell";
		static constexpr char WIDGET_ROW[] = "row";
		static constexpr char WIDGET_COLUMN[] = "column";
		static constexpr char WIDGET_PRESSED[] = "pressed";
		static constexpr char WIDGET_CHECKED[] = "checked";
		static constexpr char WIDGET_FOCUSABLE[] = "focusable";
		static constexpr char WIDGET_PARENT_FOCUSABLE[] = "parent_focusable";
		static constexpr char WIDGET_SELECTABLE[] = "selectable";
		static constexpr char WIDGET_FOCUSED[] = "focused";
		static constexpr char WIDGET_SELECTED[] = "selected";
		static constexpr char WIDGET_PRESSABLE[] = "pressable";
		static constexpr char WIDGET_FLOW[] = "flow";
		static constexpr char WIDGET_MIN_SIZE[] = "min_size";
		static constexpr char WIDGET_MAX_SIZE[] = "max_size";
		static constexpr char WIDGET_STYLE[] = "style";
		static constexpr char WIDGET_BORDERS[] = "borders";
		static constexpr char WIDGET_INHERITED_FOCUS_COLOR[] = "inherited_focus_color";
		static constexpr char WIDGET_ENABLED[] = "enabled";
		
		// List item style
		static constexpr char LIST_ITEM_LEADING[] = "leading";
		static constexpr char LIST_ITEM_TRAILING[] = "trailing";
		static constexpr char LIST_ITEM_SELECTED_COLOR[] = "selected_color";
		static constexpr char LIST_ITEM_SELECTED_TEXT_COLOR[] = "selected_text_color";
		static constexpr char LIST_ITEM_BORDER_COLOR[] = "border_color";


		// Timer style
		static constexpr char TIMER_INTERVAL[] = "interval";
		static constexpr char TIMER_RECURRING[] = "recurring";
		static constexpr char TIMER_ACTIVE[] = "active";

		// Extend (used in CommonStyle and WidgetStyle)
		static constexpr char EXTEND[] = "extend";

		// Size Policy (used in WidgetStyle)
		static constexpr char SIZE_POLICY[] = "size_policy";

		// Picture style
		static constexpr char PICTURE_FILENAME[] = "filename";
		static constexpr char PICTURE_ALPHA[] = "alpha";
		static constexpr char PICTURE_FIT_MODE[] = "fit_mode";

		// Sketch
		static constexpr char SKETCH_COLOR[] = "color";
		static constexpr char SKETCH_PATH[] = "path";
		static constexpr char SKETCH_FILENAME[] = "filename";
		static constexpr char SKETCH_RESOLUTION[] = "resolution";
		static constexpr char SKETCH_PATHS[] = "paths";
		static constexpr char SKETCH_ZOOM[] = "zoom";

		// Margin
		static constexpr char MARGIN_TOP[] = "top";
		static constexpr char MARGIN_LEFT[] = "left";
		static constexpr char MARGIN_BOTTOM[] = "bottom";
		static constexpr char MARGIN_RIGHT[] = "right";

		// Point
		static constexpr char POINT_X[] = "x";
		static constexpr char POINT_Y[] = "y";

		// Size
		static constexpr char SIZE_WIDTH[] = "width";
		static constexpr char SIZE_HEIGHT[] = "height";

		// Widget serialization
		static constexpr char WIDGET_TYPE[] = "type";
		static constexpr char WIDGET_CHILDREN[] = "children";
		static constexpr char EDIT_MASK[] = "mask";
	};
}
