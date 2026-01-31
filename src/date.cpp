#include "datelib/date.h"

#include "datelib/HolidayCalendar.h"

namespace datelib {

std::string hello_world() {
    return "Hello, World!";
}

bool isBusinessDay(const std::chrono::year_month_day& date, const HolidayCalendar& calendar) {
    // Validate the date is well-formed
    if (!date.ok()) {
        throw std::invalid_argument("Invalid date provided to isBusinessDay");
    }

    // Convert to sys_days to get the weekday
    auto sys_days_date = std::chrono::sys_days{date};
    std::chrono::weekday wd{sys_days_date};

    // Check if it's a weekday (Sunday=0, Monday=1, ..., Saturday=6)
    // Weekdays are Monday(1) through Friday(5)
    bool is_weekday = (wd != std::chrono::Sunday && wd != std::chrono::Saturday);

    // A business day is a weekday that is not a holiday
    return is_weekday && !calendar.isHoliday(date);
}

} // namespace datelib
