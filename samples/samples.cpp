#include "samples/samples.hpp"

using namespace berialdraw;

// Menu items configuration (shared between icon and plain menus)
struct MenuBinding {
	const char* text;
	const char* icon;
	void (*callback)(Widget*, const ClickEvent&);
};

static const MenuBinding menu_bindings[] = {
	{"Button",      "$(ui.icons)/view_agenda.icn",        sample_button      },
	{"Canvas",      "$(ui.icons)/draw_abstract.icn",      sample_canvas      },
	{"Column",      "$(ui.icons)/table_rows_narrow.icn",  sample_column      },
	{"Edit",        "$(ui.icons)/text_fields_alt.icn",    sample_edit        },
	{"Grid",        "$(ui.icons)/grid_on.icn",            sample_grid        },
	{"Icon",        "$(ui.icons)/image.icn",              sample_icon        },
	{"Keyboard",    "$(ui.icons)/keyboard.icn",           sample_keyboard    },
	{"Label",       "$(ui.icons)/format_size.icn",        sample_label       },
	{"Pane",        "$(ui.icons)/featured_video.icn",     sample_pane        },
	{"ProgressBar", "$(ui.icons)/sliders.icn",            sample_progress_bar},
	{"Row",         "$(ui.icons)/calendar_view_week.icn", sample_row         },
	{"ScrollView",  "$(ui.icons)/scrollable_header.icn",  sample_scroll_view },
	{"Slider",      "$(ui.icons)/tune.icn",               sample_slider      },
	{"Switch",      "$(ui.icons)/toggle_on.icn",          sample_switch      },
	{"Window",      "$(ui.icons)/select_window.icn",      sample_window      },
	{"Speedometer", "$(ui.icons)/speed.icn",              sample_speedometer },
	{"Theme",       "$(ui.icons)/filter_vintage.icn",     sample_palette     },
	{"TableView",   "$(ui.icons)/grid_on.icn",            sample_tableview   },
	{"Picture",     "$(ui.icons)/image.icn",              sample_picture     },
	{"List",        "$(ui.icons)/sort.icn",               sample_list        },
};

template<class DialogType>
void populate_menu(DialogType& dialog) {
	for (const auto& binding : menu_bindings) {
		dialog.bind(binding.text, binding.icon, binding.callback);
	}
}

void sample_icon_menu()
{
	//UIManager::notifier()->log();                 // Log all user events if it uncommented
	UIManager::styles()->style("pearl");            // Select the style pearl
	UIManager::colors()->appearance("light");       // Select the light appearance
	UIManager::colors()->palette(Color::PALETTE_LIME);  // Select the color palette

	IconMenuDialog dialog;

	dialog.title("Samples");
	dialog.add_back_button("Back");

	populate_menu(dialog);
	while (dialog.exec() != "<quit>");
}


/** Sample of menu */
void sample_menu()
{
	UIManager::styles()->style("pearl");            // Select the style pearl
	UIManager::colors()->appearance("light");       // Select the light appearance
	UIManager::colors()->palette(Color::PALETTE_LIME);  // Select the color palette

	MenuDialog dialog;

	dialog.title("Samples");
	dialog.add_back_button("Back");
	populate_menu(dialog);
	while (dialog.exec() != "<quit>");
}
