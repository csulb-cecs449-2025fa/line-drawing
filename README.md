# Line drawing

Please see Homework 1 on Canvas for the full assignment overview.

## Line-drawing contract

The functions in this project draw integer-coordinate pixels. Unless a function's
documentation says otherwise, the following rules apply.

### Coordinates and endpoints

- The coordinate system matches SFML: `x` increases to the right and `y` increases
  downward.
- Both endpoints are included in the output.
- Horizontal, vertical, diagonal, positive-slope, and negative-slope lines are valid.
- The start and end points may be supplied in either order. Reversing the endpoints
  must produce the same set of pixels, although the order in which pixels are written
  may be reversed.
- A line algorithm must not write pixels outside the framebuffer or render target.

### Low- and high-slope lines

For a line with

```text
dx = abs(end.x - start.x)
dy = abs(end.y - start.y)
```

- A **low-slope** line has `dy <= dx`. Its driving coordinate is `x`, so it writes
  one pixel for each integer `x` value from one endpoint to the other.
- A **high-slope** line has `dy > dx`. Its driving coordinate is `y`, so it writes
  one pixel for each integer `y` value from one endpoint to the other.
- A line with `dx == dy` is considered low-slope.

The naive algorithms calculate the ideal real-valued coordinate and select the
nearest integer pixel. When the ideal coordinate is exactly halfway between two
pixels, round away from zero, matching C++ `std::round`.

### Bresenham algorithms

The low- and high-slope Bresenham functions must produce the same pixel choice as
the corresponding naive algorithm, including endpoint handling and all line
directions. If an error value is exactly halfway between two candidate pixels, use
the same tie-breaking choice as the naive algorithm: select the pixel whose driven
coordinate advances first according to the line's sign.

### Gradient lines

`drawGradientLineLow` uses the same low-slope pixel geometry and endpoint rules as
the low-slope Bresenham algorithm. The first endpoint receives the start color and
the second endpoint receives the end color.

For a pixel at interpolation parameter `t`, where `t` is `0` at the start and `1` at
the end, each color channel is calculated independently as

```text
round((1 - t) * startChannel + t * endChannel)
```

The alpha channel is interpolated the same way. Values are clamped to the valid
channel range before being stored.

## CPU framebuffer

`Framebuffer` in `Common/include/framebuffer.h` provides a row-major pixel
buffer for line-drawing algorithms and unit tests. It uses the `Pixel` RGBA type
defined in the same file.

- `setPixel` returns `true` when a pixel is written and `false` when the position
  is out of bounds.
- `getPixel` returns a pixel and throws `std::out_of_range` for an invalid position.
- `clear` fills the entire buffer with one color.
- `data` exposes the read-only row-major storage for inspection or later display.

The line algorithms in `Common/include/lines.h` write to a `Framebuffer` and do not
depend on SFML. The graphical demo uses `drawFramebuffer` from
`LineDrawing/include/sfml_renderer.h`
to upload the CPU framebuffer to an SFML texture and display it as a sprite.

## Build targets

The project defines three targets:

- `common_drawing` contains the framebuffer and line algorithms and has no SFML dependency.
- `Graphics` is the interactive SFML demo in the `LineDrawing` directory.
- `line_tests` contains the GoogleTest unit tests.

The file `CMakePresets.json` provides configure, build, and test presets for:

- Windows x64
- Windows ARM64
- macOS ARM64
- macOS x64

Each platform has Debug and Release variants. 
