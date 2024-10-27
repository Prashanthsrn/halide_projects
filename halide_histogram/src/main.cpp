#include "histogram_equalisation.h"
#include <iostream>

using namespace Halide::Tools;

int main(int argc, char **argv) {
    try {
        // Check command line arguments
        if (argc < 3) {
            std::cout << "Usage: " << argv[0] << " input.jpg output.png\n";
            return 1;
        }

        // Load the input image
        Halide::Buffer<uint8_t> input = load_image(argv[1]);
        if (!input.data()) {
            std::cerr << "Error: Could not load input image '" << argv[1] << "'\n";
            return 1;
        }
        std::cout << "Processing image: " << input.width() << "x" << input.height() << "\n";

        // Create equalizer and process image
        HistogramEqualizer equalizer;
        
        // Process the image
        Halide::Buffer<uint8_t> output = equalizer.process(input);
        
        // Report processing time
        std::cout << "Processing time: " << equalizer.getProcessingTime() << "ms\n";

        // Save the output image
        save_image(output, argv[2]);
        std::cout << "Successfully saved equalized image to: " << argv[2] << "\n";

    } catch (const Halide::Error& e) {
        std::cerr << "Halide error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}