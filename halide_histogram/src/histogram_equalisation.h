#pragma once

#include <Halide.h>
#include <halide_image_io.h>
#include <cstdint>

class HistogramEqualizer {
public:
    // Process an image and return the equalized result
    Halide::Buffer<uint8_t> process(const Halide::Buffer<uint8_t>& input);
    
    // Get processing time in milliseconds
    double getProcessingTime() const { return processing_time_ms; }

private:
    // Store processing time
    double processing_time_ms{0.0};
};