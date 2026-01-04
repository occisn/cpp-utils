#include <cstdint>
#include <cstdio>
#include <ctime>
#include <omp.h>

// Compile with: -fopenmp -march=native

// With reduction:
double SHOW_parallelism_with_openmp_1(uint64_t n)
{
  timespec start_time, end_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  
  double tmp = 0.0;
  
#pragma omp parallel for schedule(static) reduction(+ : tmp)
  for (uint64_t i = 0; i < n; i++) {
    double sign = (i & 1) ? -1.0 : 1.0;
    tmp += sign / (2.0 * i + 1.0);
  }
  
  tmp *= 4.0;
  
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  double duration = (end_time.tv_sec - start_time.tv_sec) +
                    (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
  
  std::printf("Leibniz formula: pi = %.20f (in %f s)\n", tmp, duration);
  std::fflush(stdout);
  
  return duration;
}

// Without reduction:
double SHOW_parallelism_with_openmp_2(uint64_t n)
{
  timespec start_time, end_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  
  double tmp = 0.0;
  
#pragma omp parallel
  {
    double local_sum = 0.0;
    
#pragma omp for nowait
    for (uint64_t i = 0; i < n; i++) {
      double sign = (i & 1) ? -1.0 : 1.0;
      local_sum += sign / (2.0 * i + 1.0);
    }
    
#pragma omp atomic
    tmp += local_sum;
  }
  
  tmp *= 4.0;
  
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  double duration = (end_time.tv_sec - start_time.tv_sec) +
                    (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
  
  std::printf("Leibniz formula: pi = %.20f (in %f s)\n", tmp, duration);
  std::fflush(stdout);
  
  return duration;
}

// Optional: Using C++ chrono for timing (more idiomatic C++)
#include <chrono>
#include <iostream>

double SHOW_parallelism_with_openmp_1_chrono(uint64_t n)
{
  auto start_time = std::chrono::high_resolution_clock::now();
  
  double tmp = 0.0;
  
#pragma omp parallel for schedule(static) reduction(+ : tmp)
  for (uint64_t i = 0; i < n; i++) {
    double sign = (i & 1) ? -1.0 : 1.0;
    tmp += sign / (2.0 * i + 1.0);
  }
  
  tmp *= 4.0;
  
  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end_time - start_time;
  
  std::cout << "Leibniz formula: pi = " << std::fixed << tmp 
            << " (in " << duration.count() << " s)\n";
  std::cout.flush();
  
  return duration.count();
}

double SHOW_parallelism_with_openmp_2_chrono(uint64_t n)
{
  auto start_time = std::chrono::high_resolution_clock::now();
  
  double tmp = 0.0;
  
#pragma omp parallel
  {
    double local_sum = 0.0;
    
#pragma omp for nowait
    for (uint64_t i = 0; i < n; i++) {
      double sign = (i & 1) ? -1.0 : 1.0;
      local_sum += sign / (2.0 * i + 1.0);
    }
    
#pragma omp atomic
    tmp += local_sum;
  }
  
  tmp *= 4.0;
  
  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end_time - start_time;
  
  std::cout << "Leibniz formula: pi = " << std::fixed << tmp 
            << " (in " << duration.count() << " s)\n";
  std::cout.flush();
  
  return duration.count();
}

// end
