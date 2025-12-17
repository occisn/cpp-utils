#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>

/**
 * Parses a string into a double, handling different decimal separators.
 *
 * This function converts locale-specific number formats to standard C++ doubles.
 * It replaces the specified decimal separator with a dot (.) before parsing.
 *
 * @param str The numeric string to parse (e.g., "1,36115" or "1.36115")
 * @param decimalSeparator The decimal separator used in the input string (',' or '.')
 * @return double The parsed numeric value
 * @throws std::invalid_argument if the string cannot be converted to a double,
 *         if multiple decimal separators are found, or if trailing characters exist
 * @throws std::out_of_range if the converted value falls out of the range of double
 *
 * Examples:
 *   parseDouble("1,36115", ',') returns 1.36115
 *   parseDouble("1.36115", '.') returns 1.36115
 *   parseDouble("29059.0996", '.') returns 29059.0996
 *   parseDouble("1,2,3", ',') throws std::invalid_argument (multiple separators)
 *   parseDouble("1.2.3", '.') throws std::invalid_argument (multiple separators)
 *   parseDouble("123abc", '.') throws std::invalid_argument (trailing characters)
 *
 * (v2, available in occisn/cpp-utils GitHub repository, 2025-12-17)
 */
double parse_double(const std::string &str, char decimalSeparator)
{
  std::string s = str;

  // Check for multiple decimal separators before replacement
  size_t separatorCount = std::count(s.begin(), s.end(), decimalSeparator);
  if (separatorCount > 1) {
    throw std::invalid_argument("Multiple decimal separators found in string: " + str);
  }

  // Replace decimal separator if needed
  if (decimalSeparator != '.') {
    std::replace(s.begin(), s.end(), decimalSeparator, '.');
  }

  // After replacement, check if there are multiple dots
  // This handles cases where the input had dots when comma was the separator
  size_t dotCount = std::count(s.begin(), s.end(), '.');
  if (dotCount > 1) {
    throw std::invalid_argument("Multiple decimal separators found in string: " + str);
  }

  // Use stod with position tracking
  // Note: std::stod will throw std::out_of_range or std::invalid_argument naturally
  size_t pos = 0;
  double result = std::stod(s, &pos);

  // Check if all characters were consumed (ignoring trailing whitespace)
  while (pos < s.length() && std::isspace(static_cast<unsigned char>(s[pos]))) {
    ++pos;
  }

  if (pos != s.length()) {
    throw std::invalid_argument("Invalid trailing characters in string: " + str);
  }

  return result;
}

// end
