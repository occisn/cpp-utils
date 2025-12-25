#include <chrono>
#include <iostream>

int measure_duration(void)
{
  auto start = std::chrono::steady_clock::now();
  // do something
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "Duration: " << duration.count() << " seconds" << std::endl;
  return 0;
}

// end
