#include "pybind/pyberialdraw.hpp"

PYBIND11_MODULE(pyberialdraw, m) {
    m.doc() = "Berialdraw Python bindings";
    
    // Bind font classes first (fundamental classes)
    bind_font(m);
    bind_fonts(m);
    
    // Bind tool classes next (fundamental classes)
    bind_string(m);
    bind_file(m);
    bind_directory(m);
    bind_json(m);
    bind_settings(m);
    
    // Bind vector classes next (fundamental classes)
    bind_point(m);
    bind_size(m);
    bind_area(m);
    bind_margin(m);
    bind_size_policy(m);
    bind_extend(m);
    bind_align(m);
    
    // Bind styles next (base classes)
    bind_style(m);
    bind_styles(m);
    bind_color(m);
    bind_colors(m);
    bind_common_style(m);
    bind_widget_style(m);
    bind_text_style(m);
    bind_border_style(m);
    bind_line_style(m);
    bind_icon_style(m);
    bind_round_style(m);
    bind_slider_style(m);
    bind_switch_style(m);
    bind_progress_bar_style(m);
    bind_edit_style(m);
    bind_pie_style(m);
    bind_scroll_view_style(m);
    
    // Bind framebuf classes
    bind_framebuf(m);
    bind_argb8888(m);
    
    // Bind device classes
    bind_device(m);
    bind_device_screen(m);
    // Bind shape classes  
    bind_shape(m);
    bind_polygon(m);
    bind_marker(m);
    bind_rect(m);
    bind_line(m);
    bind_circle(m);
    bind_triangle(m);
    bind_square(m);
    bind_cross(m);
    bind_text(m);
    bind_pie(m);
    bind_compass(m);
    bind_poly_lines(m);
    bind_poly_points(m);
    bind_sketch(m);
    
    // Event bindings
    bind_event(m);
    bind_click_event(m);
    bind_key_event(m);
    bind_check_event(m);
    bind_select_event(m);
    bind_slide_event(m);
    bind_scroll_event(m);
    bind_focus_event(m);
    bind_touch_event(m);
    bind_event_managers(m);
    bind_notifier(m);
    
    // Bind widget classes (derived classes)
    bind_widget(m);
    bind_button(m);
    bind_label(m);
    bind_window(m);
    bind_canvas(m);
    bind_entry(m);
    bind_edit(m);
    bind_slider(m);
    bind_progress_bar(m);
    bind_row(m);
    bind_column(m);
    bind_switch(m);
    bind_grid(m);
    bind_pane(m);
    bind_scroll_view(m);
    bind_icon(m);
    bind_keyboard(m);
    bind_desktop(m);
    bind_uimanager(m);
}