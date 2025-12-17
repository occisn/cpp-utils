#include "integers_primes.hpp"
#include <cmath>
#include <iostream>
#include <vector>

/**
 * Finds the largest prime factor of a given number.
 *
 * Uses trial division with optimization: first removes all factors of 2 and 3,
 * then checks only numbers of the form 6k±1, since all primes > 3 have this form.
 *
 * @param n The number to factorize (must be positive)
 * @return The largest prime factor of n, or 0 if n is 0 or 1
 *
 * (v1 available in occisn/cpp-utils GitHub repository)
 */
long long largest_prime_factor(long long n)
{
  if (n <= 1) {
    return 0;
  }

  long long largest = 0;

  // Remove all factors of 2
  while (n % 2 == 0) {
    largest = 2;
    n /= 2;
  }

  // Remove all factors of 3
  while (n % 3 == 0) {
    largest = 3;
    n /= 3;
  }

  // Now n is odd and not divisible by 3.
  // Test divisors of the form 6k-1 and 6k+1.
  long long i = 5;
  while (i <= n / i) { // to avoid overflow
    // Check i = 6k - 1
    if (n % i == 0) {
      largest = i;
      n /= i;
    }
    // Check i+2 = 6k + 1
    else if (n % (i + 2) == 0) {
      largest = i + 2;
      n /= (i + 2);
    } else {
      i += 6;
    }
  }

  // If n is still > 1, it is prime
  if (n > 1) {
    largest = n;
  }

  return largest;
}

int SHOW__largest_prime_factor(void)
{
  const long long n = 10001;
  std::cout << "Largest prime factor of " << n << " is " << largest_prime_factor(n) << "." << std::endl;
  return EXIT_SUCCESS;
}

/**
 * Computes the Sieve of Eratosthenes up to an exclusive upper bound.
 *
 * This function generates a boolean array indicating primality for all
 * integers in the range [0, n). The algorithm marks non-prime numbers by
 * iteratively eliminating multiples of each prime starting from 2.
 *
 * Memory is managed automatically using std::vector, following RAII
 * principles. The returned container owns its data and cannot leak memory.
 *
 * @param n  Upper limit (exclusive). Values less than 2 produce an empty result.
 * @return  A std::vector<bool> where:
 *          - result[i] == true  if i is prime
 *          - result[i] == false otherwise
 *
 * (v1 available in occisn/cpp-utils GitHub repository)
 */
std::vector<bool> sieve_eratosthenes(long long n)
{
  if (n < 2) {
    return {};
  }

  std::vector<bool> is_prime(n, true);

  is_prime[0] = false;
  is_prime[1] = false;

  long long limit = static_cast<long long>(std::sqrt(n));

  for (long long p = 2; p <= limit; ++p) {
    if (is_prime[p]) {
      for (long long multiple = p * p; multiple < n; multiple += p) {
        is_prime[multiple] = false;
      }
    }
  }

  return is_prime;
}

int SHOW_sieve_eratosthenes()
{
  long long n = 50;

  auto is_prime = sieve_eratosthenes(n);

  if (is_prime.empty()) {
    std::cout << "No primes below " << n << '\n';
    return 0;
  }

  std::cout << "Primes less than " << n << ":\n";

  long long count = 0;
  for (long long i = 2; i < n; ++i) {
    if (is_prime[i]) {
      std::cout << i << ' ';
      ++count;
    }
  }

  std::cout << "\nTotal primes found: " << count << '\n';

  return 0;
}

// end
