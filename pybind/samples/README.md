# BerialDraw Python Samples

Python port of `samples/*.cpp`, one Python module per C++ file, with the same
class names, function names and structure so the mapping is straightforward:

| C++ file | Python module |
|---|---|
| `dialog.cpp/.hpp` | `dialog.py` |
| `icon_menu_dialog.cpp/.hpp` | `icon_menu_dialog.py` |
| `menu_dialog.cpp/.hpp` | `menu_dialog.py` |
| `samples.cpp` | `samples.py` |
| `main.cpp` | `main.py` |
| `sample_*.cpp` | `sample_*.py` |

## Running the Samples

From the `pybind` directory (so that `pyberialdraw` and the `samples` package
are importable):
```bash
python -m samples.main
```

This launches the icon menu (equivalent of `sample_icon_menu()`), followed by
the text menu (`sample_menu()`), exactly like `samples/main.cpp`.

You can also run a single sample directly:
```python
from pyberialdraw import *
from samples.sample_button import sample_button

UIManager.init(DeviceScreen("Test"), 480, 800, Framebuf.ARGB8888, 2, "../resources")
UIManager.style = "pearl"
UIManager.appearance = "light"

sample_button(None, None)   # signature matches (widget, event) like the C++ callback

UIManager.deinit()
```

## Requirements

- `pyberialdraw` module (must be built and in Python path)
- Resources directory with icons and images (typically `../resources` relative to `pybind`)

## Known limitations (pybind bindings, not these samples)

- Most `Shape` subclasses used on a `Canvas` (`Rect`, `Circle`, `Triangle`,
  `Square`, `Cross`, `Star`, `Line`, `Pie`, `Polygon`, `Text`) are not yet
  bound with `Shape`/`CommonStyle` as a base in `pybind/src/pybind/shape/*.cpp`,
  so `position`, `color`, `size`, `margin`, `angle_` are not yet available on
  them in Python (only `PolyLines` has them today). This affects
  `sample_canvas.py`, `sample_speedometer.py` and the canvas part of
  `sample_theme.py`.
- `Icon.icon_frame_size` and `Grid` "spacing" are not exposed in pyberialdraw.
- `Shape.repeat()` is not exposed; `sample_speedometer.py` reproduces the
  effect with an explicit Python loop.

