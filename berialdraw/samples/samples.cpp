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

	//dialog.bind("Form"       , "icons/view_agenda.icn"       ,sample_forms      );

	dialog.bind("Button"     , "icons/view_agenda.icn"       ,sample_button      );
	dialog.bind("Canvas"     , "icons/draw_abstract.icn"     ,sample_canvas      );
	dialog.bind("Column"     , "icons/table_rows_narrow.icn" ,sample_column      );
	dialog.bind("Edit"       , "icons/text_fields_alt.icn"   ,sample_edit        );
	dialog.bind("Grid"       , "icons/grid_on.icn"           ,sample_grid        );
	dialog.bind("Icon"       , "icons/image.icn"             ,sample_icon        );
	dialog.bind("Keyboard"   , "icons/keyboard.icn"          ,sample_keyboard    );
	dialog.bind("Label"      , "icons/format_size.icn"       ,sample_label       );
	dialog.bind("Pane"       , "icons/featured_video.icn"    ,sample_pane        );
	dialog.bind("ProgressBar", "icons/sliders.icn"           ,sample_progress_bar);
	dialog.bind("Row"        , "icons/calendar_view_week.icn",sample_row         );
	dialog.bind("ScrollView" , "icons/scrollable_header.icn" ,sample_scroll_view );
	dialog.bind("Slider"     , "icons/tune.icn"              ,sample_slider      );
	dialog.bind("Switch"     , "icons/toggle_on.icn"         ,sample_switch      );
	dialog.bind("Window"     , "icons/select_window.icn"     ,sample_window      );
	dialog.bind("Speedometer", "icons/speed.icn"             ,sample_speedometer );
	dialog.bind("Theme"      , "icons/filter_vintage.icn"    ,sample_theme       );
	while (dialog.exec() != "<quit>");
}


/** Sample of menu */
void sample_menu()
{
	UIManager::styles()->style("pearl");            // Select the style pearl
	UIManager::colors()->appearance("light");       // Select the light appearance
	UIManager::colors()->theme(Color::THEME_LIME);  // Select the color theme

	MenuDialog dialog;

	dialog.title("Samples");
	dialog.add_back_button("Back");
	dialog.bind("Button"     ,sample_button      );
	dialog.bind("Canvas"     ,sample_canvas      );
	dialog.bind("Column"     ,sample_column      );
	dialog.bind("Edit"       ,sample_edit        );
	dialog.bind("Grid"       ,sample_grid        );
	dialog.bind("Icon"       ,sample_icon        );
	dialog.bind("Keyboard"   ,sample_keyboard    );
	dialog.bind("Label"      ,sample_label       );
	dialog.bind("Pane"       ,sample_pane        );
	dialog.bind("ProgressBar",sample_progress_bar);
	dialog.bind("Row"        ,sample_row         );
	dialog.bind("ScrollView" ,sample_scroll_view );
	dialog.bind("Slider"     ,sample_slider      );
	dialog.bind("Switch"     ,sample_switch      );
	dialog.bind("Window"     ,sample_window      );
	dialog.bind("Speedometer",sample_speedometer );
	dialog.bind("Theme"      ,sample_theme       );
	while (dialog.exec() != "<quit>");
}
