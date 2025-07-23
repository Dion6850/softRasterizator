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
#include <unordered_map>
#include <map>
using lsr3d::Handle;
using lsr3d::VertexHandle;
using lsr3d::TextureCoordHandle;
using lsr3d::TriangleHandle;
using lsr3d::NormalHandle;

namespace lsr3d
{
    struct Vertex;struct TextureCoord;struct Normal;struct Color;
    struct Triangle;struct Material;struct TriangleData;struct Image;
    using TriangleDatas = std::unordered_map<TriangleHandle, lsr3d::Triangle>;
    using VertexDatas = std::unordered_map<VertexHandle, lsr3d::Vertex>;
    using TextureCoordDatas = std::unordered_map<TextureCoordHandle, lsr3d::TextureCoord>;
    using NormalDatas = std::unordered_map<NormalHandle, lsr3d::Normal>;
    using ImageDatas = std::unordered_map<ImageHandle, lsr3d::Image>;
    using MaterialDatas = std::unordered_map<MaterialHandle, lsr3d::Material>;
    using ColorDatas = std::unordered_map<ColorHandle, lsr3d::Color>;
    using NVec = Eigen::Vector4f; ///< normal vector type
    struct Vertex {
        NVec position; ///< 4D position vector (x, y, z, 1)
        Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : position(x, y, z, 1.0f) {
        } ///< Constructor with optional coordinates
        Vertex(NVec pos)
            : position(std::move(pos)) {
        } ///< Constructor with Eigen::Vector4f
        operator Eigen::Vector4f () const {
            return position; ///< Implicit conversion to Eigen::Vector4f
        }
        Vertex operator* (float scalar) const {
            return Vertex(position * scalar);
        }
        Vertex operator+ (const Vertex& other) const {
            return Vertex(position + other.position);
        }
        Vertex operator- (const Vertex& other) const {
            return Vertex(position - other.position);
        }
        Vertex operator/ (float scalar) const {
            if (scalar == 0.0f) {
                throw std::runtime_error("Division by zero in Vertex division");
            }
            return Vertex(position / scalar);
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
        float u() const {
            return uv[0]; ///< Get x coordinate (u) as const
        } ///< Get u coordinate as const
        float v() const {
            return uv[1]; ///< Get y coordinate (v) as const
        } ///< Get v coordinate as const
        TextureCoord operator* (float scalar) const {
            return TextureCoord(uv * scalar);
        } ///< Scale texture coordinates by a scalar
        TextureCoord operator+ (const TextureCoord& other) const {
            return TextureCoord(uv + other.uv);
        } ///< Add two texture coordinates
        TextureCoord operator- (const TextureCoord& other) const {
            return TextureCoord(uv - other.uv);
        } ///< Subtract two texture coordinates
        TextureCoord operator/ (float scalar) const {
            if (scalar == 0.0f) {
                throw std::runtime_error("Division by zero in TextureCoord division");
            }
            return TextureCoord(uv / scalar);
        } ///< Divide texture coordinates by a scalar
    };

    struct Normal {
        NVec normal; ///< 4D normal vector (x, y, z, 1)
        Normal(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : normal(x, y, z, 1.0f) {
        } ///< Constructor with optional coordinates
        Normal(NVec normal_)
            : normal(std::move(normal_)) {
        } ///< Constructor with Eigen::Vector4f
        operator Eigen::Vector4f() const {
            return normal; ///< Implicit conversion to Eigen::Vector4f
        }
        Normal operator*(float scalar) const {
            return Normal(normal * scalar);
        }
        Normal operator+(const Normal& other) const {
            return Normal(normal + other.normal);
        }
        Normal operator-(const Normal& other) const {
            return Normal(normal - other.normal);
        }        Normal operator/(float scalar) const {
            if (scalar == 0.0f) {               throw std::runtime_error("Division by zero in Normal division");
            }
            return Normal(normal / scalar);
        }
        Normal normalized() const {
            float length = normal.norm();
            if (length > 0) {
                return Normal(normal / length);
            }
            return Normal();
        }
    };

    struct Color {
        Eigen::Vector4f color; ///< RGBA color vector (r, g, b, a)
        Color(float r = 255.0f, float g = 255.0f, float b = 255.0f, float a = 255.0f)
            : color(r, g, b, a) {
        } ///< Constructor with optional RGBA values
        Color(Eigen::Vector4f color_)
            : color(std::move(color_)) {
        } ///< Constructor with Eigen::Vector4f
        operator Eigen::Vector4f() const {
            return color; ///< Implicit conversion to Eigen::Vector4f
        }
        float r() const { return color[0]; } ///< Get red component
        float g() const { return color[1]; } ///< Get green component
        float b() const { return color[2]; } ///< Get blue component
        float a() const { return color[3]; } ///< Get alpha component
        Color operator+(const Color& other) const {
            return Color(color + other.color);
        }
        Color operator-(const Color& other) const {
            return Color(color - other.color);
        }
        Color operator*(const Color& other) const {
            return Color(color.cwiseProduct(other.color));
        }
        Color operator/(float scalar) const {
            if (scalar == 0.0f) {
                throw std::runtime_error("Division by zero in Color division");
            }
            return Color(color / scalar);
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
        /**
         * @brief Sample the image using nearest neighbor interpolation
         *
         * @return Color (0, 255)
         * @warning You need convert the result to [0, 1] range
         */
        Color SampleNearest(Uv uv) const {
            return SampleNearest(uv[0], uv[1]);
        }
        /**
         * @brief Sample the image using bilinear interpolation
         *
         * @return Color (0, 255)
         * @warning You need convert the result to [0, 1] range
         */
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
        /**
         * @brief Sample the image using bilinear interpolation
         *
         * @return Color (0, 255)
         * @warning You need convert the result to [0, 1] range
         */
        Color SampleLinear(Uv uv) const {
            return SampleLinear(uv[0], uv[1]);
        }
        /**
         * @brief Sample the image using bilinear interpolation
         *
         * @return Color (0, 255)
         * @warning You need convert the result to [0, 1] range
         */
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
        bool isValid() const {
            return !name.empty();
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
     * @brief Restore triangle data
     *
     */
    struct TriangleData{
        Vertex v0, v1, v2;                    ///< Vertex positions
        TextureCoord t0, t1, t2;             ///< Texture coordinates (if available
        Color c0, c1, c2;                    ///< Vertex colors (if available)
        Normal n0, n1, n2;                   ///< Vertex normals (if available)
        bool hasTextures;                     ///< Whether texture coordinates are available
        bool hasNormals;                      ///< Whether vertex normals are available
        std::string materialName;             ///< Name of the material used by this triangle
        Material material;             ///< Pointer to the material (set after materials are loaded)
        TriangleData()
            : hasTextures(false), hasNormals(false), material() {}
        bool hasMaterial() const {
            return material.isValid();
        }
    
    };
    /**
     * @brief Restore triangle data with handle
     * @details Each triangle stores indices into the respective arrays for maximum flexibility
     */
    struct Triangle {
        VertexHandle v0, v1, v2;                    ///< Vertex positions
        TextureCoordHandle t0, t1, t2;             ///< Texture coordinates (if available)
        NormalHandle n0, n1, n2;                   ///< Vertex normals (if available)
        ColorHandle c0, c1, c2;                    ///< Vertex colors (if available)
        bool hasTextures;                           ///< Whether texture coordinates are available
        bool hasNormals;                            ///< Whether vertex normals are available
        std::string materialName;                   ///< Name of the material used by this triangle
        MaterialHandle material;                   ///< Pointer to the material (set after materials are loaded)

        /**
         * @brief Constructor
         */
        Triangle() : hasTextures(false), hasNormals(false), material() {}

        /**
         * @brief Check if this triangle has a valid material assigned
         * @return true if material is not null
         */
        bool hasMaterial() const {
            return material.isValid();
        }
        /**
         *@brief  copy data from data store
         * @param triangleDatas 
         * @param vertexDatas 
         * @param textureCoordDatas 
         * @param normalDatas 
         * @return TriangleData 
         */
        TriangleData toRawData(const TriangleDatas& triangleDatas,
                                 const VertexDatas& vertexDatas,
                                 const TextureCoordDatas& textureCoordDatas,
                                 const NormalDatas& normalDatas,
                                 const MaterialDatas& materialDatas,
                                 const ColorDatas& colorDatas = {}
                            ) const {
            TriangleData data;
            data.v0 = v0.isValid()? vertexDatas.at(v0) : Vertex();
            data.v1 = v1.isValid()? vertexDatas.at(v1) : Vertex();
            data.v2 = v2.isValid()? vertexDatas.at(v2) : Vertex();

            data.t0 = t0.isValid()? textureCoordDatas.at(t0) : TextureCoord();
            data.t1 = t1.isValid()? textureCoordDatas.at(t1) : TextureCoord();
            data.t2 = t2.isValid()? textureCoordDatas.at(t2) : TextureCoord();

            data.n0 = n0.isValid()? normalDatas.at(n0) : Normal();
            data.n1 = n1.isValid()? normalDatas.at(n1) : Normal();
            data.n2 = n2.isValid()? normalDatas.at(n2) : Normal();

            data.c0 = c0.isValid()? colorDatas.at(c0) : Color();
            data.c1 = c1.isValid()? colorDatas.at(c1) : Color();
            data.c2 = c2.isValid()? colorDatas.at(c2) : Color();

            data.material = material.isValid()? materialDatas.at(material) : Material();

            data.hasTextures = hasTextures;
            data.hasNormals = hasNormals;
            data.materialName = materialName;
            return data;
        }
    };


}


/* shader and rasterizer data */
namespace lsr3d
{
    /**
     * @brief Vertex input data structure for the vertex shader
     * @warning Can not be empty parameter create
     */
    struct vertexInputData {
        lsr3d::TriangleData triangle;
        Eigen::Matrix4f MVP;
        int width, height; ///< Viewport size for screen space conversion
    };
    /**
     * @brief Fragment output data structure for the fragment shader
     * @warning Can be empty parameter create
     */
    struct vertexOutputData {
        lsr3d::TriangleData triangle;
        bool discard = false;
    };

    /**
     * @brief Fragment input data structure for the fragment shader
     * @warning Can not be empty parameter create
     */
    struct fragementInputData {

        /* vertex shader output data */
        lsr3d::Uv position;
        lsr3d::TextureCoord textureCoord;
        lsr3d::Normal normal;
        lsr3d::Color color;
        lsr3d::Material material;

        /* other input */
        const lsr3d::ImageDatas& images; ///< Image handle for texture sampling

        /* TODO: light info */

    };
    struct fragementOutputData {
        lsr3d::Color color; ///< Output color after shading
        /* not output depth */
        // float depth; ///< Depth value for depth testing
    };
} // namespace std
#endif //RESOURCE_H
