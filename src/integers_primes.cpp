#include "integers_primes.hpp"
#include <iostream>

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

// end
