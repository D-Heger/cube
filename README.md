[![Release][release-badge]][release]
[![Changelog][changelog-badge]][changelog]
[![License][license-badge]][license]

# 3D Cube Visualization Project

A simple, dependency-free 3D cube rendered in your terminal using C. This project is a fun exercise in 3D graphics fundamentals, built from scratch.

![Demo][demo]

You should really see it in action, the gif above makes it look worse than it is!

## About The Project

This is a small project to render a spinning 3D cube in the terminal. The goal was to write it in C with no external dependencies, focusing on the core concepts of 3D graphics:

* **3D Math from Scratch:** All the logic for 3D point rotation and perspective projection is self-contained.
* **Software Z-Buffering:** Correctly handles depth and occlusion for a proper 3D effect.
* **Terminal Rendering:** Draws directly to a character-based frame buffer, which is then printed to the console.
* **Clean & Modular Code:** The logic is separated into modules for math, rendering, input, and the cube object itself.

## Project Structure

```txt
cube/
├── src/                  # Source files
│   ├── main.c            # Main program loop and initialization
│   ├── math3d.c          # 3D mathematics and rotation calculations
│   ├── renderer.c        # Rendering engine and buffer management
│   ├── input.c           # Terminal input handling and controls
│   └── cube.c            # Cube-specific drawing and animation logic
├── include/              # Header files
│   ├── math3d.h          # 3D structures and function declarations
│   ├── renderer.h        # Rendering system declarations
│   ├── input.h           # Input handling declarations
│   ├── cube.h            # Cube logic declarations
│   └── constants.h       # Constants used throughout the project
├── Makefile              # Build system configuration
├── README.md             # This file
├── CHANGELOG.md          # Project change log
├── LICENSE               # Project license file
└── demo.gif              # Animated demonstration of the cube rendering
```

## Getting Started

### Prerequisites

You'll need a C compiler (like `GCC`) and `make`. The code is written in C99 + GNU Extensions and should work on any POSIX-compatible system (Linux, macOS, WSL).

### Installing and Building

```bash
# Clone the repo
git clone https://github.com/D-Heger/cube.git
cd cube

# Compile and build the executable
make

# Running the executable

make run
# Or
./cube
```

The `Makefile` also offers other useful commands:

```bash
# Clean build artifacts
make clean

# Rebuild from scratch
make rebuild
```

### Controls

* The cube will rotate on its own.
* Press the `1` key to exit cleanly.
* If needed, you can always stop the program with `Ctrl+C`.

## Benchmarking

A dedicated benchmark harness is available to measure frame latency and capture memory metrics.

```bash
# Build and run the harness (default 10 second measurement window)
# Default output file is logs/benchmark.csv
make benchmark
```

The harness renders frames without terminal animation, printing a timing summary to `stdout` and appending structured results to `logs/benchmark.csv`. Configure the run duration or log location via environment variables:

* `RUN_SECONDS`: measurement window in seconds (minimum 1.0, defaults to 10.0)
* `CUBE_BENCHMARK_LOG`: path to the CSV output file (defaults to `logs/benchmark.csv`)

Example:

```bash
RUN_SECONDS=5 CUBE_BENCHMARK_LOG=/tmp/cube-bench.csv make benchmark
```

Each CSV row contains: `timestamp,frames,total_ms,avg_ms,p95_ms,max_ms,max_rss_kib`.

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for a detailed list of changes and release history.

## License

This project is open-source and available under the [MIT License](LICENSE).

## Authors

* D. Heger

<!-- Files -->
[changelog]: ./CHANGELOG.md
[license]: ./LICENSE
[demo]: ./demo.gif
<!-- Links -->
[release]: https://github.com/D-Heger/cube/releases
<!-- Badges -->
[changelog-badge]: https://img.shields.io/badge/changelog-1.2.0-blue.svg
[license-badge]: https://img.shields.io/badge/license-MIT-green.svg
[release-badge]: https://img.shields.io/badge/cube-v1.2.0-orange.svg
