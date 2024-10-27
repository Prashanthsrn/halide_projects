#include "histogram_equalisation.h"
#include <chrono>

using namespace Halide;

Buffer<uint8_t> HistogramEqualizer::process(const Buffer<uint8_t>& input) {
    // Determine if the image is grayscale or color
    const int channels = input.channels();
    
    // Create output buffer with same dimensions as input
    Buffer<uint8_t> output(input.width(), input.height(), channels);
    
    auto t1 = std::chrono::high_resolution_clock::now();
    
    // Process each channel independently
    for (int c = 0; c < channels; c++) {
        Var x("x"), y("y");
        
        // Step 1: Compute histogram for this channel
        Func histogram("histogram");
        RDom r(0, input.width(), 0, input.height());
        histogram(x) = cast<float>(0);  // Changed to float
        histogram(clamp(cast<int32_t>(input(r.x, r.y, c)), 0, 255)) += cast<float>(1);  // Cast increment to float
        
        // Step 2: Compute CDF using scan operation
        Func cdf("cdf");
        RDom hist_range(0, 256);
        cdf(x) = cast<float>(0);
        cdf(hist_range) = histogram(hist_range);  // Types now match
        
        // Compute running sum
        Func running_sum("running_sum");
        running_sum(x) = cast<float>(0);
        running_sum(hist_range) = cdf(hist_range);
        running_sum(hist_range) += running_sum(max(hist_range-1, 0));
        
        // Step 3: Normalize CDF
        Func normalized_cdf("normalized_cdf");
        Expr total_pixels = cast<float>(input.width() * input.height());
        normalized_cdf(x) = (running_sum(x) * 255.0f) / total_pixels;
        
        // Step 4: Final equalization for this channel
        Func equalized("equalized");
        equalized(x, y) = cast<uint8_t>(normalized_cdf(clamp(cast<int32_t>(input(x, y, c)), 0, 255)));
        
        // Apply optimization schedule
        histogram.compute_root();
        cdf.compute_root();
        running_sum.compute_root();
        normalized_cdf.compute_root();
        equalized.vectorize(x, 8).parallel(y);
        
        // Realize this channel
        Buffer<uint8_t> channel_output = equalized.realize({input.width(), input.height()});
        
        // Copy the channel data to the final output buffer
        for (int y = 0; y < input.height(); y++) {
            for (int x = 0; x < input.width(); x++) {
                output(x, y, c) = channel_output(x, y);
            }
        }
    }
    
    auto t2 = std::chrono::high_resolution_clock::now();
    processing_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    
    std::cout << "Histogram equalization completed in " << processing_time_ms << "ms\n";
    std::cout << "Processed " << channels << " channels\n";
    
    return output;
}