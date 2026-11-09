#include "samples/samples.hpp"

using namespace berialdraw;

class SampleCombo : public Dialog                                      // Date-selection example built with Combo widgets.
{
public:
	SampleCombo() :
		Dialog()                                                       // Initialize the base dialog.
	{
		Column * content = new Column(m_content);                      // Stack the fields and preview vertically.
		content->margin(16);                                           // Add space around the sample content.

		Row * fields = new Row(content);                               // Place the three date fields on one line.
		fields->margin(8);                                             // Add spacing around the row.

		Column * day_field = new Column(fields);                       // Group the day caption and selector.
		Label * label = new Label(day_field);                          // Create the day caption.
		label->text("Day");                                            // Label the day selector.
		m_day = new Combo(day_field);                                  // Create the day selector.
		m_day->size(90, 40);                                           // Set its display size.
		m_day->editable(false);                                        // Restrict the day to the provided choices.
		m_day->bind(this, &SampleCombo::on_combo_key);                 // Refresh the preview after keyboard selection.

		Column * month_field = new Column(fields);                     // Group the month caption and selector.
		label = new Label(month_field);                                // Create the month caption.
		label->text("Month");                                          // Label the month selector.
		m_month = new Combo(month_field);                              // Create the month selector.
		m_month->size(150, 40);                                        // Allow room for full month names.
		m_month->editable(false);                                      // Restrict the month to the provided choices.
		m_month->bind(this, &SampleCombo::on_combo_key);               // Refresh the preview after keyboard selection.

		Column * year_field = new Column(fields);                      // Group the year caption and selector.
		label = new Label(year_field);                                 // Create the year caption.
		label->text("Year (editable)");                                // Indicate that custom years are accepted.
		m_year = new Combo(year_field);                                // Create the year selector.
		m_year->size(130, 40);                                         // Set its display size.
		m_year->editable(true);                                        // Allow years outside the suggested range.
		m_year->bind(this, &SampleCombo::on_combo_key);                // Refresh the preview as the year is typed.

		for (int day = 1; day <= 31; day++)                            // Populate the fixed day choices.
		{
			String day_text;                                           // Format days with two digits.
			day_text.print("%02d", day);                               // Create values from 01 through 31.
			ListItem * item = m_day->append(day_text);                 // Add the day to the fixed list.
			item->bind(this, &SampleCombo::on_option_click);           // Refresh the preview when it is chosen.
		}

		static const char * months[] =                                 // Fixed month choices.
		{
			"January", "February", "March", "April", "May", "June",
			"July", "August", "September", "October", "November", "December"
		};
		for (const char * month : months)
		{
			ListItem * item = m_month->append(month);                  // Add one month to the list.
			item->bind(this, &SampleCombo::on_option_click);           // Refresh the preview when it is chosen.
		}

		for (int year = 2000; year <= 2026; year++)                    // Populate suggested years; the field remains editable.
		{
			String year_text;                                          // Convert each year to display text.
			year_text.print("%d", year);                               // Format the current year.
			ListItem * item = m_year->append(year_text);               // Add it as a suggested choice.
			item->bind(this, &SampleCombo::on_option_click);           // Refresh the preview when it is chosen.
		}
		m_year->select(26);                                            // Start with 2026 selected.

		m_preview = new Label(content);                                // Display the composed date below the row.
		m_preview->margin(12, 8);                                      // Separate the preview from the selectors.
		update_preview();                                              // Show the initial date immediately.
	}

private:
	void on_option_click(Widget * widget, const ClickEvent & evt)
	{
		(void)widget;                                                  // The selected item is not needed here.
		(void)evt;                                                     // The click details are not needed here.
		update_preview();                                              // Reflect the selection in the preview.
	}

	void on_combo_key(Widget * widget, const KeyEvent & evt)
	{
		(void)evt;                                                     // The key itself does not affect formatting.
		if (widget && widget->focused())
		{
			update_preview();                                          // Reflect keyboard selection or year edits.
		}
	}

	void update_preview()
	{
		m_preview->text("Selected date: %s %s %s",                     // Combine values without validating a calendar date.
			m_day->input().c_str(),
			m_month->input().c_str(),
			m_year->input().c_str());
	}

	Combo * m_day = nullptr;                                           // Fixed day selector.
	Combo * m_month = nullptr;                                         // Fixed month selector.
	Combo * m_year = nullptr;                                          // Editable year selector.
	Label * m_preview = nullptr;                                       // Live date preview.
};

void sample_combo(Widget * widget, const ClickEvent & evt)
{
	(void)widget;                                                      // The menu widget is not needed by this sample.
	(void)evt;                                                         // The menu click details are not needed.
	SampleCombo sample;                                                // Create the date-selection dialog.
	sample.title("Date selection");                                    // Set the dialog title.
	sample.add_back_button("Back");                                    // Add a button to return to the samples menu.
	sample.exec();                                                     // Run the dialog.
}
