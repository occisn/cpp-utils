#include "dates_and_times.hpp"
#include "doubles.hpp"
#include "duration.hpp"
#include "files.hpp"
#include "integers_digits.hpp"
#include "integers_primes.hpp"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>

int main()
{
  std::cout << "Hello world!" << std::endl;

  std::cout << std::endl;
  std::cout << "dates_and_times / parse_date_time_UTC" << std::endl;
  std::cout << "-------------------------------------" << std::endl;
  [[maybe_unused]] auto tmp1 = parse_date_time_UTC("01.02.2013 00:00:00.000");
  std::cout << "Done." << std::endl;

  std::cout << std::endl;
  std::cout << "dates_and_times / format_date_time_UTC" << std::endl;
  std::cout << "--------------------------------------" << std::endl;
  std::tm tm{};
  tm.tm_year = 2005 - 1900;
  tm.tm_mon = 3; // April
  tm.tm_mday = 3;
  tm.tm_hour = 6;
  tm.tm_min = 7;
  tm.tm_sec = 8;
#if defined(_WIN32)
  std::time_t tt = _mkgmtime(&tm);
#else
  std::time_t tt = timegm(&tm);
#endif
  auto tp = std::chrono::system_clock::from_time_t(tt);
  std::cout << format_date_time_UTC(tp) << std::endl;

  std::cout << std::endl;
  std::cout << "doubles / parse_double" << std::endl;
  std::cout << "----------------------" << std::endl;
  std::cout << "1,53 with delimiter ',' is parsed into " << parse_double("1,53", ',') << std::endl;

  std::cout << std::endl;
  std::cout << "duration / measure_duration" << std::endl;
  std::cout << "---------------------------" << std::endl;
  measure_duration();

  std::cout << std::endl;
  std::cout << "files / count_lines" << std::endl;
  std::cout << "-------------------" << std::endl;
  std::cout << "Current path: " << std::filesystem::current_path() << std::endl;
  std::cout << "Number of lines in 'src/main.c' file: " << count_lines("./src/main.cpp", false) << std::endl;
  std::cout << std::endl;
  std::cout << "integers_primes / SHOW__largest_prime_factor" << std::endl;
  std::cout << "--------------------------------------------" << std::endl;
  SHOW__largest_prime_factor();

  std::cout << std::endl;
  std::cout << "integers_primes / SHOW_sieve_eratosthenes" << std::endl;
  std::cout << "-----------------------------------------" << std::endl;
  SHOW_sieve_eratosthenes();

  return EXIT_SUCCESS;
}
