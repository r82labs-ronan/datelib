#pragma once

#include "datelib/date_util.h"

#include <chrono>
#include <unordered_set>

namespace datelib {

// Forward declaration
class HolidayCalendar;

/**
 * @brief Check if a given date is a business day
 * @param date The date to check
 * @param calendar The holiday calendar to use for checking holidays
 * @param weekend_days The set of weekdays considered as weekend (defaults to Saturday and Sunday)
 * @return true if the date is not a weekend day and not a holiday, false otherwise
 * @throws std::invalid_argument if the date is invalid (e.g., February 30th)
 */
bool isBusinessDay(const std::chrono::year_month_day& date, const HolidayCalendar& calendar,
                   const std::unordered_set<std::chrono::weekday, WeekdayHash>& weekend_days = {
                       std::chrono::Saturday, std::chrono::Sunday});

} // namespace datelib
