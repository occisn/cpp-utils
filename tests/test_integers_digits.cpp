#include "integers_digits.hpp"
#include <catch_amalgamated.hpp>

TEST_CASE("TEST_CASE_reverse_number")
{
  SECTION("Single digit")
  {
    REQUIRE(reverse_number(7) == 7);
  }

  SECTION("Multiple digits")
  {
    REQUIRE(reverse_number(123) == 321);
  }

  SECTION("Trailing zeros")
  {
    REQUIRE(reverse_number(1200) == 21);
  }

  SECTION("Palindrome number")
  {
    REQUIRE(reverse_number(1221) == 1221);
  }

  SECTION("Zero input")
  {
    REQUIRE(reverse_number(0) == 0);
  }
}

// end
