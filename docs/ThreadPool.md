# ThreadPool 线程池

## 简介

ThreadPool 是一个现代 C++20 线程池实现，用于在软光栅渲染器中执行并行任务。它提供了简单的接口来提交任务并获取结果。

## 特性

- **自动线程管理**: 自动创建和管理工作线程
- **任务队列**: 线程安全的任务队列
- **Future 支持**: 使用 std::future 获取任务结果
- **RAII 设计**: 析构时自动等待所有任务完成
- **灵活配置**: 可配置线程数量，默认使用硬件并发数

## 使用方法

### 基本用法

```cpp
#include <utils/ThreadPool.h>

// 创建线程池（使用硬件并发数）
ThreadPool pool;

// 或指定线程数
ThreadPool pool(4);

// 提交任务
auto result = pool.enqueue([](int x) {
    return x * x;
}, 42);

// 获取结果
int value = result.get();  // value = 1764
```

### 并行处理多个任务

```cpp
std::vector<std::future<int>> results;

for(int i = 0; i < 100; ++i) {
    results.emplace_back(
        pool.enqueue([i] {
            // 执行某些计算
            return compute(i);
        })
    );
}

// 等待所有任务完成
for(auto& result : results) {
    int value = result.get();
    // 处理结果
}
```

### 在光栅化中的应用

在光栅化渲染中，可以将屏幕分成多个区域并行处理：

```cpp
#include <utils/ThreadPool.h>

void parallel_rasterize_triangle() {
    ThreadPool pool(std::thread::hardware_concurrency());
    
    // 将屏幕分成多个水平带
    int num_bands = pool.size();
    int band_height = screen_height / num_bands;
    
    std::vector<std::future<void>> results;
    
    for(int band = 0; band < num_bands; ++band) {
        int y_start = band * band_height;
        int y_end = y_start + band_height;
        
        results.emplace_back(
            pool.enqueue([=]() {
                // 光栅化这个带中的像素
                for(int y = y_start; y < y_end; ++y) {
                    for(int x = 0; x < screen_width; ++x) {
                        if(inside_triangle(x, y)) {
                            // 绘制像素
                            draw_pixel(x, y);
                        }
                    }
                }
            })
        );
    }
    
    // 等待所有带完成
    for(auto& result : results) {
        result.get();
    }
}
```

## API 参考

### 构造函数

```cpp
explicit ThreadPool(size_t threads = std::thread::hardware_concurrency());
```

创建一个包含指定数量工作线程的线程池。

**参数:**
- `threads`: 工作线程数量，默认为硬件并发数

### enqueue

```cpp
template<class F, class... Args>
auto enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::invoke_result<F, Args...>::type>;
```

向线程池提交一个任务。

**参数:**
- `f`: 要执行的函数
- `args`: 传递给函数的参数

**返回:**
- `std::future`: 用于获取任务结果的 future 对象

### size

```cpp
size_t size() const;
```

获取线程池中的工作线程数量。

**返回:**
- 线程数量

## 性能

在测试中，ThreadPool 在 4 核处理器上实现了接近 4 倍的加速：

```
Sequential execution: 1009ms
Parallel execution:   253ms
Speedup: 3.98x
```

## 注意事项

1. **线程安全**: 所有 ThreadPool 方法都是线程安全的
2. **任务顺序**: 任务的执行顺序不保证与提交顺序相同
3. **异常处理**: 任务中抛出的异常会在调用 `future.get()` 时重新抛出
4. **资源管理**: 线程池析构时会自动等待所有任务完成

## 示例

完整的使用示例请参考：
- `test/test_threadpool.cpp` - 基础功能测试
- `include/test/test_threadpool_rasterization.h` - 光栅化应用示例

## 构建和测试

```bash
mkdir build && cd build
cmake ..
make test_threadpool
./test_threadpool
```

## 依赖

- C++20 或更高版本
- pthread (通常在 Linux/Unix 系统上)
- 标准库线程支持

## 许可证

Copyright (c) 2025
