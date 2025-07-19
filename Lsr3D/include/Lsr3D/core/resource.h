/**
 * @file resource.h
 * @author dion (hduer_zdy@outlook.com)
 * @brief vital struct
 * @version 0.1
 * @date 2025/7/13
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef RESOURCE_H
#define RESOURCE_H


#include <Eigen/Core>
#include <Lsr3D/core/handle.h>
using lsr3d::Handle;
using lsr3d::VertexHandle;
using lsr3d::TextureCoordHandle;
using lsr3d::TriangleHandle;
using lsr3d::NormalHandle;

namespace lsr3d
{
    using NVec = Eigen::Vector4f; ///< normal vector type
    struct Vertex {
        NVec position; ///< 4D position vector (x, y, z, 1)
        Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : position(x, y, z, 1.0f) {
        } ///< Constructor with optional coordinates
        operator Eigen::Vector4f () const {
            return position; ///< Implicit conversion to Eigen::Vector4f
        }
    };

    using Uv = Eigen::Vector2f; ///< 2D texture coordinate type
    struct TextureCoord {
        Uv uv; ///< 2D texture coordinates (u, v)
        TextureCoord(float u = 0.0f, float v = 0.0f)
            : uv(u, v) {
        } ///< Constructor with optional coordinates
        TextureCoord(Uv uv_)
            : uv(std::move(uv_)) {
        } ///< Constructor with Eigen::Vector2f
        float& u(){
            return uv[0]; ///< Get x coordinate (u)
        } ///< Get u coordinate
        float& v(){
            return uv[1]; ///< Get y coordinate (v)
        } ///< Get v coordinate
    };

    struct Normal {
        NVec normal; ///< 4D normal vector (x, y, z, 1)
        Normal(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : normal(x, y, z, 1.0f) {
        } ///< Constructor with optional coordinates
    };

    struct Color {
        Eigen::Vector4f color; ///< RGBA color vector (r, g, b, a)
        Color(float r = 255.0f, float g = 255.0f, float b = 255.0f, float a = 255.0f)
            : color(r, g, b, a) {
        } ///< Constructor with optional RGBA values
        operator Eigen::Vector4f() const {
            return color; ///< Implicit conversion to Eigen::Vector4f
        }
        float r() const { return color[0]; } ///< Get red component
        float g() const { return color[1]; } ///< Get green component
        float b() const { return color[2]; } ///< Get blue component
        float a() const { return color[3]; } ///< Get alpha component
        Color operator+(const Color& other) const {
            return Color(color[0] + other.color[0],
                color[1] + other.color[1],
                color[2] + other.color[2],
                color[3] + other.color[3]);
        }
        Color operator-(const Color& other) const {
            return Color(color[0] - other.color[0],
                color[1] - other.color[1],
                color[2] - other.color[2],
                color[3] - other.color[3]);
        }
        Color operator*(const Color& other) const {
            return Color(color[0] * other.color[0],
                color[1] * other.color[1],
                color[2] * other.color[2],
                color[3] * other.color[3]);
        }
        Color operator/(float scalar) const {
            if (scalar == 0.0f) {
                throw std::runtime_error("Division by zero in Color division");
            }
            return Color(color[0] / scalar,
                color[1] / scalar,
                color[2] / scalar,
                color[3] / scalar);
        }
    };

    struct Image {
        std::string name; ///< Image name
        int width = 0;    ///< Image width
        int height = 0;   ///< Image height
        int channels = 0;  ///< Image channels
        std::vector<unsigned char> data; ///< Image pixel data (RGBA format)

        /**
         * @brief Check if the image is valid (has data)
         * @return true if image data is not empty
         */
        bool isValid() const {
            return !data.empty();
        }
        Image(std::string _name = "", int _width = 0, int _height = 0, std::vector<unsigned char> _data = {})
            : name(std::move(_name)), width(_width), height(_height), data(std::move(_data)) {
        }
        Image(std::string _name, int _width, int _height, unsigned char* _data, unsigned int _channel = 4)
            : name(std::move(_name)), width(_width), height(_height), data(_data, _data + (_width * _height * _channel)), channels(_channel) {
        }

        Color SampleNearest(Uv uv) const {
            return SampleNearest(uv[0], uv[1]);
        }
        Color SampleNearest(float u, float v) const {
            if (data.empty()) {
                return Color(); // Return transparent color if data is empty
            }
            // Clamp u and v to [0, 1]
            u = std::clamp(u, 0.0f, 1.0f);
            v = std::clamp(v, 0.0f, 1.0f);
            // Calculate pixel coordinates
            int x = static_cast<int>(u * (width - 1));
            int y = static_cast<int>(v * (height - 1));
            // Calculate index in the data array
            return getRGBA(x, y);
        }
        Color SampleLinear(Uv uv) const {
            return SampleLinear(uv[0], uv[1]);
        }
        Color SampleLinear(float u, float v) const {
            if (data.empty()) {
                return Color(); // Return transparent color if data is empty
            }
            // Clamp u and v to [0, 1]
            u = std::clamp(u, 0.0f, 1.0f);
            v = std::clamp(v, 0.0f, 1.0f);
            // Calculate linear interpolation coordinates
            float x = u * (width - 1);
            float y = v * (height - 1);
            // Calculate integer coordinates
            int x0 = static_cast<int>(x);
            int y0 = static_cast<int>(y);
            int x1 = std::min(x0 + 1, width - 1);
            int y1 = std::min(y0 + 1, height - 1);
            // Calculate fractional coordinates
            float fx = x - x0;
            float fy = y - y0;
            // Perform linear interpolation
            Color c00 = getRGBA(x0, y0);
            Color c01 = getRGBA(x0, y1);
            Color c10 = getRGBA(x1, y0);
            Color c11 = getRGBA(x1, y1);
            Color factor = Color(
                (1 - fx) * (1 - fy),    // r
                fx * (1 - fy),          // g
                (1 - fx) * fy,          // b
                fx * fy                 // a
            );
            return c00 * factor +
                c01 * factor +
                c10 * factor +
                c11 * factor;
        }
    private:
        inline unsigned char getR(int x, int y) const {
            return data[(y * width + x) * channels];
        }
        inline unsigned char getG(int x, int y) const {
            return data[(y * width + x) * channels + 1];
        }
        inline unsigned char getB(int x, int y) const {
            return data[(y * width + x) * channels + 2];
        }
        inline unsigned char getA(int x, int y) const {
            if(channels < 4) {
                return 255; // Return opaque alpha if no alpha channel is present
            }
            return data[(y * width + x) * 4 + 3];
        }
        inline Color getRGBA(int x, int y) const {
            if (x < 0 || x >= width || y < 0 || y >= height) {
                return Color(); // Return transparent color if coordinates are out of bounds
            }
            return Color(getR(x, y), getG(x, y), getB(x, y), getA(x, y));
        }

    };
    /**
     * @brief Material information from MTL files
     */
    struct Material {
        std::string name;                           ///< Material name
        float ambient[3] = { 0.2f, 0.2f, 0.2f };    ///< Ambient color (Ka)
        float diffuse[3] = { 0.8f, 0.8f, 0.8f };    ///< Diffuse color (Kd)
        float specular[3] = { 0.0f, 0.0f, 0.0f };   ///< Specular color (Ks)
        float shininess = 0.0f;                     ///< Specular exponent (Ns)
        std::string textureName;                 ///< Diffuse texture map (map_Kd)
        ImageHandle imageHandle;                  ///< Diffuse texture image

        /**
         * @brief Check if this material has a diffuse texture
         * @return true if diffuse texture filename is not empty
         */
        bool hasDiffuseTexture() const {
            return !textureName.empty();
        }

        /**
         * @brief Get the full texture path (can be used with base path)
         * @param basePath Base directory path
         * @return Full path to the texture file
         */
        std::string getFullTexturePath(const std::string& basePath) const {
            if (textureName.empty()) {
                return "";
            }
            if (basePath.empty()) {
                return textureName;
            }
            return basePath + "/" + textureName;
        }
    };
    /**
     * @brief Represents a triangle with vertex, texture, and normal indices
     * @details Each triangle stores indices into the respective arrays for maximum flexibility
     */
    struct Triangle {
        VertexHandle v0, v1, v2;                    ///< Vertex positions
        TextureCoordHandle t0, t1, t2;             ///< Texture coordinates (if available)
        NormalHandle n0, n1, n2;                   ///< Vertex normals (if available)
        bool hasTextures;                     ///< Whether texture coordinates are available
        bool hasNormals;                      ///< Whether vertex normals are available
        std::string materialName;             ///< Name of the material used by this triangle
        const Material* material_;             ///< Pointer to the material (set after materials are loaded)

        /**
         * @brief Constructor
         */
        Triangle() : hasTextures(false), hasNormals(false), material_(nullptr) {}

        /**
         * @brief Check if this triangle has a valid material assigned
         * @return true if material is not null
         */
        bool hasMaterial() const {
            return material_ != nullptr;
        }
    };



}
#endif //RESOURCE_H
