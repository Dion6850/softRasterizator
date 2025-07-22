/**
 * @file shader.h
 * @author dion (hduer_zdy@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2025/7/15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <Eigen/Core>
#include <Lsr3D/core/resource.h>
#include <Lsr3D/utils/utils.h>

namespace lsr3d
{
    template<typename InputType, typename OutputType>
    class Shader {
    public:

        ~Shader() = default; ///< Virtual destructor for proper cleanup
        virtual void shading(const InputType& input, OutputType& output) = 0; ///< Pure virtual function for shading
        virtual std::string getName() const {
            return shaderName; ///< Get the shader name
        };
        Shader(std::string name) :shaderName(name) {}
    protected:
        std::string shaderName;
    };

    /**
     * @brief Simple triangle vertex shader
     *
     */
    class triangleVertexShader : public Shader<vertexInputData, vertexOutputData> {
    public:
        triangleVertexShader() :Shader("triangleVertexShader") {
            enableFaceCulling(true);
        }
        void enableFaceCulling(bool enable = true) {
            isEnabledFaceCull = enable;
        }
        void shading(const vertexInputData& input, vertexOutputData& output) override;

    private:
        bool isEnabledFaceCull;

    };

    /**
     * @brief base class for fragment shader
     *
     */
    class triangleFragmentShader : public Shader<fragementInputData, fragementOutputData> {
    public:
        triangleFragmentShader() :Shader("triangleFragmentShader") {}
        virtual void shading(const fragementInputData& input, fragementOutputData& output) override;
        bool isSupportEarlyZBuffer() const {
            return m_isSupportEarlyZBuffer; ///< Check if early Z-buffer is supported
        }
    protected:
        bool m_isSupportEarlyZBuffer = true; ///< 是否支持早期Z缓冲
    };
}
#endif //SHADER_H
