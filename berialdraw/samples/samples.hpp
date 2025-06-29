#ifndef samples_hpp_INCLUDED
#define samples_hpp_INCLUDED

#include "berialdraw.hpp"
#include "dialog.hpp"
#include "menu_dialog.hpp"
#include "icon_menu_dialog.hpp"


/** Sample of icon menu */
void sample_icon_menu();

/** Sample of menu */
void sample_menu();

// Sample function to show Button widget
void sample_button(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Column widget
void sample_column(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Row widget
void sample_row(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Grid widget
void sample_grid(berialdraw::Widget * widget, const berialdraw::ClickEvent & evtevt);

// Sample function to show Canvas widget
void sample_canvas(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Label widget
void sample_label(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Edit widget
void sample_edit(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Icon widget
void sample_icon(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Pane widget
void sample_pane(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show ProgressBar widget
void sample_progress_bar(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Slider widget
void sample_slider(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Keyboard widget
void sample_keyboard(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show ScrollView widget
void sample_scroll_view(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Switch widget
void sample_switch(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show Window widget
void sample_window(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show speedometer on canvas widget
void sample_speedometer(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show all colors themes
void sample_theme(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Sample function to show forms
void sample_forms(berialdraw::Widget * widget, const berialdraw::ClickEvent & evt);

// Simple example of a window displaying a clickable button with the text "hello world",
// the example contains the initialization of the library, as well as the deinitialization
void sample_hello_world();

#endif