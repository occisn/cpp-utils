#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// Compile with: g++ -std=c++17 -pthread -march=native -O3

// Task structure for work items
struct Task {
    uint64_t start;
    uint64_t end;
    double result;
    
    Task() : start(0), end(0), result(0.0) {}
    Task(uint64_t s, uint64_t e) : start(s), end(e), result(0.0) {}
};

// Thread pool class
class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::vector<Task> tasks;
    
    int task_count;
    int next_task;
    
    std::mutex queue_mutex;
    std::condition_variable queue_cond;
    bool shutdown;
    
    // Worker thread function
    void worker_thread() {
        while (true) {
            std::unique_lock<std::mutex> lock(queue_mutex);
            
            // Wait for work or shutdown signal
            queue_cond.wait(lock, [this] {
                return next_task < task_count || shutdown;
            });
            
            // Check if we should exit
            if (shutdown && next_task >= task_count) {
                break;
            }
            
            // Get next task
            int task_id = next_task++;
            Task& task = tasks[task_id];
            lock.unlock();
            
            // Process task
            double local_sum = 0.0;
            for (uint64_t i = task.start; i < task.end; i++) {
                double sign = (i & 1) ? -1.0 : 1.0;
                local_sum += sign / (2.0 * i + 1.0);
            }
            task.result = local_sum;
        }
    }

public:
    // Constructor
    ThreadPool(int num_threads) 
        : task_count(0), next_task(0), shutdown(false) {
        tasks.reserve(1024);
        
        // Create worker threads
        threads.reserve(num_threads);
        for (int i = 0; i < num_threads; i++) {
            threads.emplace_back(&ThreadPool::worker_thread, this);
        }
    }
    
    // Destructor
    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            shutdown = true;
        }
        queue_cond.notify_all();
        
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
    
    // Submit tasks to pool
    void submit_tasks(uint64_t n, int num_tasks) {
        std::lock_guard<std::mutex> lock(queue_mutex);
        
        task_count = num_tasks;
        next_task = 0;
        
        tasks.clear();
        tasks.resize(num_tasks);
        
        uint64_t chunk_size = (n + num_tasks - 1) / num_tasks;
        
        for (int i = 0; i < num_tasks; i++) {
            tasks[i].start = i * chunk_size;
            tasks[i].end = std::min((i + 1) * chunk_size, n);
            tasks[i].result = 0.0;
        }
        
        queue_cond.notify_all();
    }
    
    // Wait for all tasks to complete
    void wait() {
        while (true) {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (next_task >= task_count) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    
    // Get results
    double get_results() const {
        double sum = 0.0;
        for (const auto& task : tasks) {
            sum += task.result;
        }
        return sum;
    }
};

// Main computation function using thread pool
double SHOW_parallelism_with_threads(uint64_t n) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    const int num_threads = 8;
    const int num_tasks = num_threads * 4;
    
    ThreadPool pool(num_threads);
    
    // Submit tasks
    pool.submit_tasks(n, num_tasks);
    
    // Wait for completion
    pool.wait();
    
    // Collect results
    double tmp = pool.get_results() * 4.0;
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    
    std::cout << "Leibniz formula: pi = " << std::fixed << tmp 
              << " (in " << duration.count() << " s, " 
              << num_threads << " threads, " << num_tasks << " tasks)\n";
    std::cout.flush();
    
    return duration.count();
}

// end
