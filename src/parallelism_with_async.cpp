#include <algorithm>
#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
#include <vector>

double SHOW_parallelism_with_async(uint64_t n) {
    auto start = std::chrono::high_resolution_clock::now();
    
    const int num_tasks = 32;
    std::vector<std::future<double>> futures;
    futures.reserve(num_tasks);
    
    uint64_t chunk_size = (n + num_tasks - 1) / num_tasks;
    
    // Launch async tasks
    for (int i = 0; i < num_tasks; i++) {
        uint64_t task_start = i * chunk_size;
        uint64_t task_end = std::min((i + 1) * chunk_size, n);
        
        futures.push_back(std::async(std::launch::async, 
            [task_start, task_end]() {
                double local_sum = 0.0;
                for (uint64_t j = task_start; j < task_end; j++) {
                    double sign = (j & 1) ? -1.0 : 1.0;
                    local_sum += sign / (2.0 * j + 1.0);
                }
                return local_sum;
            }
        ));
    }
    
    // Collect results (automatically waits for completion)
    double result = 0.0;
    for (auto& future : futures) {
        result += future.get();
    }
    result *= 4.0;
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "[std::async] pi = " << result 
              << " (in " << duration.count() << " s, " 
              << num_tasks << " tasks)\n";
    
    return duration.count();
}

// end
