#include <catch_amalgamated.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "dates_and_times.hpp"

// Helper function to extract date/time components from time_point
struct DateTime {
    int day, month, year, hour, minute, second, millisecond;
    
    static DateTime from_time_point(const std::chrono::system_clock::time_point& tp) {
        auto timeT = std::chrono::system_clock::to_time_t(tp);
        std::tm tm = *std::localtime(&timeT);
        
        auto millisSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch()).count();
        int millis = millisSinceEpoch % 1000;
        
        return {tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
                tm.tm_hour, tm.tm_min, tm.tm_sec, millis};
    }
};

TEST_CASE("parse_date_time handles various datetime formats and edge cases", "[parse_date_time]") {
    
    SECTION("Valid datetime with milliseconds") {
        auto tp = parse_date_time("01.02.2013 00:00:00.000");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.day == 1);
        REQUIRE(dt.month == 2);
        REQUIRE(dt.year == 2013);
        REQUIRE(dt.hour == 0);
        REQUIRE(dt.minute == 0);
        REQUIRE(dt.second == 0);
        REQUIRE(dt.millisecond == 0);
    }
    
    SECTION("Valid datetime without milliseconds") {
        auto tp = parse_date_time("15.03.2020 14:30:45");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.day == 15);
        REQUIRE(dt.month == 3);
        REQUIRE(dt.year == 2020);
        REQUIRE(dt.hour == 14);
        REQUIRE(dt.minute == 30);
        REQUIRE(dt.second == 45);
    }
    
    SECTION("Datetime with non-zero milliseconds") {
        auto tp = parse_date_time("10.06.2021 12:34:56.789");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.day == 10);
        REQUIRE(dt.month == 6);
        REQUIRE(dt.year == 2021);
        REQUIRE(dt.hour == 12);
        REQUIRE(dt.minute == 34);
        REQUIRE(dt.second == 56);
        REQUIRE(dt.millisecond == 789);
    }
    
    SECTION("Beginning of day - midnight") {
        auto tp = parse_date_time("01.01.2020 00:00:00.000");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.hour == 0);
        REQUIRE(dt.minute == 0);
        REQUIRE(dt.second == 0);
        REQUIRE(dt.millisecond == 0);
    }
    
    SECTION("End of day - 23:59:59") {
        auto tp = parse_date_time("31.12.2019 23:59:59.999");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.day == 31);
        REQUIRE(dt.month == 12);
        REQUIRE(dt.year == 2019);
        REQUIRE(dt.hour == 23);
        REQUIRE(dt.minute == 59);
        REQUIRE(dt.second == 59);
        REQUIRE(dt.millisecond == 999);
    }
    
    SECTION("Leap year - February 29th") {
        auto tp = parse_date_time("29.02.2020 10:20:30.000");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.day == 29);
        REQUIRE(dt.month == 2);
        REQUIRE(dt.year == 2020);
    }
    
    SECTION("First day of year") {
        auto tp = parse_date_time("01.01.2000 06:30:00.500");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.day == 1);
        REQUIRE(dt.month == 1);
        REQUIRE(dt.year == 2000);
    }
    
    SECTION("Last day of year") {
        auto tp = parse_date_time("31.12.2025 18:45:30.123");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.day == 31);
        REQUIRE(dt.month == 12);
        REQUIRE(dt.year == 2025);
    }
    
    SECTION("Various months - 30 days") {
        auto tp = parse_date_time("30.04.2022 12:00:00.000");
        auto dt = DateTime::from_time_point(tp);
        REQUIRE(dt.day == 30);
        REQUIRE(dt.month == 4);
        
        tp = parse_date_time("30.09.2022 12:00:00.000");
        dt = DateTime::from_time_point(tp);
        REQUIRE(dt.day == 30);
        REQUIRE(dt.month == 9);
    }
    
    SECTION("Various months - 31 days") {
        auto tp = parse_date_time("31.01.2022 12:00:00.000");
        auto dt = DateTime::from_time_point(tp);
        REQUIRE(dt.day == 31);
        REQUIRE(dt.month == 1);
        
        tp = parse_date_time("31.08.2022 12:00:00.000");
        dt = DateTime::from_time_point(tp);
        REQUIRE(dt.day == 31);
        REQUIRE(dt.month == 8);
    }
    
    SECTION("Single digit day and month with leading zeros") {
        auto tp = parse_date_time("05.07.2018 09:05:03.001");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.day == 5);
        REQUIRE(dt.month == 7);
        REQUIRE(dt.year == 2018);
        REQUIRE(dt.hour == 9);
        REQUIRE(dt.minute == 5);
        REQUIRE(dt.second == 3);
        REQUIRE(dt.millisecond == 1);
    }
    
    SECTION("Different century - 1990s") {
        auto tp = parse_date_time("15.08.1995 11:22:33.444");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.day == 15);
        REQUIRE(dt.month == 8);
        REQUIRE(dt.year == 1995);
    }
    
    SECTION("Far future date") {
        auto tp = parse_date_time("25.12.2099 20:15:10.555");
        auto dt = DateTime::from_time_point(tp);
        
        REQUIRE(dt.day == 25);
        REQUIRE(dt.month == 12);
        REQUIRE(dt.year == 2099);
    }
    
    SECTION("Empty string throws exception") {
        REQUIRE_THROWS_AS(parse_date_time(""), std::runtime_error);
    }
    
    SECTION("Invalid format - missing components") {
        REQUIRE_THROWS_AS(parse_date_time("01.02.2013"), std::runtime_error);
        REQUIRE_THROWS_AS(parse_date_time("01.02.2013 12:30"), std::runtime_error);
        REQUIRE_THROWS_AS(parse_date_time("12:30:45"), std::runtime_error);
    }
    
    SECTION("Invalid format - wrong separators") {
        REQUIRE_THROWS_AS(parse_date_time("01/02/2013 12:30:45"), std::runtime_error);
        REQUIRE_THROWS_AS(parse_date_time("01-02-2013 12:30:45"), std::runtime_error);
        REQUIRE_THROWS_AS(parse_date_time("01.02.2013 12-30-45"), std::runtime_error);
    }
    
    SECTION("Non-numeric characters") {
        REQUIRE_THROWS_AS(parse_date_time("abc.def.ghij kl:mn:op"), std::runtime_error);
        REQUIRE_THROWS_AS(parse_date_time("01.02.ABCD 12:30:45"), std::runtime_error);
    }
    
    SECTION("Trailing characters after valid datetime") {
        REQUIRE_THROWS_AS(parse_date_time("01.02.2013 12:30:45.000 extra"), std::runtime_error);
        REQUIRE_THROWS_AS(parse_date_time("01.02.2013 12:30:45xyz"), std::runtime_error);
        REQUIRE_THROWS_AS(parse_date_time("01.02.2013 12:30:45.000abc"), std::runtime_error);
    }
    
    SECTION("Whitespace variations") {
        // Trailing whitespace is allowed
        REQUIRE_NOTHROW(parse_date_time("01.02.2013 12:30:45 "));
        REQUIRE_NOTHROW(parse_date_time("01.02.2013 12:30:45.000  "));
        REQUIRE_NOTHROW(parse_date_time("01.02.2013 12:30:45\t"));
        
        // sscanf treats single space in format as "any whitespace"
        // So multiple spaces between date and time are accepted (this is standard behavior)
        REQUIRE_NOTHROW(parse_date_time("01.02.2013  12:30:45"));
        REQUIRE_NOTHROW(parse_date_time("01.02.2013   12:30:45"));
        
        // Leading whitespace is also accepted by sscanf (%d skips leading whitespace)
        REQUIRE_NOTHROW(parse_date_time(" 01.02.2013 12:30:45"));
        REQUIRE_NOTHROW(parse_date_time("  01.02.2013 12:30:45"));
    }
    
    SECTION("Consistency check - parse and re-parse") {
        std::string original = "17.11.2023 15:45:30.250";
        auto tp1 = parse_date_time(original);
        
        // Convert back to string (simplified format without milliseconds)
        auto timeT = std::chrono::system_clock::to_time_t(tp1);
        std::tm tm = *std::localtime(&timeT);
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << tm.tm_mday << "."
            << std::setfill('0') << std::setw(2) << (tm.tm_mon + 1) << "."
            << (tm.tm_year + 1900) << " "
            << std::setfill('0') << std::setw(2) << tm.tm_hour << ":"
            << std::setfill('0') << std::setw(2) << tm.tm_min << ":"
            << std::setfill('0') << std::setw(2) << tm.tm_sec;
        
        auto tp2 = parse_date_time(oss.str());
        
        // Compare at second precision (milliseconds might differ)
        auto t1 = std::chrono::system_clock::to_time_t(tp1);
        auto t2 = std::chrono::system_clock::to_time_t(tp2);
        REQUIRE(t1 == t2);
    }
}

// end
