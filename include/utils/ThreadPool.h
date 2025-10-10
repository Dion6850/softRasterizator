/**
 * @file ThreadPool.h
 * @author dion (hduer_zdy@outlook.com)
 * @brief A thread pool implementation for parallel task execution
 * @version 0.1
 * @date 2025-10-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <stdexcept>

/**
 * @brief A thread pool for executing tasks in parallel
 * 
 * This class manages a pool of worker threads that can execute tasks asynchronously.
 * Tasks can be submitted using the enqueue method, which returns a future for the result.
 */
class ThreadPool {
public:
    /**
     * @brief Construct a new Thread Pool object
     * 
     * @param threads Number of worker threads to create (default: hardware concurrency)
     */
    explicit ThreadPool(size_t threads = std::thread::hardware_concurrency());

    /**
     * @brief Destroy the Thread Pool object
     * 
     * Waits for all tasks to complete and joins all worker threads
     */
    ~ThreadPool();

    /**
     * @brief Enqueue a task for execution
     * 
     * @tparam F Function type
     * @tparam Args Argument types
     * @param f Function to execute
     * @param args Arguments to pass to the function
     * @return std::future<typename std::invoke_result<F, Args...>::type> Future for the result
     */
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::invoke_result<F, Args...>::type>;

    /**
     * @brief Get the number of worker threads
     * 
     * @return size_t Number of threads
     */
    size_t size() const { return workers.size(); }

    // Delete copy constructor and assignment operator
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    // Worker threads
    std::vector<std::thread> workers;
    
    // Task queue
    std::queue<std::function<void()>> tasks;
    
    // Synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// Implementation of enqueue
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::invoke_result<F, Args...>::type>
{
    using return_type = typename std::invoke_result<F, Args...>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // Don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

#endif // THREADPOOL_H
