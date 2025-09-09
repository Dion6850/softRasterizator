/**
 * @file buffer.h
 * @author zhywyt (zhywyt@yeah.net)
 * @brief buffer class for all rendering buffers
 * @version 0.1
 * @date 2025-09-07
 * @note This class without any other dependencies
 * @copyright Copyright (c) 2025
 * 
*/
#pragma once
#include <fstream>
#include <string>
#include <vector>
using std::vector;

/**
 * @brief Base Buffer class define
 * @warning Store the data with the OpenGL style (bottom-left is origin)
 * @tparam T 
 */
template<typename T>
class Buffer {
public:
    Buffer(int width, int height, int channels): width(width), height(height), channels(channels) {
       data = new T[width * height * channels];
       clear();
    };
   ~Buffer() {
       if (data)delete[] data;
   }
   virtual void resize(int newWidth, int newHeight, int newChannels = 3) {
       if(newWidth <= 0 || newHeight <= 0 || newChannels <= 0) {
           throw std::invalid_argument("Width, Height and Channels must be positive integers.");
       }
       else if(newWidth == width && newHeight == height && newChannels == channels) {
           return; // No need to resize if dimensions are the same
       }
       if (data) delete[] data;
       width = newWidth;
       height = newHeight;
       channels = newChannels;
       data = new T[width * height * channels];
       clear();
   }
   virtual void clear() {
       if (data) {
           memset(data, 0, width * height * channels * sizeof(T));
       }
   }
   /**
    * @warning No bounds check
    */
   T& operator[](int index) { return data[index]; }
   /**
    * @brief Get the Offset object
    * @note OpenGL style access
    * return ((height - y - 1)*width + x) * channels + channel;
    */
   inline int getOffset(int x, int y, int channel) const {
        return ((height - y - 1)*width + x) * channels + channel;
   }
   T& operator()(int x, int y, int channel) {
        return this->operator[](getOffset(x, y, channel));
    }
    virtual inline void setData(int x, int y, int channel, const T& value) {
        this->operator()(x, y, channel) = value;
   }

   T* getData() { return data; };
   const T* getData() const{ return data; };
   int getWidth() const { return width; }
   int getHeight() const { return height; }

protected:
   bool checkOutOfBounds(int x, int y, int channel) const {
       return x < 0 || x >= width || y < 0 || y >= height || channel < 0 || channel >= channels;
   }
    int width;
    int height;
    int channels;
    T* data;
};

/**
 * @brief Frame buffer type definition
 * @details Store the color with char format
 */
class FrameBuffer : public Buffer<char> {
public:
    using dataT = char;
    enum class SaveFormat {
        SAVE_FORMAT_PPM
    };
    FrameBuffer(int width, int height, int channels=3) : Buffer<dataT>(width, height, channels) {}
    ~FrameBuffer() {}
    /**
     * @brief save the frame buffer to a file
     * @note Default format is PPM
     * @param filename 
     * @param format
     * @param flip Whether to flip the image vertically when saving
     */
    void saveToFile(const std::string& filename, SaveFormat format = SaveFormat::SAVE_FORMAT_PPM, bool flip = true) const {
        // Implementation for saving the frame buffer to a file
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open file");
        }
        switch (format) {
        case SaveFormat::SAVE_FORMAT_PPM:
            // PPM file header
            file << "P6\n" << width << " " << height << "\n255\n";
            for(int y = 0; y < height; ++y) {
                int row = flip ? (height - y - 1) : y;
                file.write(reinterpret_cast<const char*>(getData() + row * width * channels), width * channels * sizeof(dataT));
            }
            break;
        }
        file.close();
    }
};

