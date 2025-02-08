
Berialdraw Library is a fully vector-based C++ graphics library, designed to be simple to use, lightweight to run on small processors.

Features:
- Supports anti-aliasing and transparency
- Manages several simple widgets and color themes
- Offers the ability to export screens to SVG
- Draws Bezier curves with rotation and resizing
- Defines some simple widgets like Edit, Label, Button, Layout, ScrollView, Slider...
- Includes a tool to trace memory leaks and find them
- Includes a tool to automate user interface unit tests
- Provides the ability to reskin widgets by json file

**Berialdraw is incomplete and still under development, but it works on Windows, OSX and probably any OS that supports SDL.**

Git clone :
git clone --recurse-submodules https://github.com/remibert/berialdraw.git


[C++ Api documentation](berialdraw/doc/html/index.html)

Click on the image to see the source code that produces this screen, example code for each widget :


[<img src="berialdraw/test/ref/sample_icon_list_1.svg" width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/icon_menu_dialog.cpp)
[<img src="berialdraw/test/ref/sample_buton_1.svg"     width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_button.cpp)
[<img src="berialdraw/test/ref/sample_edit_1.svg"      width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_edit.cpp)
[<img src="berialdraw/test/ref/sample_icon_1.svg"      width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_icon.cpp)
[<img src="berialdraw/test/ref/sample_keyboard_1.svg"  width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_keyboard.cpp)
[<img src="berialdraw/test/ref/sample_label_1.svg"     width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_label.cpp)
[<img src="berialdraw/test/ref/sample_pane_1.svg"      width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_pane.cpp)
[<img src="berialdraw/test/ref/sample_progress_1.svg"  width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_progress_bar.cpp)
[<img src="berialdraw/test/ref/sample_slider_1.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_slider.cpp)
[<img src="berialdraw/test/ref/sample_switch_2.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_switch.cpp)
[<img src="berialdraw/test/ref/sample_window_1.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_window.cpp)


Example drawing

[<img src="berialdraw/test/ref/sample_speed_2.svg"     width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_speedometer.cpp)
[<img src="berialdraw/test/ref/sample_canvas_1.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_canvas.cpp)

<img src="berialdraw/test/ref/polyline0_26.svg"       width="200" height="200" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/rect2.svg"              width="200" height="200" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">


Color theme available

[<img src="berialdraw/test/ref/sample_theme_4.svg"     width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_1.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_2.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_3.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_4.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_5.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_6.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_7.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_8.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_9.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_10.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_11.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_12.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_13.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_14.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_15.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_16.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_17.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_18.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_19.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_20.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_21.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_22.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_23.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_24.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_25.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_26.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_27.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_28.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_29.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_30.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_31.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_32.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_33.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_34.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_35.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_36.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_37.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_38.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_39.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="berialdraw/test/ref/sample_theme_40.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_theme.cpp)

Example code for managing placement layouts :

[<img src="berialdraw/test/ref/sample_row_1.svg"       width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_row.cpp)
[<img src="berialdraw/test/ref/sample_scroll_1.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_scrollview.cpp)
[<img src="berialdraw/test/ref/sample_grid_1.svg"      width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_grid.cpp)
[<img src="berialdraw/test/ref/sample_column_1.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](berialdraw/samples/sample_column.cpp)


