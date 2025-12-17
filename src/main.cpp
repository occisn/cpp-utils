#include "dates_and_times.hpp"
#include "doubles.hpp"
#include "duration.hpp"
#include "integers_primes.hpp"
#include <cstdlib>
#include <iostream>

int main()
{
  std::cout << "Hello world!" << std::endl;

  std::cout << std::endl;
  std::cout << "dates_and_times / parse_date_time" << std::endl;
  std::cout << "---------------------------------" << std::endl;
  [[maybe_unused]] auto tmp1 = parse_date_time("01.02.2013 00:00:00.000");
  std::cout << "Done." << std::endl;

  std::cout << std::endl;
  std::cout << "doubles / parse_double" << std::endl;
  std::cout << "----------------------" << std::endl;
  std::cout << "1,53 with delimiter ',' is parsed into " << parse_double("1,53", ',') << std::endl;

  std::cout << std::endl;
  std::cout << "duration / measure_duration" << std::endl;
  std::cout << "---------------------------" << std::endl;
  measure_duration();

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
