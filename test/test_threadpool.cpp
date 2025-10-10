/**
 * @file test_threadpool.cpp
 * @author dion (hduer_zdy@outlook.com)
 * @brief Test for ThreadPool implementation
 * @version 0.1
 * @date 2025-10-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <vector>
#include <chrono>
#include "utils/ThreadPool.h"

// Test function that simulates work
int compute(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return x * x;
}

int main() {
    std::cout << "Testing ThreadPool..." << std::endl;
    
    // Test 1: Basic functionality
    {
        std::cout << "\nTest 1: Basic task execution" << std::endl;
        ThreadPool pool(4);
        std::vector<std::future<int>> results;

        for(int i = 0; i < 8; ++i) {
            results.emplace_back(
                pool.enqueue([i] {
                    return i * i;
                })
            );
        }

        for(size_t i = 0; i < results.size(); ++i) {
            int expected = i * i;
            int actual = results[i].get();
            if (expected == actual) {
                std::cout << "Task " << i << ": " << actual << " ✓" << std::endl;
            } else {
                std::cout << "Task " << i << ": FAILED (expected " << expected 
                          << ", got " << actual << ")" << std::endl;
                return 1;
            }
        }
    }

    // Test 2: Thread pool with function arguments
    {
        std::cout << "\nTest 2: Task execution with arguments" << std::endl;
        ThreadPool pool(4);
        std::vector<std::future<int>> results;

        for(int i = 0; i < 8; ++i) {
            results.emplace_back(pool.enqueue(compute, i));
        }

        for(size_t i = 0; i < results.size(); ++i) {
            int expected = i * i;
            int actual = results[i].get();
            if (expected == actual) {
                std::cout << "Task " << i << ": " << actual << " ✓" << std::endl;
            } else {
                std::cout << "Task " << i << ": FAILED (expected " << expected 
                          << ", got " << actual << ")" << std::endl;
                return 1;
            }
        }
    }

    // Test 3: Performance test
    {
        std::cout << "\nTest 3: Performance comparison" << std::endl;
        
        const int num_tasks = 100;
        
        // Sequential execution
        auto start_seq = std::chrono::high_resolution_clock::now();
        std::vector<int> seq_results;
        for(int i = 0; i < num_tasks; ++i) {
            seq_results.push_back(compute(i));
        }
        auto end_seq = std::chrono::high_resolution_clock::now();
        auto duration_seq = std::chrono::duration_cast<std::chrono::milliseconds>(end_seq - start_seq);
        
        // Parallel execution
        auto start_par = std::chrono::high_resolution_clock::now();
        {
            ThreadPool pool(4);
            std::vector<std::future<int>> results;
            
            for(int i = 0; i < num_tasks; ++i) {
                results.emplace_back(pool.enqueue(compute, i));
            }
            
            for(auto& result : results) {
                result.get();
            }
        }
        auto end_par = std::chrono::high_resolution_clock::now();
        auto duration_par = std::chrono::duration_cast<std::chrono::milliseconds>(end_par - start_par);
        
        std::cout << "Sequential execution: " << duration_seq.count() << "ms" << std::endl;
        std::cout << "Parallel execution:   " << duration_par.count() << "ms" << std::endl;
        std::cout << "Speedup: " << (double)duration_seq.count() / duration_par.count() << "x" << std::endl;
    }

    // Test 4: Thread pool size
    {
        std::cout << "\nTest 4: Thread pool size" << std::endl;
        ThreadPool pool(8);
        std::cout << "Thread pool has " << pool.size() << " threads ✓" << std::endl;
    }

    std::cout << "\n✓ All tests passed!" << std::endl;
    return 0;
}
