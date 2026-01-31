#include "datelib/date.h"

#include "datelib/HolidayCalendar.h"

namespace datelib {

bool isBusinessDay(const std::chrono::year_month_day& date, const HolidayCalendar& calendar,
                   const std::unordered_set<unsigned>& weekend_days) {
    // Validate the date is well-formed
    if (!date.ok()) {
        throw std::invalid_argument("Invalid date provided to isBusinessDay");
    }

    // Convert to sys_days to get the weekday
    auto sys_days_date = std::chrono::sys_days{date};
    std::chrono::weekday wd{sys_days_date};

    // Check if the day is not a weekend day
    // weekday encoding: Sunday=0, Monday=1, ..., Saturday=6
    bool is_not_weekend = (weekend_days.find(wd.c_encoding()) == weekend_days.end());

    // A business day is not a weekend day and not a holiday
    return is_not_weekend && !calendar.isHoliday(date);
}

} // namespace datelib
