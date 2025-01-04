#include "samples/samples.hpp"

using namespace berialdraw;

void sample_icon_menu()
{
	//UIManager::notifier()->log();                 // Log all user events if it uncommented
	UIManager::styles()->style("pearl");            // Select the style pearl
	UIManager::colors()->appearance("light");       // Select the light appearance
	UIManager::colors()->theme(Color::THEME_LIME);  // Select the color theme

	IconMenuDialog dialog;

	dialog.title("Samples");
	dialog.add_back_button("Back");

	//dialog.bind("Form"       , "resources/icons/view_agenda.icn"       ,sample_forms      );

	dialog.bind("Button"     , "resources/icons/view_agenda.icn"       ,sample_button      );
	dialog.bind("Canvas"     , "resources/icons/draw_abstract.icn"     ,sample_canvas      );
	dialog.bind("Column"     , "resources/icons/table_rows_narrow.icn" ,sample_column      );
	dialog.bind("Edit"       , "resources/icons/text_fields_alt.icn"   ,sample_edit        );
	dialog.bind("Grid"       , "resources/icons/grid_on.icn"           ,sample_grid        );
	dialog.bind("Icon"       , "resources/icons/image.icn"             ,sample_icon        );
	dialog.bind("Keyboard"   , "resources/icons/keyboard.icn"          ,sample_keyboard    );
	dialog.bind("Label"      , "resources/icons/format_size.icn"       ,sample_label       );
	dialog.bind("Pane"       , "resources/icons/featured_video.icn"    ,sample_pane        );
	dialog.bind("ProgressBar", "resources/icons/sliders.icn"           ,sample_progress_bar);
	dialog.bind("Row"        , "resources/icons/calendar_view_week.icn",sample_row         );
	dialog.bind("ScrollView" , "resources/icons/scrollable_header.icn" ,sample_scroll_view );
	dialog.bind("Slider"     , "resources/icons/tune.icn"              ,sample_slider      );
	dialog.bind("Switch"     , "resources/icons/toggle_on.icn"         ,sample_switch      );
	dialog.bind("Window"     , "resources/icons/select_window.icn"     ,sample_window      );
	dialog.bind("Speedometer", "resources/icons/speed.icn"             ,sample_speedometer );
	dialog.bind("Theme"      , "resources/icons/filter_vintage.icn"    ,sample_theme       );
	while (dialog.exec() != "<quit>");
}
