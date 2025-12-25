#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

// Helper function to calculate pi using Leibniz formula
double calculate_pi_leibniz_A(long n)
{
  double tmp = 0.0;
  for (long i = 0; i < n; i++) {
    double sign = (i % 2 == 0) ? 1.0 : -1.0;
    tmp += sign / (2.0 * i + 1.0);
  }
  tmp = 4.0 * tmp;
  std::cout << "Leibniz: pi = " << std::fixed << std::setprecision(6) << tmp << std::endl;
  return tmp;
}

int SHOW__measure_duration(void)
{
  auto start = std::chrono::steady_clock::now();

  // do something
  calculate_pi_leibniz_A(100'000'000);

  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "Duration: " << duration.count() << " seconds" << std::endl;
  return 0;
}

/*
 * Execute function 5 times, print each duration, and report the quickest.
 */
void SHOW__benchmark_5_times_A(void)
{
  constexpr int nb_runs = 5;
  std::vector<double> durations;
  durations.reserve(nb_runs);

  for (int i = 0; i < nb_runs; i++) {
    auto start = std::chrono::steady_clock::now();

    // The function to benchmark
    calculate_pi_leibniz_A(100'000'000);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;
    double duration_sec = duration.count();

    std::cout << "Run " << (i + 1) << " / " << nb_runs << ": "
              << std::fixed << std::setprecision(6) << duration_sec
              << " seconds" << std::endl;
    durations.push_back(duration_sec);
  }

  // Find quickest and slowest
  double quickest = *std::min_element(durations.begin(), durations.end());
  double slowest = *std::max_element(durations.begin(), durations.end());

  std::cout << "\nRESULTS:" << std::endl;
  for (int i = 0; i < nb_runs; i++) {
    std::cout << "Run " << (i + 1) << " / " << nb_runs << ": "
              << std::fixed << std::setprecision(6) << durations[i]
              << " seconds" << std::endl;
  }

  std::cout << "=> quickest time: " << std::fixed << std::setprecision(6)
            << quickest << " seconds" << std::endl;
  std::cout << "=> slowest time:  " << std::fixed << std::setprecision(6)
            << slowest << " seconds = quickest + "
            << static_cast<long>(100.0 * (slowest - quickest) / quickest)
            << " %" << std::endl;
}

double calculate_pi_leibniz_B(long n)
{
  auto start = std::chrono::steady_clock::now();

  double tmp = 0.0;
  for (long i = 0; i < n; i++) {
    double sign = (i % 2 == 0) ? 1.0 : -1.0;
    tmp += sign / (2.0 * i + 1.0);
  }
  tmp = 4.0 * tmp;

  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> duration = end - start;
  double duration_sec = duration.count();

  std::cout << "Leibniz: pi = " << std::fixed << std::setprecision(6)
            << tmp << " (in " << duration_sec << " seconds)" << std::endl;

  return duration_sec;
}

/*
 * Execute function 5 times, print each duration, and report the quickest.
 * In this version, the function shall return the execution duration to be benchmarked.
 * This variant enables to do other things in the version, outside of the measured time,
 * for instance printing result.
 */
void SHOW__benchmark_5_times_B(void)
{
  constexpr int nb_runs = 5;
  std::vector<double> durations;
  durations.reserve(nb_runs);

  for (int i = 0; i < nb_runs; i++) {
    double duration = calculate_pi_leibniz_B(100'000'000);
    std::cout << "Run " << (i + 1) << " / " << nb_runs << ": "
              << std::fixed << std::setprecision(6) << duration
              << " seconds" << std::endl;
    durations.push_back(duration);
  }

  // Find quickest and slowest
  double quickest = *std::min_element(durations.begin(), durations.end());
  double slowest = *std::max_element(durations.begin(), durations.end());

  std::cout << "\nRESULTS:" << std::endl;
  for (int i = 0; i < nb_runs; i++) {
    std::cout << "Run " << (i + 1) << " / " << nb_runs << ": "
              << std::fixed << std::setprecision(6) << durations[i]
              << " seconds" << std::endl;
  }

  std::cout << "=> quickest time: " << std::fixed << std::setprecision(6)
            << quickest << " seconds" << std::endl;
  std::cout << "=> slowest time:  " << std::fixed << std::setprecision(6)
            << slowest << " seconds = quickest + "
            << static_cast<long>(100.0 * (slowest - quickest) / quickest)
            << " %" << std::endl;
}

// end
