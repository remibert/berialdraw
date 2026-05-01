# BerialDraw Python Bindings Documentation

## Overview

BerialDraw is a comprehensive C++ GUI framework with Python bindings created using pybind11. This documentation covers all available widgets, their properties, methods, and event handling capabilities.

## Table of Contents

- [Base Classes](#base-classes)
  - [Widget](#widget)
  - [Style Classes](#style-classes)
- [Container Widgets](#container-widgets)
  - [Window](#window)
  - [Pane](#pane)
  - [Column](#column)
  - [Row](#row)
  - [Grid](#grid)
  - [ScrollView](#scrollview)
  - [Canvas](#canvas)
- [Input Widgets](#input-widgets)
  - [Button](#button)
  - [Edit](#edit)
  - [Entry](#entry)
  - [Switch](#switch)
  - [Slider](#slider)
  - [Keyboard](#keyboard)
- [Display Widgets](#display-widgets)
  - [Label](#label)
  - [Icon](#icon)
  - [ProgressBar](#progressbar)
- [System Widgets](#system-widgets)
  - [Desktop](#desktop)
  - [UIManager](#uimanager)
- [Event System](#event-system)
- [Examples](#examples)

---

## Base Classes

### Widget

The base class for all GUI widgets in BerialDraw.

**Inheritance:** `CommonStyle`, `WidgetStyle`

#### Properties (Read-only)
- `parent` → `Widget`: Parent widget reference
- `root` → `Widget`: Root widget reference  
- `classname` → `str`: Widget class name

#### Methods
- `scroll_view()` → `Widget`: Get scroll view container
- `clear()`: Clear widget content
- `clean_all()`: Clean all child widgets
- `search(name)` → `Widget`: Search for child widget by name
- `pressed_color(color, pressed)`: Set pressed state color
- `stated_color(color)`: Set widget state color

#### Events
- `on_touch`: Touch event callback

---

### Style Classes

#### CommonStyle
**Properties:**
- `visible` → `bool`: Widget visibility
- `enable` → `bool`: Widget enabled state
- `focusable` → `bool`: Widget can receive focus
- `position` → `(x, y)`: Widget position as tuple
- `size` → `(width, height)`: Widget size as tuple
- `margin` → `(top, right, bottom, left)`: Margin values (supports CSS-like syntax)
- `name` → `str`: Widget name identifier
- `background_color` → `uint32`: Background color
- `opacity` → `float`: Widget opacity (0.0-1.0)

#### TextStyle
**Properties:**
- `font_familly` → `str`: Font family name
- `font_size` → `(width, height)`: Font size (can be int for square or tuple)
- `padding` → `(top, right, bottom, left)`: Text padding (supports CSS-like syntax)
- `text` → `str`: Text content
- `text_color` → `uint32`: Text color
- `text_align` → `Align`: Text alignment

#### BorderStyle  
**Properties:**
- `radius` → `Dim`: Border radius
- `thickness` → `Dim`: Border thickness
- `border_color` → `uint32`: Border color
- `focus_color` → `uint32`: Focus border color
- `focus_gap` → `Dim`: Gap between border and focus outline
- `focus_thickness` → `Dim`: Focus outline thickness

#### WidgetStyle
**Properties:**
- `shadow_color` → `uint32`: Shadow color
- `shadow_gap` → `Dim`: Shadow offset
- `shadow_radius` → `Dim`: Shadow blur radius

---

## Container Widgets

### Window

Main application window container.

**Inheritance:** `Widget`, `TextStyle`, `BorderStyle`

#### Constructor
```python
Window(parent=None)
```

#### Methods
- `copy(other_window)`: Copy properties from another window

#### Events
- `on_click`: Click event callback
- `on_key_down`: Key press event callback
- `on_focus`: Focus change event callback
- `on_touch`: Touch event callback

---

### Pane

Generic container widget for grouping other widgets.

**Inheritance:** `Widget`, `BorderStyle`

#### Constructor
```python
Pane(parent)
```

#### Methods
- `copy(other_pane)`: Copy properties from another pane

#### Events
- `on_click`: Click event callback
- `on_touch`: Touch event callback

---

### Column

Vertical layout container that arranges children in a column.

**Inheritance:** `Widget`

#### Constructor
```python
Column(parent)
```

#### Methods
- `copy(other_column)`: Copy properties from another column

#### Events
- `on_click`: Click event callback
- `on_touch`: Touch event callback

---

### Row

Horizontal layout container that arranges children in a row.

**Inheritance:** `Widget`

#### Constructor
```python
Row(parent)
```

#### Methods
- `copy(other_row)`: Copy properties from another row

#### Events
- `on_click`: Click event callback
- `on_touch`: Touch event callback

---

### Grid

Grid layout container for organizing widgets in rows and columns.

**Inheritance:** `Widget`

#### Constructor
```python
Grid(parent)
```

#### Methods
- `copy(other_grid)`: Copy properties from another grid

#### Events
- `on_click`: Click event callback
- `on_touch`: Touch event callback

---

### ScrollView

Scrollable container widget with viewport and scrollbars.

**Inheritance:** `Widget`

#### Constructor
```python
ScrollView(parent)
```

#### Properties
- `scroll_size` → `(width, height)`: Total scrollable content size
- `scroll_position` → `(x, y)`: Current scroll position
- `viewport_size` → `(width, height)`: Visible viewport size

#### Methods
- `content_size()`: Get content size
- `marged_size()`: Get size including margins
- `scroll_focus()`: Scroll to focused widget
- `scroll_area()`: Get scrollable area

#### Events
- `on_click`: Click event callback
- `on_scroll`: Scroll event callback
- `on_touch`: Touch event callback

---

### Canvas

Drawing surface widget for custom graphics.

**Inheritance:** `Widget`, `BorderStyle`

#### Constructor
```python
Canvas(parent)
```

#### Methods
- `copy(other_canvas)`: Copy properties from another canvas

#### Events
- `on_click`: Click event callback
- `on_touch`: Touch event callback

---

## Input Widgets

### Button

Clickable button widget.

**Inheritance:** `Widget`, `TextStyle`, `BorderStyle`

#### Constructor
```python
Button(parent)
```

#### Methods
- `copy(other_button)`: Copy properties from another button

#### Events
- `on_click`: Click event callback
- `on_key_down`: Key press event callback
- `on_focus`: Focus change event callback
- `on_touch`: Touch event callback

---

### Edit

Multi-line text editing widget.

**Inheritance:** `Widget`, `TextStyle`, `BorderStyle`, `EditStyle`, `Entry`

#### Constructor
```python
Edit(parent)
```

#### Methods
- `copy(other_edit)`: Copy properties from another edit widget

#### Events
- `on_click`: Click event callback
- `on_key_down`: Key press event callback
- `on_select`: Text selection event callback
- `on_focus`: Focus change event callback
- `on_touch`: Touch event callback

---

### Entry

Single-line text input widget.

**Inheritance:** `Widget`, `TextStyle`, `BorderStyle`, `EditStyle`

#### Factory Methods
```python
Entry.create_entry(parent)          # Basic entry
Entry.create_integer_entry(parent)  # Integer input only
Entry.create_decimal_entry(parent)  # Decimal number input
Entry.create_password_entry(parent) # Password input (masked)
```

#### Events
- `on_click`: Click event callback
- `on_key_down`: Key press event callback
- `on_select`: Text selection event callback
- `on_focus`: Focus change event callback
- `on_touch`: Touch event callback

---

### Switch

Toggle switch widget (on/off state).

**Inheritance:** `Widget`, `BorderStyle`, `SwitchStyle`

#### Constructor
```python
Switch(parent)
```

#### Methods
- `copy(other_switch)`: Copy properties from another switch

#### Events
- `on_click`: Click event callback
- `on_key_down`: Key press event callback
- `on_check`: State change event callback
- `on_focus`: Focus change event callback
- `on_touch`: Touch event callback

---

### Slider

Value selection slider widget.

**Inheritance:** `Widget`, `BorderStyle`, `SliderStyle`

#### Constructor
```python
Slider(parent)
```

#### Methods
- `copy(other_slider)`: Copy properties from another slider

#### Events
- `on_click`: Click event callback
- `on_key_down`: Key press event callback
- `on_slide`: Value change event callback
- `on_focus`: Focus change event callback
- `on_touch`: Touch event callback

---

### Keyboard

Virtual keyboard widget.

**Inheritance:** `Widget`, `TextStyle`, `BorderStyle`

#### Constructor
```python
Keyboard(parent)
```

#### Methods
- `copy(other_keyboard)`: Copy properties from another keyboard

#### Events
- `on_click`: Click event callback
- `on_key_down`: Key press event callback
- `on_focus`: Focus change event callback
- `on_touch`: Touch event callback

---

## Display Widgets

### Label

Static text display widget.

**Inheritance:** `Widget`, `TextStyle`

#### Constructor
```python
Label(parent)
```

#### Methods
- `copy(other_label)`: Copy properties from another label

#### Events
- `on_click`: Click event callback
- `on_touch`: Touch event callback

---

### Icon

Image/icon display widget.

**Inheritance:** `Widget`, `IconStyle`

#### Constructor
```python
Icon(parent)
```

#### Methods
- `copy(other_icon)`: Copy properties from another icon

#### Events
- `on_click`: Click event callback
- `on_key_down`: Key press event callback
- `on_focus`: Focus change event callback
- `on_touch`: Touch event callback

---

### ProgressBar

Progress indication widget.

**Inheritance:** `Widget`, `BorderStyle`, `ProgressBarStyle`

#### Constructor
```python
ProgressBar(parent)
```

#### Methods
- `copy(other_progress_bar)`: Copy properties from another progress bar

#### Events
- `on_click`: Click event callback
- `on_touch`: Touch event callback

---

## System Widgets

### Desktop

System desktop container.

**Inheritance:** `Widget`

#### Constructor
```python
Desktop()
```

#### Methods
- `copy(other_desktop)`: Copy properties from another desktop

#### Events
- `on_click`: Click event callback
- `on_touch`: Touch event callback

---

### UIManager

User interface management system.

**Inheritance:** `Widget`

#### Constructor
```python
UIManager(parent)
```

#### Methods
- `copy(other_ui_manager)`: Copy properties from another UI manager

#### Events
- `on_click`: Click event callback
- `on_touch`: Touch event callback

---

## Event System

BerialDraw uses a modern event system with callback support. Multiple callbacks can be registered for each event using the `+=` operator.

### Event Types

- **ClickEvent**: Mouse/touch click events
- **KeyEvent**: Keyboard input events  
- **CheckEvent**: Switch state change events
- **SelectEvent**: Text selection events
- **SlideEvent**: Slider value change events
- **ScrollEvent**: Scroll position change events
- **FocusEvent**: Focus state change events
- **TouchEvent**: Touch interaction events

### Usage Pattern

```python
def on_button_click(event):
    print(f"Button clicked at {event.position}")

def on_button_click_2(event):
    print("Second callback")

# Register single callback
button.on_click += on_button_click

# Register multiple callbacks
button.on_click += on_button_click_2

# Lambda callbacks
button.on_click += lambda event: print("Lambda callback")
```

### Event Properties

Each event type provides specific information:

- **ClickEvent**: `position`, `button`
- **KeyEvent**: `key`, `scancode`, `modifiers`
- **CheckEvent**: `checked` state
- **SelectEvent**: `selection` info, `status`
- **SlideEvent**: `value`, `position`
- **ScrollEvent**: `position`, `delta`
- **FocusEvent**: `focused` state
- **TouchEvent**: `state`, `position`

---

## Examples

### Basic Window with Button

```python
import berialdraw as bd

# Create main window
window = bd.Window()
window.size = (400, 300)
window.text = "My Application"

# Create button
button = bd.Button(window)
button.text = "Click Me"
button.position = (50, 50)
button.size = (100, 30)

# Add click handler
def button_clicked(event):
    print("Button was clicked!")

button.on_click += button_clicked
```

### Entry Widget with Validation

```python
# Create integer entry
entry = bd.Entry.create_integer_entry(window)
entry.position = (50, 100)
entry.size = (150, 25)

# Handle text selection
def on_text_select(event):
    print(f"Selected text: {event.selection}")

entry.on_select += on_text_select
```

### Slider with Value Display

```python
# Create slider
slider = bd.Slider(window)
slider.position = (50, 150)
slider.size = (200, 20)

# Create label for value display
label = bd.Label(window)
label.position = (260, 150)
label.size = (50, 20)

# Update label when slider changes
def on_slide(event):
    label.text = f"{event.value:.1f}"

slider.on_slide += on_slide
```

### Layout with Column and Row

```python
# Create column layout
column = bd.Column(window)
column.position = (20, 20)
column.size = (200, 250)

# Add buttons to column
for i in range(3):
    button = bd.Button(column)
    button.text = f"Button {i+1}"
    button.size = (180, 30)
    button.on_click += lambda event, i=i: print(f"Button {i+1} clicked")
```

### Switch with State Management

```python
# Create switch
switch = bd.Switch(window)
switch.position = (50, 200)
switch.size = (50, 25)

# Handle state changes
def on_switch_toggle(event):
    if event.checked:
        print("Switch is ON")
        window.background_color = 0x90EE90  # Light green
    else:
        print("Switch is OFF") 
        window.background_color = 0xFFFFFF  # White

switch.on_check += on_switch_toggle
```

---

## Property Syntax

### CSS-like Properties

Many properties support CSS-like syntax for convenience:

```python
# Margin/Padding examples
widget.margin = 10                    # All sides: (10, 10, 10, 10)
widget.margin = (5, 15)              # Vertical, horizontal: (5, 15, 5, 15)  
widget.margin = (1, 2, 3, 4)         # Top, right, bottom, left

# Size examples
widget.size = 100                    # Square: (100, 100)
widget.size = (200, 150)             # Width, height

# Position examples  
widget.position = (50, 75)           # X, Y coordinates

# Font size examples
text_widget.font_size = 12           # Square: (12, 12)
text_widget.font_size = (10, 14)     # Width, height
```

### Color Values

Colors are specified as 32-bit RGBA values:

```python
widget.background_color = 0xFF0000FF  # Red with full alpha
widget.text_color = 0x000000FF       # Black text  
widget.border_color = 0x808080FF     # Gray border
```

---

This documentation covers the complete BerialDraw Python binding API. Each widget inherits properties and methods from its base classes, providing a consistent and powerful interface for GUI development.