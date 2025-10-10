/**
 * @file test_threadpool_rasterization.h
 * @author dion (hduer_zdy@outlook.com)
 * @brief Example usage of ThreadPool for rasterization tasks
 * @version 0.1
 * @date 2025-10-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once
#include <iostream>
#include <vector>
#include <utils/utils.h>
#include <utils/ThreadPool.h>

namespace test_threadpool_rasterization {

/**
 * @brief Example of using ThreadPool to parallelize rasterization
 * 
 * This demonstrates how to use the ThreadPool for parallel pixel processing
 * in the rasterization pipeline. It divides the screen into horizontal bands
 * and processes each band in parallel.
 */
void parallel_rasterization_example() {
    // Triangle vertices in screen space
    Eigen::Vector4d v0(256, 100, -2, 1);
    Eigen::Vector4d v1(350, 400, -4, 1);
    Eigen::Vector4d v2(162, 400, -2, 1);

    // Screen dimensions
    const int screen_width = 512;
    const int screen_height = 512;

    // Compute bounding box
    int min_x = std::min({static_cast<int>(v0.x()), static_cast<int>(v1.x()), static_cast<int>(v2.x())});
    int max_x = std::max({static_cast<int>(v0.x()), static_cast<int>(v1.x()), static_cast<int>(v2.x())});
    int min_y = std::min({static_cast<int>(v0.y()), static_cast<int>(v1.y()), static_cast<int>(v2.y())});
    int max_y = std::max({static_cast<int>(v0.y()), static_cast<int>(v1.y()), static_cast<int>(v2.y())});

    // Create thread pool with hardware concurrency
    ThreadPool pool(std::thread::hardware_concurrency());
    
    // Divide work into bands (one per thread)
    int num_bands = pool.size();
    int band_height = (max_y - min_y + num_bands - 1) / num_bands;
    
    std::cout << "Rasterizing triangle with " << num_bands << " threads..." << std::endl;
    std::cout << "Bounding box: [" << min_x << ", " << min_y << "] to [" << max_x << ", " << max_y << "]" << std::endl;
    
    // Store futures for synchronization
    std::vector<std::future<int>> results;
    
    // Submit rasterization tasks for each band
    for (int band = 0; band < num_bands; ++band) {
        int band_min_y = min_y + band * band_height;
        int band_max_y = std::min(band_min_y + band_height - 1, max_y);
        
        results.emplace_back(
            pool.enqueue([=]() {
                int pixels_drawn = 0;
                
                // Process pixels in this band
                for (int i = band_min_y; i <= band_max_y; ++i) {
                    for (int j = min_x; j <= max_x; ++j) {
                        Eigen::Vector2i pixel(j, i);
                        Eigen::Vector2i v0_pixel(static_cast<int>(v0.x()), static_cast<int>(v0.y()));
                        Eigen::Vector2i v1_pixel(static_cast<int>(v1.x()), static_cast<int>(v1.y()));
                        Eigen::Vector2i v2_pixel(static_cast<int>(v2.x()), static_cast<int>(v2.y()));
                        
                        // Check if pixel is inside triangle using cross product
                        if (cross(pixel - v1_pixel, v0_pixel - v1_pixel) < 0 && 
                            cross(pixel - v0_pixel, v2_pixel - v0_pixel) < 0 &&
                            cross(pixel - v2_pixel, v1_pixel - v2_pixel) < 0) {
                            // In actual rendering, you would call SetPixel here
                            // SetPixel(j, i, Eigen::Vector3d(255, 0, 0));
                            pixels_drawn++;
                        }
                    }
                }
                
                return pixels_drawn;
            })
        );
    }
    
    // Wait for all tasks to complete and collect results
    int total_pixels = 0;
    for (auto& result : results) {
        total_pixels += result.get();
    }
    
    std::cout << "Rasterization complete! Drew " << total_pixels << " pixels." << std::endl;
}

/**
 * @brief Task function for the rendering loop
 * 
 * This shows how to integrate the parallel rasterization example
 * into the main rendering pipeline.
 */
void task() {
    parallel_rasterization_example();
}

} // namespace test_threadpool_rasterization

/**
 * @brief Display function example
 * 
 * This is an example of how to use the parallel rasterization in the main loop.
 * Uncomment and include this header in main.cpp to use it.
 */
/*
inline void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    
    test_threadpool_rasterization::task();
    
    glEnd();
    glFlush();
}
*/
