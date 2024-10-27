# Histogram Equalization with Halide

A fast image contrast enhancement tool using Halide framework, supporting both grayscale and color images.

## Requirements
- CMake (>= 3.22)
- C++17
- Halide
- JPEG and PNG libraries

## Quick Start

```bash
# Install dependencies (macOS)
brew install cmake jpeg libpng halide

# Build
mkdir build && cd build
cmake ..
make

# Run
./histogram_equalizer input.jpg output.png
```

## Features
- Processes both grayscale and color images
- Optimized with parallel processing
- Shows processing time and image stats

## Usage Example
```cpp
HistogramEqualizer equalizer;
Halide::Buffer<uint8_t> output = equalizer.process(input);
```

## License
MIT License