
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
git clone --recurse-submodules https://github.com/remibert/berialdraw.git  -c core.autocrlf=false -c core.eol=native


[C++ Api documentation](doc/html/index.html)

Click on the image to see the source code that produces this screen, example code for each widget :


[<img src="test/snapshot/ref/sample_icon_list_1.svg" width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/icon_menu_dialog.cpp)
[<img src="test/snapshot/ref/sample_buton_1.svg"     width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_button.cpp)
[<img src="test/snapshot/ref/sample_edit_1.svg"      width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_edit.cpp)
[<img src="test/snapshot/ref/sample_icon_1.svg"      width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_icon.cpp)
[<img src="test/snapshot/ref/sample_keyboard_1.svg"  width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_keyboard.cpp)
[<img src="test/snapshot/ref/sample_label_1.svg"     width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_label.cpp)
[<img src="test/snapshot/ref/sample_pane_1.svg"      width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_pane.cpp)
[<img src="test/snapshot/ref/sample_progress_1.svg"  width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_progress_bar.cpp)
[<img src="test/snapshot/ref/sample_slider_1.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_slider.cpp)
[<img src="test/snapshot/ref/sample_switch_2.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_switch.cpp)
[<img src="test/snapshot/ref/sample_window_1.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_window.cpp)


Example drawing

[<img src="test/snapshot/ref/sample_speed_2.svg"     width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_speedometer.cpp)
[<img src="test/snapshot/ref/sample_canvas_1.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_canvas.cpp)

<img src="test/snapshot/ref/polyline0_26.svg"       width="200" height="200" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/rect2.svg"              width="200" height="200" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">


Color theme available

[<img src="test/snapshot/ref/sample_palette_4.svg"     width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_1.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_2.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_3.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_4.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_5.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_6.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_7.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_8.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_9.svg"     width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_10.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_11.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_12.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_13.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_14.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_15.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_16.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_17.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_18.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_19.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_20.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_21.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_22.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_23.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_24.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_25.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_26.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_27.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_28.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_29.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_30.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_31.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_32.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_33.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_34.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_35.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_36.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_37.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_38.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_39.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">
<img src="test/snapshot/ref/sample_palette_40.svg"    width="100" height="100" style="margin-right: 10px;box-shadow: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_theme.cpp)

Example code for managing placement layouts :

[<img src="test/snapshot/ref/sample_row_1.svg"       width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_row.cpp)
[<img src="test/snapshot/ref/sample_scroll_1.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_scrollview.cpp)
[<img src="test/snapshot/ref/sample_grid_1.svg"      width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_grid.cpp)
[<img src="test/snapshot/ref/sample_column_1.svg"    width="200" height="200" style="margin-right: 10px;box-shadow: 10px 10px 15px rgba(0, 0, 0, 0.3);">](samples/sample_column.cpp)


