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
		static constexpr const char* BORDER_COLOR = "border-color";
		static constexpr const char* BORDER_FOCUS_COLOR = "focus-color";
		static constexpr const char* BORDER_FOCUS_GAP = "focus-gap_";
		static constexpr const char* BORDER_FOCUS_THICKNESS = "focus-thickness";

		// Checkbox style
		static constexpr const char* CHECKBOX_PADDING = "check-padding";
		static constexpr const char* CHECKBOX_COLOR = "check-color";
		static constexpr const char* CHECKBOX_SKETCH = "check-sketch";
		static constexpr const char* CHECKBOX_SIZE = "checkbox-size";

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
		static constexpr const char* EDIT_MAX_LINES = "max-lines";
		static constexpr const char* EDIT_MAX_COLUMNS = "max-columns";
		static constexpr const char* EDIT_SELECT_COLOR = "select-color";
		static constexpr const char* EDIT_CURSOR_COLOR = "cursor-color";
		static constexpr const char* EDIT_PLACEHOLDER_COLOR = "place-holder-color";
		static constexpr const char* EDIT_PASSWORD = "password";
		static constexpr const char* EDIT_PLACEHOLDER = "place-holder";

		// Icon style
		static constexpr const char* ICON_FILENAME = "filename";
		static constexpr const char* ICON_COLOR = "icon-color";
		static constexpr const char* ICON_ZOOM = "zoom_";
		static constexpr const char* ICON_SIZE = "icon-size";
		static constexpr const char* ICON_PADDING = "icon-padding";

		// Line style
		static constexpr const char* LINE_POINT1 = "point1";
		static constexpr const char* LINE_POINT2 = "point2";

		// Pie style
		static constexpr const char* PIE_SWEEP_ANGLE = "sweep-angle_";
		static constexpr const char* PIE_START_ANGLE = "start-angle_";
		static constexpr const char* PIE_ROPE = "rope";

		// Range style (used by both Progress Bar and Slider)
		static constexpr const char* RANGE_VALUE = "value";
		static constexpr const char* RANGE_MIN_VALUE = "min-value";
		static constexpr const char* RANGE_MAX_VALUE = "max-value";
		static constexpr const char* RANGE_STEP_VALUE = "step-value";

		// Progress bar style
		static constexpr const char* PROGRESSBAR_TRACK_COLOR = "track-color";
		static constexpr const char* PROGRESSBAR_FILL_COLOR = "fill-color";
		static constexpr const char* PROGRESSBAR_TRACK_SIZE = "track-size_";
		static constexpr const char* PROGRESSBAR_FILL_SIZE = "fill-size_";

		// Radio style
		static constexpr const char* RADIO_PADDING = "radio-padding";
		static constexpr const char* RADIO_COLOR = "radio-color";
		static constexpr const char* RADIO_SKETCH = "radio-sketch";
		static constexpr const char* RADIO_GROUP = "group";
		static constexpr const char* RADIO_SIZE = "radio-size";

		// Scroll view style
		static constexpr const char* SCROLLVIEW_SIZE = "scroll-size";
		static constexpr const char* SCROLLVIEW_POSITION = "scroll-position";
		static constexpr const char* SCROLLVIEW_DIRECTION = "scroll-direction";
		static constexpr const char* SCROLLVIEW_VIEWPORT_SIZE = "viewport-size";

		// Slider style
		static constexpr const char* SLIDER_TRACK_COLOR = "track-color";
		static constexpr const char* SLIDER_HANDLE_COLOR = "handle-color";
		static constexpr const char* SLIDER_TRACK_SIZE = "track-size_";
		static constexpr const char* SLIDER_HANDLE_SIZE = "handle-size_";

		// Switch style
		static constexpr const char* SWITCH_THUMB_PADDING = "thumb-padding_";
		static constexpr const char* SWITCH_ON_TRACK_COLOR = "on-track-color";
		static constexpr const char* SWITCH_OFF_TRACK_COLOR = "off-track-color";
		static constexpr const char* SWITCH_THUMB_COLOR = "thumb-color";
		static constexpr const char* SWITCH_SIZE = "switch-size";

		// Table view style
		static constexpr const char* TABLEVIEW_GRID_COLOR = "grid-color";
		static constexpr const char* TABLEVIEW_HORIZONTAL_THICKNESS = "horizontal-thickness_";
		static constexpr const char* TABLEVIEW_VERTICAL_THICKNESS = "vertical-thickness_";
		static constexpr const char* TABLEVIEW_GRID_VISIBLE = "grid-visible";
		static constexpr const char* TABLEVIEW_HEADER_VISIBLE = "header-visible";
		static constexpr const char* TABLEVIEW_HEADER_BACKGROUND_COLOR = "header-background-color";
		static constexpr const char* TABLEVIEW_ALTERNATING_ROW_COLOR_1 = "alternating-row-color-1";
		static constexpr const char* TABLEVIEW_ALTERNATING_ROW_COLOR_2 = "alternating-row-color-2";
		static constexpr const char* TABLEVIEW_SCROLL_DIRECTION = "scroll-direction";

		// Text style
		static constexpr const char* TEXT_CONTENT = "text";
		static constexpr const char* TEXT_FONT_FAMILY = "font-familly";
		static constexpr const char* TEXT_COLOR = "text-color";
		static constexpr const char* TEXT_FONT_SIZE = "font-size";
		static constexpr const char* TEXT_ALIGN = "text-align";
		static constexpr const char* TEXT_PADDING = "padding";

		// Widget style
		static constexpr const char* WIDGET_CELL = "cell";
		static constexpr const char* WIDGET_ROW = "row";
		static constexpr const char* WIDGET_COLUMN = "column";
		static constexpr const char* WIDGET_PRESSED = "pressed";
		static constexpr const char* WIDGET_CHECKED = "checked";
		static constexpr const char* WIDGET_FOCUSABLE = "focusable";
		static constexpr const char* WIDGET_SELECTABLE = "selectable";
		static constexpr const char* WIDGET_PRESSABLE = "pressable";
		static constexpr const char* WIDGET_FLOW = "flow";
		static constexpr const char* WIDGET_MIN_SIZE = "min-size";
		static constexpr const char* WIDGET_MAX_SIZE = "max-size";
		static constexpr const char* WIDGET_STYLE = "style";
	};
}
