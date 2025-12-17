#include <chrono>
#include <stdexcept>
#include <string>

/**
 * @brief Parses a date-time string into a std::chrono::system_clock::time_point.
 *
 * Supports the following formats:
 *   - "DD.MM.YYYY HH:MM:SS"
 *   - "DD.MM.YYYY HH:MM:SS.mmm" (with milliseconds)
 *
 * The function validates that the entire input string is consumed
 * (ignoring trailing whitespace). Any non-whitespace trailing characters
 * will cause an exception to be thrown.
 *
 * The parsed time is interpreted in the local time zone. Daylight Saving
 * Time (DST) is determined automatically by the system.
 *
 * @param str The date-time string to parse.
 * @return A std::chrono::system_clock::time_point representing the parsed date and time.
 *
 * @throws std::runtime_error If the string does not match a supported format
 *                            or contains trailing non-whitespace characters.
 *
 * (v2, available in occisn/cpp-utils GitHub repository, 2025-12-17)
 */
std::chrono::system_clock::time_point parse_date_time(const std::string &str)
{
  std::tm tm = {};
  int milliseconds = 0;
  int chars_read = 0;

  // Try parsing with milliseconds first: 01.02.2013 00:00:00.000
  int fields = sscanf(str.c_str(), "%d.%d.%d %d:%d:%d.%d%n",
                      &tm.tm_mday, &tm.tm_mon, &tm.tm_year,
                      &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &milliseconds, &chars_read);

  // If that didn't work, try format without milliseconds: 01.02.2013 00:00:00
  if (fields < 7) {
    milliseconds = 0;
    chars_read = 0; // Reset chars_read
    fields = sscanf(str.c_str(), "%d.%d.%d %d:%d:%d%n",
                    &tm.tm_mday, &tm.tm_mon, &tm.tm_year,
                    &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &chars_read);
  }

  if (fields >= 6) {
    // Check for trailing non-whitespace characters
    for (size_t i = chars_read; i < str.length(); ++i) {
      if (!std::isspace(static_cast<unsigned char>(str[i]))) {
        throw std::runtime_error("Trailing characters in datetime string: " + str);
      }
    }

    tm.tm_mon -= 1;     // Month is 0-11
    tm.tm_year -= 1900; // Year since 1900
    tm.tm_isdst = -1;   // Let mktime determine DST automatically

    auto timeT = std::mktime(&tm);
    auto tp = std::chrono::system_clock::from_time_t(timeT);
    tp += std::chrono::milliseconds(milliseconds);
    return tp;
  }

  throw std::runtime_error("Failed to parse datetime: " + str);
}

// end
