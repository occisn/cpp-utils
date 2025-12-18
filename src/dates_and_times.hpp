#ifndef DATES_AND_TIMES_HPP
#define DATES_AND_TIMES_HPP

#include <chrono>
#include <string>

std::chrono::system_clock::time_point parse_date_time_UTC(const std::string& str);
std::string format_date_time_UTC(const std::chrono::system_clock::time_point &tp);
void print_date_time_UTC(const std::chrono::system_clock::time_point &tp);

#endif // DATES_AND_TIMES_HPP
