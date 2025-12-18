#include "dates_and_times.hpp"
#include <catch_amalgamated.hpp>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <string>

TEST_CASE("parse_date_time_UTC parses UTC date-time strings correctly", "[parse_date_time_UTC][utc]")
{
  using clock = std::chrono::system_clock;

  SECTION("Parses date-time without milliseconds")
  {
    auto tp = parse_date_time_UTC("01.02.2013 00:00:00");

    std::time_t tt = clock::to_time_t(tp);
    std::tm *gmt = std::gmtime(&tt);

    REQUIRE(gmt != nullptr);
    REQUIRE(gmt->tm_year == 2013 - 1900);
    REQUIRE(gmt->tm_mon == 1); // February
    REQUIRE(gmt->tm_mday == 1);
    REQUIRE(gmt->tm_hour == 0);
    REQUIRE(gmt->tm_min == 0);
    REQUIRE(gmt->tm_sec == 0);
  }

  SECTION("Parses date-time with milliseconds")
  {
    auto tp = parse_date_time_UTC("01.02.2013 00:00:00.123");

    auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(tp);
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(tp - seconds);

    REQUIRE(millis.count() == 123);

    std::time_t tt = clock::to_time_t(seconds);
    std::tm *gmt = std::gmtime(&tt);

    REQUIRE(gmt != nullptr);
    REQUIRE(gmt->tm_sec == 0);
  }

  SECTION("Ignores trailing whitespace")
  {
    REQUIRE_NOTHROW(parse_date_time_UTC("01.02.2013 00:00:00   "));
    REQUIRE_NOTHROW(parse_date_time_UTC("01.02.2013 00:00:00.000\t\n"));
  }

  SECTION("Rejects trailing non-whitespace characters")
  {
    REQUIRE_THROWS_AS(
        parse_date_time_UTC("01.02.2013 00:00:00abc"),
        std::runtime_error);

    REQUIRE_THROWS_AS(
        parse_date_time_UTC("01.02.2013 00:00:00.000Z"),
        std::runtime_error);
  }

  SECTION("Rejects invalid formats")
  {
    REQUIRE_THROWS_AS(
        parse_date_time_UTC("2013-02-01 00:00:00"),
        std::runtime_error);

    REQUIRE_THROWS_AS(
        parse_date_time_UTC("01.02.2013"),
        std::runtime_error);

    REQUIRE_THROWS_AS(
        parse_date_time_UTC(""),
        std::runtime_error);
  }

  SECTION("UTC parsing is independent of local timezone")
  {
    auto tp = parse_date_time_UTC("01.01.1970 00:00:00");

    REQUIRE(tp.time_since_epoch() == std::chrono::seconds{0});
  }
}


TEST_CASE("format_date_time_UTC formats UTC time_point correctly",
          "[format_date_time_UTC][utc][format]")
{
    using clock = std::chrono::system_clock;

    SECTION("Formats Unix epoch correctly")
    {
        auto tp = clock::time_point{std::chrono::seconds{0}};
        REQUIRE(format_date_time_UTC(tp) == "01.01.1970 00:00:00");
    }

    SECTION("Formats a known historical date")
    {
        // 01.02.2013 00:00:00 UTC
        std::tm tm{};
        tm.tm_year = 2013 - 1900;
        tm.tm_mon  = 1;   // February
        tm.tm_mday = 1;
        tm.tm_hour = 0;
        tm.tm_min  = 0;
        tm.tm_sec  = 0;

#if defined(_WIN32)
        std::time_t tt = _mkgmtime(&tm);
#else
        std::time_t tt = timegm(&tm);
#endif

        auto tp = clock::from_time_t(tt);
        REQUIRE(format_date_time_UTC(tp) == "01.02.2013 00:00:00");
    }

    SECTION("Ignores sub-second precision")
    {
        auto tp = clock::time_point{std::chrono::seconds{0}}
                + std::chrono::milliseconds{999};

        REQUIRE(format_date_time_UTC(tp) == "01.01.1970 00:00:00");
    }

    SECTION("Formats end-of-day boundary correctly")
    {
        // 31.12.1999 23:59:59 UTC
        std::tm tm{};
        tm.tm_year = 1999 - 1900;
        tm.tm_mon  = 11;  // December
        tm.tm_mday = 31;
        tm.tm_hour = 23;
        tm.tm_min  = 59;
        tm.tm_sec  = 59;

#if defined(_WIN32)
        std::time_t tt = _mkgmtime(&tm);
#else
        std::time_t tt = timegm(&tm);
#endif

        auto tp = clock::from_time_t(tt);
        REQUIRE(format_date_time_UTC(tp) == "31.12.1999 23:59:59");
    }

    SECTION("Output format is fixed width with leading zeros")
    {
        // 03.04.2005 06:07:08 UTC
        std::tm tm{};
        tm.tm_year = 2005 - 1900;
        tm.tm_mon  = 3;   // April
        tm.tm_mday = 3;
        tm.tm_hour = 6;
        tm.tm_min  = 7;
        tm.tm_sec  = 8;

#if defined(_WIN32)
        std::time_t tt = _mkgmtime(&tm);
#else
        std::time_t tt = timegm(&tm);
#endif

        auto tp = clock::from_time_t(tt);
        REQUIRE(format_date_time_UTC(tp) == "03.04.2005 06:07:08");
    }
}


// end
