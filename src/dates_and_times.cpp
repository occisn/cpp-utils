#include <chrono>
#include <stdexcept>
#include <string>
#include <iostream>
#include <iomanip>

/**
 * @brief Parses a date-time string into a std::chrono::system_clock::time_point (UTC).
 *
 * Supports the following formats:
 *   - "DD.MM.YYYY HH:MM:SS"
 *   - "DD.MM.YYYY HH:MM:SS.mmm" (with milliseconds)
 *
 * The function validates that the entire input string is consumed
 * (ignoring trailing whitespace). Any non-whitespace trailing characters
 * will cause an exception to be thrown.
 *
 * The parsed time is interpreted as Coordinated Universal Time (UTC),
 * independent of the system's local time zone or daylight saving rules.
 *
 * @param str The date-time string to parse.
 * @return A std::chrono::system_clock::time_point representing the parsed UTC date and time.
 *
 * @throws std::runtime_error If the string does not match a supported format
 *                            or contains trailing non-whitespace characters.
 *
 * (v3, available in occisn/cpp-utils GitHub repository, 2025-12-18)
 */
std::chrono::system_clock::time_point parse_date_time_UTC(const std::string &str)
{
  std::tm tm = {};
  int milliseconds = 0;
  int chars_read = 0;

  // Try parsing with milliseconds first
  int fields = std::sscanf(str.c_str(), "%d.%d.%d %d:%d:%d.%d%n",
                           &tm.tm_mday, &tm.tm_mon, &tm.tm_year,
                           &tm.tm_hour, &tm.tm_min, &tm.tm_sec,
                           &milliseconds, &chars_read);

  // Fallback: without milliseconds
  if (fields < 7) {
    milliseconds = 0;
    chars_read = 0;
    fields = std::sscanf(str.c_str(), "%d.%d.%d %d:%d:%d%n",
                         &tm.tm_mday, &tm.tm_mon, &tm.tm_year,
                         &tm.tm_hour, &tm.tm_min, &tm.tm_sec,
                         &chars_read);
  }

  if (fields >= 6) {
    // Check for trailing non-whitespace characters
    for (size_t i = chars_read; i < str.length(); ++i) {
      if (!std::isspace(static_cast<unsigned char>(str[i]))) {
        throw std::runtime_error("Trailing characters in datetime string: " + str);
      }
    }

    tm.tm_mon -= 1;     // Month: 0–11
    tm.tm_year -= 1900; // Years since 1900
    tm.tm_isdst = 0;    // Explicitly disable DST

#if defined(_WIN32)
    // Windows provides _mkgmtime for UTC conversion
    std::time_t timeT = _mkgmtime(&tm);
#else
    // POSIX systems provide timegm for UTC conversion
    std::time_t timeT = timegm(&tm);
#endif

    if (timeT == static_cast<std::time_t>(-1)) {
      throw std::runtime_error("Failed to convert datetime to UTC: " + str);
    }

    auto tp = std::chrono::system_clock::from_time_t(timeT);
    tp += std::chrono::milliseconds(milliseconds);
    return tp;
  }

  throw std::runtime_error("Failed to parse datetime: " + str);
}

/**
 * Formats a std::chrono::system_clock::time_point as a human-readable
 * UTC (timezone-neutral) date-time string.
 *
 * Converts a C++ chrono time_point to Coordinated Universal Time (UTC)
 * and returns a formatted string representation. The output is independent
 * of the system's local time zone.
 *
 * @param tp The time_point to format.
 *
 * @return A std::string containing the formatted UTC date-time.
 *
 * Format:
 *   dd.MM.yyyy HH:MM:SS
 *
 * Example return value:
 *   "01.02.2013 00:00:00"
 *
 * @note The conversion uses UTC (via std::gmtime), not local time.
 * @note Output precision is limited to seconds (sub-second precision is discarded).
 * @note std::gmtime is not thread-safe; concurrent calls may require
 *       gmtime_r (POSIX) or gmtime_s (Windows).
 * @note The function does not append a trailing newline.
 *
 * @see std::chrono::system_clock
 * @see std::gmtime
 * @see std::put_time
 *
 * (v1, available in occisn/cpp-utils GitHub repository, 2025-12-18)
 */
std::string format_date_time_UTC(const std::chrono::system_clock::time_point &tp)
{
  std::time_t tt = std::chrono::system_clock::to_time_t(tp);
  std::tm tm = *std::gmtime(&tt);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%d.%m.%Y %H:%M:%S");
  return oss.str();
}

/**
 * Prints a chrono time_point to standard output in human-readable format.
 *
 * Converts a C++ chrono time_point to a formatted date-time string and prints it
 * to std::cout. The output format matches the input format used in OHLC files.
 *
 * @param tp The time_point to print
 *
 * Output format: dd.MM.yyyy HH:MM:SS
 * Example output: "01.02.2013 00:00:00"
 *
 * The parsed time is interpreted as Coordinated Universal Time (UTC),
 * independent of the system's local time zone or daylight saving rules.
 *
 * @note Milliseconds are not displayed (only seconds precision)
 * @note Output is sent directly to std::cout with no trailing newline
 *
 * (v1, available in occisn/cpp-utils GitHub repository, 2025-12-18)
 */
void print_date_time_UTC(const std::chrono::system_clock::time_point &tp)
{
  std::time_t tt = std::chrono::system_clock::to_time_t(tp);
  std::tm tm = *std::gmtime(&tt);
  std::cout << std::put_time(&tm, "%d.%m.%Y %H:%M:%S");
}

// end
