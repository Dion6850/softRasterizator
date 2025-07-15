/**
 * @file main.cpp
 * @author zhywyt (zhywyt@yeah.net)
 * @brief Enhanced test program for the ModelLoader module
 * @details This is a comprehensive test file for the enhanced ModelLoader module, 
 * which demonstrates loading and processing of standard OBJ files with support for
 * vertices, texture coordinates, normals, materials, and other OBJ features.
 * @version 0.2
 * @date 2025-07-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <ModelLoader.h>
#include <iomanip>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__linux__)
#include <unistd.h>
#include <fstream>
#elif defined(__APPLE__)
#include <mach/mach.h>
#endif

/**
 * @brief Get current memory usage in KB
 * @return Memory usage in kilobytes
 * @details Cross-platform memory monitoring function that works on Windows, Linux, and macOS
 */
size_t getMemoryUsageKB() {
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / 1024;
    }
    return 0;
#elif defined(__linux__)
    std::ifstream statm("/proc/self/statm");
    long rss = 0;
    if (statm) {
        long dummy;
        statm >> dummy >> rss;
        statm.close();
        long page_size_kb = sysconf(_SC_PAGESIZE) / 1024;
        return rss * page_size_kb;
    }
    return 0;
#elif defined(__APPLE__)
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
        return info.resident_size / 1024;
    }
    return 0;
#else
    return 0;
#endif
}

/**
 * @brief Print detailed model statistics
 * @param loader The ModelLoader instance to analyze
 * @param filename The filename that was loaded
 */
void printModelStatistics(const ModelLoader& loader, const std::string& filename) {
    size_t vertexCount, triangleCount, textureCount, normalCount, materialCount;
    loader.getStatistics(vertexCount, triangleCount, textureCount, normalCount, materialCount);
    
    std::cout << "\n=== 模型统计信息 (" << filename << ") ===" << std::endl;
    std::cout << "顶点数: " << vertexCount << std::endl;
    std::cout << "三角形数: " << triangleCount << std::endl;
    std::cout << "纹理坐标数: " << textureCount << std::endl;
    std::cout << "法向量数: " << normalCount << std::endl;
    std::cout << "材质数: " << materialCount << std::endl;
    
    if (!loader.getCurrentObjectName().empty()) {
        std::cout << "对象名称: " << loader.getCurrentObjectName() << std::endl;
    }
}

/**
 * @brief Print sample data from the loaded model
 * @param loader The ModelLoader instance to sample from
 */
void printSampleData(const ModelLoader& loader) {
    const auto& vertices = loader.getVertices();
    const auto& triangles = loader.getTriangles();
    const auto& textureCoords = loader.getTextureCoords();
    const auto& normals = loader.getNormals();
    
    std::cout << std::fixed << std::setprecision(6);
    
    // Show first vertex
    if (!vertices.empty()) {
        const auto& v = vertices[0];
        std::cout << "第一个顶点: (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    }
    
    // Show first triangle
    if (!triangles.empty()) {
        const auto& t = triangles[0];
        std::cout << "第一个三角形: (" << t.v0.x << ", " << t.v0.y << ", " << t.v0.z << ") - ("
                  << t.v1.x << ", " << t.v1.y << ", " << t.v1.z << ") - ("
                  << t.v2.x << ", " << t.v2.y << ", " << t.v2.z << ")" << std::endl;
        
        if (t.hasTextures) {
            std::cout << "第一个三角形纹理坐标: (" << t.t0.u << ", " << t.t0.v << ") - ("
                      << t.t1.u << ", " << t.t1.v << ") - ("
                      << t.t2.u << ", " << t.t2.v << ")" << std::endl;
        }
        
        if (t.hasNormals) {
            std::cout << "第一个三角形法向量: (" << t.n0.x << ", " << t.n0.y << ", " << t.n0.z << ") - ("
                      << t.n1.x << ", " << t.n1.y << ", " << t.n1.z << ") - ("
                      << t.n2.x << ", " << t.n2.y << ", " << t.n2.z << ")" << std::endl;
        }
    }
    
    // Show first texture coordinate
    if (!textureCoords.empty()) {
        const auto& tc = textureCoords[0];
        std::cout << "第一个纹理坐标: (" << tc.u << ", " << tc.v << ")" << std::endl;
    }
    
    // Show first normal
    if (!normals.empty()) {
        const auto& n = normals[0];
        std::cout << "第一个法向量: (" << n.x << ", " << n.y << ", " << n.z << ")" << std::endl;
    }
}

/**
 * @brief Test enhanced OBJ loading features
 * @param filename Path to OBJ file to test
 * @return true if all tests passed
 */
bool testEnhancedFeatures(const std::string& filename) {
    ModelLoader loader;
    
    std::cout << "\n=== 增强功能测试 (" << filename << ") ===" << std::endl;
    
    size_t memBefore = getMemoryUsageKB();
    bool result = loader.loadModel(filename);
    size_t memAfter = getMemoryUsageKB();
    
    std::cout << "加载结果: " << (result ? "成功" : "失败") << std::endl;
    std::cout << "[内存监控] 加载前: " << memBefore << " KB, 后: " << memAfter 
              << " KB, 增量: " << (memAfter - memBefore) << " KB" << std::endl;
    
    if (result) {
        printModelStatistics(loader, filename);
        printSampleData(loader);
        
        // Test additional features
        size_t vCount, tCount, texCount, nCount, mCount;
        loader.getStatistics(vCount, tCount, texCount, nCount, mCount);
        
        std::cout << "\n--- 功能验证 ---" << std::endl;
        std::cout << "✓ 基础顶点加载: " << (vCount > 0 ? "通过" : "失败") << std::endl;
        std::cout << "✓ 三角形生成: " << (tCount > 0 ? "通过" : "失败") << std::endl;
        std::cout << "✓ 纹理坐标支持: " << (texCount > 0 ? "通过" : "无纹理坐标") << std::endl;
        std::cout << "✓ 法向量支持: " << (nCount > 0 ? "通过" : "无法向量") << std::endl;
        std::cout << "✓ 对象名称支持: " << (!loader.getCurrentObjectName().empty() ? "通过" : "无对象名") << std::endl;
    }
    
    return result;
}

/**
 * @brief Main test function
 * @param argc Command line argument count
 * @param argv Command line arguments
 * @return Exit code (0 for success)
 */
int main(int argc, char* argv[]) {
    ModelLoader loader;
    std::cout << "[内存监控] 启动时内存: " << getMemoryUsageKB() << " KB" << std::endl;
    
    // Test non-existent file
    bool result = loader.loadModel("nonexistent.obj");
    std::cout << "加载不存在文件: " << (result ? "成功" : "失败") << std::endl;
    std::cout << "[内存监控] 加载不存在文件后内存: " << getMemoryUsageKB() << " KB" << std::endl;

    // Parse command line arguments
    std::vector<std::string> files;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--files" && i + 1 < argc) {
            for (int j = i + 1; j < argc && std::string(argv[j]).rfind("--", 0) != 0; ++j) {
                files.push_back(argv[j]);
                i = j;
            }
        }
    }

    // Test each file
    for (const auto& file : files) {
        size_t memBefore = getMemoryUsageKB();
        result = loader.loadModel(file);
        size_t memAfter = getMemoryUsageKB();
        
        std::cout << "\n加载" << file << ": " << (result ? "成功" : "失败") << std::endl;
        std::cout << "[内存监控] 加载" << file << " 前: " << memBefore << " KB, 后: " << memAfter 
                  << " KB, 增量: " << (memAfter - memBefore) << " KB" << std::endl;
        
        if (result) {
            printModelStatistics(loader, file);
            printSampleData(loader);
        }
    }
    
    // Run enhanced feature tests
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "运行增强功能测试" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // Test with the enhanced cube
    if (testEnhancedFeatures("../cube_with_textures.obj")) {
        std::cout << "\n✓ 增强功能测试: 通过" << std::endl;
    } else {
        std::cout << "\n✗ 增强功能测试: 失败" << std::endl;
    }

    std::cout << "\n[内存监控] 程序结束时内存: " << getMemoryUsageKB() << " KB" << std::endl;
    return 0;
}
