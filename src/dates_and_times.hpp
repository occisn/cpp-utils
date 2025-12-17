#ifndef DATES_AND_TIMES_HPP
#define DATES_AND_TIMES_HPP

#include <chrono>
#include <string>

std::chrono::system_clock::time_point parse_date_time(const std::string& str);

#endif // DATES_AND_TIMES_HPP
