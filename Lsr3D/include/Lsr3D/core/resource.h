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
            : position(x, y, z, 1.0f) {} ///< Constructor with optional coordinates
    };

    using Uv = Eigen::Vector2f; ///< 2D texture coordinate type
    struct TextureCoord {
        Uv uv; ///< 2D texture coordinates (u, v)
        TextureCoord(float u = 0.0f, float v = 0.0f)
            : uv(u, v) {} ///< Constructor with optional coordinates
    };

    struct Normal {
        NVec normal; ///< 4D normal vector (x, y, z, 1)
        Normal(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : normal(x, y, z, 1.0f) {} ///< Constructor with optional coordinates
    };

    struct Image {
        std::string name; ///< Image name
        int width = 0;    ///< Image width
        int height = 0;   ///< Image height
        std::vector<unsigned char> data; ///< Image pixel data (RGBA format)

        /**
         * @brief Check if the image is valid (has data)
         * @return true if image data is not empty
         */
        bool isValid() const {
            return !data.empty();
        }
        Image(std::string _name = "", int _width = 0, int _height = 0, std::vector<unsigned char> _data = {})
            : name(std::move(_name)), width(_width), height(_height), data(std::move(_data)) {}
        Image(std::string _name, int _width, int _height, unsigned char* _data, unsigned int _channel = 4)
            : name(std::move(_name)), width(_width), height(_height), data(_data, _data + (_width * _height * _channel)) {}
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
