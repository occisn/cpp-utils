#include "integers_primes.hpp"
#include <catch_amalgamated.hpp>
#include <vector>
#include <cstdint>

TEST_CASE("Largest prime factor calculation", "[primes]")
{

  SECTION("Small primes")
  {
    REQUIRE(largest_prime_factor(2) == 2);
    REQUIRE(largest_prime_factor(3) == 3);
    REQUIRE(largest_prime_factor(5) == 5);
    REQUIRE(largest_prime_factor(7) == 7);
    REQUIRE(largest_prime_factor(11) == 11);
    REQUIRE(largest_prime_factor(13) == 13);
  }

  SECTION("Larger primes")
  {
    REQUIRE(largest_prime_factor(97) == 97);
    REQUIRE(largest_prime_factor(101) == 101);
    REQUIRE(largest_prime_factor(997) == 997);
    REQUIRE(largest_prime_factor(9973) == 9973);
  }

  SECTION("Powers of 2")
  {
    REQUIRE(largest_prime_factor(4) == 2);
    REQUIRE(largest_prime_factor(8) == 2);
    REQUIRE(largest_prime_factor(16) == 2);
    REQUIRE(largest_prime_factor(1024) == 2);
  }

  SECTION("Powers of 3")
  {
    REQUIRE(largest_prime_factor(9) == 3);
    REQUIRE(largest_prime_factor(27) == 3);
    REQUIRE(largest_prime_factor(81) == 3);
    REQUIRE(largest_prime_factor(243) == 3);
  }

  SECTION("Powers of other primes")
  {
    REQUIRE(largest_prime_factor(25) == 5);
    REQUIRE(largest_prime_factor(125) == 5);
    REQUIRE(largest_prime_factor(49) == 7);
    REQUIRE(largest_prime_factor(121) == 11);
  }

  SECTION("Products of two primes")
  {
    REQUIRE(largest_prime_factor(6) == 3);
    REQUIRE(largest_prime_factor(10) == 5);
    REQUIRE(largest_prime_factor(14) == 7);
    REQUIRE(largest_prime_factor(21) == 7);
    REQUIRE(largest_prime_factor(33) == 11);
    REQUIRE(largest_prime_factor(65) == 13);
  }

  SECTION("Products of three primes")
  {
    REQUIRE(largest_prime_factor(30) == 5);
    REQUIRE(largest_prime_factor(42) == 7);
    REQUIRE(largest_prime_factor(66) == 11);
    REQUIRE(largest_prime_factor(78) == 13);
  }

  SECTION("Products with repeated prime factors")
  {
    REQUIRE(largest_prime_factor(100) == 5);
    REQUIRE(largest_prime_factor(196) == 7);
    REQUIRE(largest_prime_factor(242) == 11);
  }

  SECTION("Products of 6k-1 form primes")
  {
    REQUIRE(largest_prime_factor(35) == 7);  // 5*7
    REQUIRE(largest_prime_factor(55) == 11); // 5*11
    REQUIRE(largest_prime_factor(85) == 17); // 5*17
  }

  SECTION("Products of 6k+1 form primes")
  {
    REQUIRE(largest_prime_factor(77) == 11);  // 7*11
    REQUIRE(largest_prime_factor(91) == 13);  // 7*13
    REQUIRE(largest_prime_factor(133) == 19); // 7*19
  }

  SECTION("Mixed 6k±1 products")
  {
    REQUIRE(largest_prime_factor(15) == 5);   // 3*5
    REQUIRE(largest_prime_factor(39) == 13);  // 3*13
    REQUIRE(largest_prime_factor(115) == 23); // 5*23
  }

  SECTION("Larger composite numbers")
  {
    REQUIRE(largest_prime_factor(323) == 19); // 17*19
    REQUIRE(largest_prime_factor(325) == 13); // 5^2*13
  }

  SECTION("Medium-sized numbers")
  {
    REQUIRE(largest_prime_factor(10100) == 101); // 2^2*5^2*101
    REQUIRE(largest_prime_factor(1270) == 127);  // 2*5*127
  }

  SECTION("Products of large primes")
  {
    REQUIRE(largest_prime_factor(89 * 97) == 97);
    REQUIRE(largest_prime_factor(97 * 103) == 103);
  }

  SECTION("Very large primes")
  {
    REQUIRE(largest_prime_factor(1000000007LL) == 1000000007LL);
    REQUIRE(largest_prime_factor(2LL * 999999937LL) == 999999937LL);
  }

  SECTION("Testing 6k-1 and 6k+1 checking logic")
  {
    REQUIRE(largest_prime_factor(17 * 2) == 17);
    REQUIRE(largest_prime_factor(19 * 2) == 19);
    REQUIRE(largest_prime_factor(23 * 3) == 23);
    REQUIRE(largest_prime_factor(31 * 5) == 31);
  }

  SECTION("Multiple 6k±1 iterations needed")
  {
    REQUIRE(largest_prime_factor(37 * 2) == 37);
    REQUIRE(largest_prime_factor(41 * 3) == 41);
    REQUIRE(largest_prime_factor(43 * 5) == 43);
  }
}


TEST_CASE("Sieve of Eratosthenes produces correct primality results", "[sieve]")
{
    SECTION("Returns empty result for n < 2")
    {
        REQUIRE(sieve_eratosthenes(0).empty());
        REQUIRE(sieve_eratosthenes(1).empty());
        REQUIRE(sieve_eratosthenes(-5).empty());
    }

    SECTION("Handles smallest valid upper bound (n = 2)")
    {
        auto is_prime = sieve_eratosthenes(2);

        REQUIRE(is_prime.size() == 2);
        REQUIRE_FALSE(is_prime[0]);
        REQUIRE_FALSE(is_prime[1]);
    }

    SECTION("Correctly identifies primes and composites below 10")
    {
        auto is_prime = sieve_eratosthenes(10);

        REQUIRE(is_prime.size() == 10);

        REQUIRE_FALSE(is_prime[0]);
        REQUIRE_FALSE(is_prime[1]);

        REQUIRE(is_prime[2]);
        REQUIRE(is_prime[3]);
        REQUIRE_FALSE(is_prime[4]);
        REQUIRE(is_prime[5]);
        REQUIRE_FALSE(is_prime[6]);
        REQUIRE(is_prime[7]);
        REQUIRE_FALSE(is_prime[8]);
        REQUIRE_FALSE(is_prime[9]);
    }

    SECTION("Marks known composite numbers as non-prime")
    {
        auto is_prime = sieve_eratosthenes(50);

        std::vector<long long> composites = {
            4, 6, 8, 9, 10, 12, 14, 15,
            16, 18, 20, 21, 22, 24, 25
        };

        for (long long c : composites) {
            REQUIRE_FALSE(is_prime[c]);
        }
    }

    SECTION("Finds the correct number of primes below 100")
    {
        auto is_prime = sieve_eratosthenes(100);

        long long count = 0;
        for (long long i = 2; i < 100; ++i) {
            if (is_prime[i]) {
                ++count;
            }
        }

        // There are exactly 25 primes less than 100
        REQUIRE(count == 25);
    }
}


// end
