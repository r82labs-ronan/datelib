#pragma once

#include <chrono>
#include <string>

namespace datelib {

// Forward declaration
class HolidayCalendar;

/**
 * @brief Returns a hello world message
 * @return A greeting string
 */
std::string hello_world();

/**
 * @brief Check if a given date is a business day
 * @param date The date to check
 * @param calendar The holiday calendar to use for checking holidays
 * @return true if the date is a weekday (Monday-Friday) and not a holiday, false otherwise
 * @throws std::invalid_argument if the date is invalid (e.g., February 30th)
 */
bool isBusinessDay(const std::chrono::year_month_day& date, const HolidayCalendar& calendar);

} // namespace datelib
