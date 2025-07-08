/**
 * @file main.cpp
 * @author zhywyt (zhywyt@yeah.net)
 * @brief test the module loder
 * @details This is a test file for the loder module, which is used to load object models from files.
 * @version 0.1
 * @date 2025-07-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include <iostream>
#include <ModelLoader.h>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__linux__)
#include <unistd.h>
#include <fstream>
#elif defined(__APPLE__)
#include <mach/mach.h>
#endif

// 获取当前进程的内存使用（单位：KB）

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

int main(int argc, char* argv[]) {
    ModelLoader loader;
    std::cout << "[内存监控] 启动时内存: " << getMemoryUsageKB() << " KB" << std::endl;
    // 测试用例：尝试加载一个不存在的模型文件
    bool result = loader.loadModel("nonexistent.obj");
    std::cout << "加载不存在文件: " << (result ? "成功" : "失败") << std::endl;
    std::cout << "[内存监控] 加载不存在文件后内存: " << getMemoryUsageKB() << " KB" << std::endl;

    // 检查是否有 --files 参数
    std::vector<std::string> files;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--files" && i + 1 < argc) {
            // 收集后续所有文件名直到下一个以--开头的参数或结尾
            for (int j = i + 1; j < argc && std::string(argv[j]).rfind("--", 0) != 0; ++j) {
                files.push_back(argv[j]);
                i = j;
            }
        }
    }

    for (const auto& file : files) {
        size_t memBefore = getMemoryUsageKB();
        result = loader.loadModel(file);
        size_t memAfter = getMemoryUsageKB();
        std::cout << "加载" << file << ": " << (result ? "成功" : "失败") << std::endl;
        std::cout << "[内存监控] 加载" << file << " 前: " << memBefore << " KB, 后: " << memAfter << " KB, 增量: " << (memAfter - memBefore) << " KB" << std::endl;
        if (result) {
            std::cout << "顶点数: " << loader.getVertices().size() << std::endl;
            std::cout << "三角形数: " << loader.getTriangles().size() << std::endl;
            // 打印部分顶点和三角形信息
            if (!loader.getVertices().empty()) {
                const auto& v = loader.getVertices()[0];
                std::cout << "第一个顶点: (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
            }
            if (!loader.getTriangles().empty()) {
                const auto& t = loader.getTriangles()[0];
                std::cout << "第一个三角形: (" << t.v0.x << ", " << t.v0.y << ", " << t.v0.z << ") - ("
                          << t.v1.x << ", " << t.v1.y << ", " << t.v1.z << ") - ("
                          << t.v2.x << ", " << t.v2.y << ", " << t.v2.z << ")" << std::endl;
            }
        }
    }

    std::cout << "[内存监控] 程序结束时内存: " << getMemoryUsageKB() << " KB" << std::endl;
    return 0;
}
