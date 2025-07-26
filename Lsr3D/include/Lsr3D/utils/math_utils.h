/**
 * @file math_utils.h
 * @author dion (hduer_zdy@outlook.com)
 * @brief Math utility functions
 * @version 0.1
 * @date 2025/7/26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

namespace lsr3d {
    template<typename T>
    T abs(const T& t) {
        return t > 0 ? t : -t;
    }
    
    template<typename T>
    T clamp(const T& t, const T& min, const T& max) {
        return t > max ? max : (t < min ? min : t);
    }

    template<typename T>
    T max(const T& a, const T& b) {
        return a > b ? a : b;
    }
}
