
/**
 * @brief Reverses the digits of a non-negative integer.
 *
 * This function takes a non-negative integer `n` and returns a new integer
 * whose digits are in reverse order.
 *
 * Examples:
 *   reverse_number(123)   -> 321
 *   reverse_number(1000)  -> 1
 *   reverse_number(0)     -> 0
 *
 * @param n Non-negative integer to reverse
 * @return Reversed integer
 *
 * (v1 available in occisn/cpp-utils GitHub repository, 2025-12-19)
 */
int reverse_number(int n)
{
  int reversed = 0;
  while (n > 0) {
    reversed = reversed * 10 + (n % 10);
    n /= 10;
  }
  return reversed;
}

// end
