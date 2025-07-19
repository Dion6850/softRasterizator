/**
 * @file ImageLoader.h
 * @author zhywyt (zhywyt@yeah.net)
 * @brief 
 * @version 0.1
 * @date 2025-07-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#include <Lsr3D/core/resource.h>
#include <string>
#include <stb_image.h>

namespace lsr3d {

    /**
     * @brief Load an image from file
     * @param filename Path to the image file
     * @return ImageHandle if successful, invalid handle otherwise
     */
    Image loadImage(const std::string& filename) {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (!data) {
            throw std::runtime_error("Failed to load image: " + filename);
        }
        lsr3d::Image image(filename, width, height, data, channels);
        return image;
    }

} // namespace lsr3d