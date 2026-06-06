# BerialDraw — AI Instructions

## Language & Communication
- User speaks **French** — respond in French
- All code, comments, variable names, commit messages: **English only**
- Do NOT compile or run builds — user handles that
- Concise answers, no unnecessary refactoring

---

## Project Overview
BerialDraw is a **C++20 UI framework** targeting embedded (ESP32), desktop (Win32, macOS/Cocoa, Linux/SDL3) and headless rendering. It renders anti-aliased vector graphics and raster images via FreeType's rasterizer, without GPU acceleration.

### Architecture
```
UIManager (singleton)
├── Desktop (window tree)
│   └── Window → Widget tree (Grid, Row, Column, ScrollView, Button, Label, …)
│       └── Canvas → Shape list (Rect, Circle, Polygon, Text, Image, Sketch, …)
├── Renderer (FreeType rasterizer)
├── Framebuf (ARGB8888 software buffer)
├── Notifier (event dispatch + test scripts)
├── Invalidator (dirty tracking: GEOMETRY / REDRAW)
├── ImageCache (LRU decoded raster cache)
├── Fonts / Styles / Colors / Settings / …
└── Device (platform backend)
```

---

## Build System
- **CMake C++20**, lib statique `berialdraw` + targets `app_unitary_test`, `app_sample`, `pyberialdraw`
- **VS2019** solution in `builds/vs2019/`
- **Linux default**: XCB backend (no external dependencies beyond system libs)
- **Linux optional**: SDL3 backend via `-DUSE_SDL=ON` (requires Conan for `sdl/3.2.14`)
- External libs (FreeType, zlib, minizip, libpng, libjpeg-turbo, pybind11) vendored in `external/`
- Flags: `-DUNICODE -D_UNICODE` (Win32), `LINUX`, `OSX`, `FT2_BUILD_LIBRARY`

---

## Include Convention
Every `.cpp` file starts with:
```cpp
#include "berialdraw_imp.hpp"
using namespace berialdraw;
```
- `berialdraw.hpp` = public includes, strictly ordered: system → tool → framebuf → vector → font → image → styles → event → shape → widget
- `berialdraw_imp.hpp` = adds FreeType, private headers (glyph, renderer, outline, json items, zip…)

**Never include individual headers directly in .cpp files.**

---

## Fixed-Point Arithmetic (CRITICAL)

### Q6 (project standard, like FreeType)
- **1 pixel = 64 units** (`<< 6`), `SCALE = (1LL << 6)`
- `Coord` = `int32_t`, `Dim` = `uint32_t`, both stored in Q6
- Accessor convention:
  - `x()` / `width()` = **pixel value** (getter applies shift + rounding)
  - `x_()` / `width_()` = **raw Q6 value** (64th of a pixel)
  - Setter: `position(10, 20)` = pixels; `position_(640, 1280)` = Q6

### Rounding rules by type
| Type | Getter rounding | Example |
|------|----------------|---------|
| `Point` | Nearest: `(m_x + 32) >> 6` | Position |
| `Size` | Ceil: `(m_width + 63) >> 6` | Dimensions |
| `Margin` | Trunc: `m_top >> 6` | Spacing |

### Angle conversion to FreeType
```cpp
// m_angle is in Q6 degrees (64 = 1°)
int32_t angle_ft = m_angle << 10;  // Convert to FreeType angle units
FT_Vector_Unit(&sincos, angle_ft); // Get cos/sin in Q16.16
```

### Image processing fixed-point
- `resize_bicubic()` and `rotate_bilinear()` use **Q6** (`1LL << 6`) for interpolation weights
- `rotate_bilinear()` uses **Q16.16** (`1LL << 16`) for sub-pixel coordinate mapping (via FreeType's `FT_Vector_Unit`)
- **NEVER use float or double** in image processing code — use `int64_t` with appropriate shifts

---

## Type System

| Type | Underlying | Purpose |
|------|-----------|---------|
| `Coord` | `int32_t` | Signed position (Q6) |
| `Dim` | `uint32_t` | Unsigned dimension (Q6) |
| `Point` | 2×Coord + undefined flags | 2D position |
| `Size` | 2×Dim + undefined flags (`MAX_SIZE=0x7FFFFFFF`) | Width/height |
| `Area` | Point + Size | Bounding rectangle |
| `Margin` | 4×Dim (top/left/bottom/right) | Spacing |

---

## Class Hierarchy

### Style chain
```
Style (abstract: serialize/unserialize/property_name)
├── CommonStyle → margin, position, size, angle, center, color, align, borders, hidden
│   ├── Shape (abstract) → Canvas*/Widget*, repetition, polygon
│   │   ├── Marker : Shape, RoundStyle → radius, thickness
│   │   │   ├── Circle
│   │   │   ├── Cross
│   │   │   ├── Square
│   │   │   └── Triangle
│   │   ├── Rect : Shape, RoundStyle
│   │   ├── Polygon : Shape
│   │   ├── PolyLines : Shape, RoundStyle
│   │   ├── Line : Shape, LineStyle, RoundStyle
│   │   ├── Pie : Shape, PieStyle, RoundStyle
│   │   ├── Text : Shape, TextStyle
│   │   ├── Sketch : Shape (vector .icn/.svg)
│   │   ├── Image : Shape (raster .png/.jpg)
│   │   └── LineChart : Shape, BorderStyle, TextStyle
│   └── Compass : CommonStyle, RoundStyle
│
├── WidgetStyle → row, column, id, min/max_size, extend, pressed, focused
│
├── BorderStyle → border_color, thickness, radius
├── TextStyle → font_size, font_family, text_color, text_align, text
├── RoundStyle → radius, thickness
├── EditStyle → cursor, selection, placeholder
├── CheckboxStyle → checked, check_color
├── RadioStyle → checked, check_color
├── SliderStyle → value, min, max, step
├── SwitchStyle → checked
├── ProgressBarStyle → value, min, max
├── PieStyle → start_angle, end_angle, rope
├── LineStyle → point1, point2
├── IconStyle → icon_color, icon_padding, zoom
├── PictureStyle → filename, fit_mode, alpha
├── GridStyle → spacing
├── ScrollViewStyle → scroll_direction
├── TableViewStyle
├── CellStyle / CellsStyle
└── Keys / Key : CommonStyle, WidgetStyle, TextStyle, BorderStyle
```

### Widget hierarchy (all inherit CommonStyle + WidgetStyle via Widget)
```
Widget : CommonStyle, WidgetStyle
├── Window
├── Canvas
├── Grid
├── Row
├── Column
├── Pane : Widget, BorderStyle
├── Label : Widget, TextStyle
├── Button : Widget, TextStyle, BorderStyle
├── Keyboard : Widget, TextStyle, BorderStyle
├── Edit : Widget, TextStyle, BorderStyle, EditStyle, Entry
├── Icon : Widget, BorderStyle, IconStyle, TextStyle
├── Checkbox : Widget, TextStyle, BorderStyle, CheckboxStyle
├── Radio : Widget, TextStyle, BorderStyle, RadioStyle
├── Switch : Widget, TextStyle, BorderStyle, SwitchStyle
├── Slider : Widget, BorderStyle, SliderStyle
├── ProgressBar : Widget, BorderStyle, ProgressBarStyle
├── Picture : Widget, BorderStyle, PictureStyle
└── ScrollableContent : Widget, ScrollViewStyle
    ├── ScrollView
    └── TableView : ScrollableContent, GridStyle, TableViewStyle
```

### Other hierarchies
```
Event (abstract)
├── TouchEvent, KeyEvent, ClickEvent, CheckEvent
├── FocusEvent, ScrollEvent, SelectEvent, SlideEvent

Device (abstract)
├── DeviceWin32, DeviceCocoa, DeviceSdl, DeviceWayland

Framebuf (abstract)
└── Argb8888

ImageDecoder (abstract)
├── PngDecoder, JpegDecoder

Item (abstract, JSON model)
├── ItemVariant
│   ├── ItemBool, ItemInt, ItemString, ItemNull
└── ItemCollection
    ├── ItemArray, ItemObject

Stream / TextStream → String, File

FileInterface (abstract) → LocalFile, ZipFile
DirectoryInterface (abstract) → LocalDirectory, ZipDirectory
ClipboardProvider (abstract) → ClipboardProviderWin32/Cocoa/SDL/Wayland
Exporter (abstract) → SvgOut
Validator (abstract) → MaskValidator
Json → Settings

Callable (abstract) → MethodCaller<C,E>, FunctionCaller<E>
DataSet : BorderStyle, TextStyle
```

### Shape lifecycle
```cpp
// Shapes auto-register to canvas in constructor:
Polygon * p = new Polygon(canvas);  // Added to canvas->m_shapes
// Canvas owns shapes and deletes them in ~Canvas() / clear()

// Widget tree: parent owns children (linked list m_children/m_next)
Button * b = new Button(&window);  // Added to window's child list
```

---

## Image Pipeline

### Image loading & caching
```
filename → ImageCache::get() → ImageDecoder::create(ext) → PngDecoder/JpegDecoder
         → ImageCacheEntry { pixels, width, height } (LRU cache, max 64 entries)
```

### Image rendering pipeline (Image shape)
```
1. load()         → get decoded pixels from ImageCache
2. rebuild_cache() → resize (bicubic) + rotate (bilinear) → m_cached_pixels
3. paint()        → draw_image() via Renderer (angle=0, rotation pre-applied)
```

### Cache invalidation
- `m_cached_width`, `m_cached_height`, `m_cached_angle` track current cache state
- Cache is rebuilt when area size or angle changes
- `clear_cache()` frees `m_cached_pixels`

---

## Renderer Interface
```cpp
// Vector shape rendering (uses FreeType rasterizer + outline)
void draw(const Shape& shape, const Point& shift);
void draw(position, margin, shift, center, color, angle, Outline&);

// Raster image rendering
void draw_image(position, size, center, margin, angle, pixels, w, h, alpha);
// Note: angle in draw_image only handles 0/90/180/270 via pixel remapping
// Arbitrary angles must be pre-applied via ImageProcessor::rotate_bilinear()

// Primitives
void draw_buffer(x_, y_, buffer, width, height, color);
void fillrect(x, y, width, height, color);
void draw_line(x, y, length, coverage, alpha, color);
```

---

## Test Conventions

### Pattern
```cpp
// Tests are static methods on the class being tested
void ClassName::test1() { /* ... */ }
void ClassName::test()
{
    static bool done = false;
    if (done == false)
    {
        MemoryLeakLog          // Macro: traces file/line/function
        done = true;
        test3();               // Called in REVERSE order
        test2();
        test1();
    }
}
```

### Snapshot testing
```cpp
// Single snapshot
UIManager::desktop()->dispatch("$(ui.tests)/out/testname.svg");

// Animated / event-driven snapshots
String script(
"["
    "{'type':'touch','x':100,'y':200,'state':'down'},"
    "{'type':'touch','x':100,'y':200,'state':'up'},"
    "{'type':'key','key':9208,'state':'down','modifier':'','character':' '},"
    "{'type':'key','key':9208,'state':'up'  ,'modifier':'','character':' '},"
"]");
UIManager::notifier()->play_script(script, "$(ui.tests)/out/testname_%d.svg");
```

### Test orchestration
- All `::test()` methods called from `src/tool/test.cpp` → `test_widget()`, `test_shape()`, etc.
- `ScreenCrc` compares CRC32 of rendered outputs (out/ vs ref/)
- Add new tests to the `#ifdef _DEBUG` block in the class header

---

## Resource Paths
```cpp
$(ui.root-dir)   // Project root
$(ui.fonts)      // resources/fonts
$(ui.styles)     // resources/styles
$(ui.icons)      // resources/icons
$(ui.images)     // resources/images
$(ui.colors)     // resources/colors
$(ui.tests)      // test/
```
Resolved by `Settings::resolve()` — recursive `$(name.subname)` expansion via JSON path navigation (max 3 passes).

---

## UIManager Singleton
```cpp
UIManager::init(device, width, height, framebuf_type, scale, root_dir);
UIManager::desktop()     // Desktop*
UIManager::renderer()    // Renderer*
UIManager::framebuf()    // Framebuf*
UIManager::notifier()    // Notifier*
UIManager::invalidator() // Invalidator*
UIManager::styles()      // Styles*
UIManager::colors()      // Colors*
UIManager::fonts()       // Fonts*
UIManager::device()      // Device*
UIManager::exporter()    // Exporter*
UIManager::screen_crc()  // ScreenCrc*
UIManager::arc_cache()   // ArcCache*
UIManager::settings()    // Settings*
UIManager::clipboard()   // Clipboard*
UIManager::image_cache() // ImageCache*
UIManager::deinit();
```

---

## Memory Management
- **No STL smart pointers in user code** (embedded target)
- Widgets: `new Widget(parent)` — parent owns, cascade delete
- Shapes: `new Shape(canvas)` — canvas owns, cascade delete
- Custom `SharedPtr<T>` for ref-counted resources
- Custom `Vector<T>` wrapper (supports negative indexing)
- `MemoryLeakTracer`: global new/delete tracking in debug, `start()/stop()/show()/break_at(id)`

---

## Serialization
```cpp
// Serialize to JSON
void serialize(JsonIterator & it) override {
    CommonStyle::serialize(it);
    it["color"] = (int)m_color;
    m_position.serialize("position", it);
}

// Unserialize from JSON (with defaults)
void unserialize(JsonIterator & it) override {
    CommonStyle::unserialize(it);
    m_color = (int)(it["color"] | (int)m_color);
    m_position.unserialize("position", it);
}
```

---

## FreeType Integration
- `FT_Matrix` for 2D rotation/transform matrices (Q16.16)
- `FT_Vector_Unit(vec, angle)` → cos/sin in Q16.16
- `FT_Vector_Transform(vec, matrix)` → apply matrix
- `FT_Outline` / `FT_Raster` for vector rendering with anti-aliasing
- `vector_matrix(angle)` → cached rotation matrix from FreeType angle
- `FT_ANGLE_PI2 = 0x10000` (90° in FreeType units)

---

## Coding Style

### Formatting
- Tabs for indentation
- Opening brace on next line (Allman style)
- Member variables: `m_` prefix
- Doxygen comments with `/** */`
- `/// @cond DOXYGEN_IGNORE` to hide internals in docs
- Header guards: `#pragma once`
- Namespace: `berialdraw` (everything is in it)

### Accessors pattern
```cpp
// Getter (const)
const String & filename() const;
uint8_t alpha() const;
ImageFitMode fit_mode() const;

// Setter (same name, parameter)
void filename(const String & s);
void alpha(uint8_t a);
void fit_mode(ImageFitMode mode);
```

### Invalidation pattern
```cpp
void MyClass::property(Type value)
{
    UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY); // or REDRAW
    m_property = value;
    // clear cache if needed
}
```

### Control Flow: Single Exit Point
**Functions with multiple branches must have ONE exit point at the end** (no scattered `return` statements). Use a state variable:
```cpp
void MyFunction(...)
{
    bool should_process = false;
    Result result;
    
    // Validation phase
    if (condition1 && condition2 && condition3)
    {
        // Compute phase
        result = compute(...);
        if (result.valid)
        {
            should_process = true;
        }
    }
    
    // Single exit point
    if (should_process)
    {
        // Action phase
        perform(result);
    }
}
```
**Rationale**: Makes control flow traceable and easier to add logging/cleanup at the single exit point.

### Braces for Single Statements (REQUIRED)
**Always use braces `{}` even for single-line statements**, including if/else, for, while, etc.

❌ **Bad:**
```cpp
if (condition)
    return value;
for (int i = 0; i < n; i++)
    process(i);
```

✅ **Good:**
```cpp
if (condition)
{
    return value;
}
for (int i = 0; i < n; i++)
{
    process(i);
}
```

**Rationale**: Improves readability, prevents bugs when adding statements later, maintains consistency.

### Code Comments & Documentation

**Method declarations in .cpp files:**
- Each method implementation must have a **single-line comment** at the start (matching the one from .hpp)
- Format: `// Method name and brief description`
- Purpose: Visual anchor for finding function starts
- Example:
```cpp
// Resize image with bicubic interpolation
uint32_t* ImageProcessor::resize_bicubic(...)
{
    ...
}
```

**Structural comments in code blocks:**
- Add comments on **important logic blocks**, not secondary lines
- Format: `// Brief description of what block does`
- Examples: "Validate inputs", "Cache lookup", "Apply rotation", "Evict oldest entry"

**Rationale**: Helps code navigation and understanding complex transformations (image processing, cache logic).

### Platform support
| Platform | Device class | Notes |
|----------|-------------|-------|
| Windows | `DeviceWin32` | Native Win32 API, UNICODE |
| macOS | `DeviceCocoa` | CoreGraphics, ObjC++ (.mm) |
| Linux | `DeviceXcb` | XCB (X11), default backend |
| Linux | `DeviceSDL` | SDL3, optional (`-DUSE_SDL=ON`) |
| Wayland | `DeviceWayland` | Headers exist, not default |
| ESP32 | — | Target for Q6 fixed-point design |

---

## Key Files Quick Reference
| What | Where |
|------|-------|
| Main include | `include/berialdraw_imp.hpp` |
| All shapes | `include/shape/*.hpp`, `src/shape/*.cpp` |
| All widgets | `include/widget/*.hpp`, `src/widget/*.cpp` |
| Image processing | `include/image/*.hpp`, `src/image/*.cpp` |
| Style system | `include/styles/*.hpp`, `src/styles/*.cpp` |
| Test files | `test/**/*_test.cpp` (naming: `test/widget/canvas_test.cpp`) |
| Test orchestrator | `src/tool/test.cpp` |
| Tools / utilities | `include/tool/*.hpp`, `src/tool/*.cpp` |

---

## Project Directories

### `test/` — Unit tests
Tests are organized by category, mirroring `src/` structure:
```
test/
├── check.py              # SVG comparison script (out/ vs ref/, converts to PNG via Inkscape on diff)
├── chart/                # line_chart_test.cpp
├── font/                 # font_test.cpp
├── shape/                # circle, compass, cross, line, marker, outline, pie, poly_lines,
│                         # poly_points, polygon, rect, sketch, square, text, triangle
├── styles/               # colors, styles, widget_style
├── tool/                 # directory, file, json, mask_validator, settings, string, text_stream, tools
├── vector/               # linear, region
├── widget/               # button, canvas, checkbox, edit, entry, grid, icon, keyboard, label,
│                         # pane, picture, progress_bar, radio, scroll_view, slider, switch,
│                         # table_view, window
└── snapshot/
    └── ref/              # Reference snapshots for regression testing
        ├── all_crcs.json # CRC32 checksums of all SVG outputs (keyed by $(ui.tests)/out/name.svg)
        └── *.json        # Per-test reference data (e.g. button7_1.json)
```

- Each test file contains static `test1()`, `test2()`, … methods called in **reverse order** from `test()`
- `dispatch()` renders the UI to SVG files in `test/out/`
- `ScreenCrc` computes CRC32 of rendered output and compares against `test/snapshot/ref/all_crcs.json`
- `check.py` does a line-by-line diff of `out/*.svg` vs `ref/*.svg`, and converts mismatches to PNG via Inkscape for visual comparison

### `pybind/` — Python bindings
Python interface to BerialDraw via **pybind11**. Built as `pyberialdraw` shared library.
```
pybind/
├── CMakeLists.txt
├── test.py               # Python test script
├── inc/pybind/
│   ├── pyberialdraw.hpp  # Master header: helper templates (bind_point_property, bind_size_property,
│   │                     # bind_margin_property, bind_dim_property, bind_coord_property)
│   └── event_system.hpp  # Event binding helpers
└── src/
    ├── pyberialdraw.cpp  # PYBIND11_MODULE entry point
    └── pybind/           # Mirrors src/ structure: device/, event/, font/, framebuf/,
                          # image/, shape/, styles/, tool/, vector/, widget/
```

- Each C++ class gets a corresponding `.cpp` binding file in `src/pybind/<category>/`
- Uses helper templates to bind Point/Size/Margin/Coord/Dim properties uniformly
- Exposes all widgets, shapes, styles, events, tools, and UIManager to Python

### `external/` — Vendored external libraries
All third-party dependencies are vendored (no system install required except SDL3 via Conan):
| Library | Purpose |
|---------|---------|
| `freetype/` | Font rasterization + outline rendering (core dependency) |
| `zlib/` | Compression (used by libpng, minizip) |
| `libpng/` | PNG image decoding |
| `libjpeg-turbo/` | JPEG image decoding |
| `minizip/` | ZIP archive reading (icon/resource bundles) |
| `pybind11/` | Python ↔ C++ binding framework |

### `doc/` — Documentation
```
doc/
├── build_doc.py          # Runs `doxygen doxygen.dox`, regenerates html/
├── doxygen.dox           # Doxygen configuration
└── html/                 # Generated API documentation (Doxygen output)
```
