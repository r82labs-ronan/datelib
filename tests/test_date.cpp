#define CATCH_CONFIG_MAIN
#include "datelib/HolidayCalendar.h"
#include "datelib/date.h"

#include "catch2/catch.hpp"

using namespace std::chrono;

TEST_CASE("hello_world returns greeting", "[hello]") {
    std::string result = datelib::hello_world();
    REQUIRE(result == "Hello, World!");
}

TEST_CASE("isBusinessDay with empty calendar", "[isBusinessDay]") {
    datelib::HolidayCalendar emptyCalendar;

    SECTION("Weekdays are business days") {
        // Monday, January 1, 2024
        REQUIRE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{1}}, emptyCalendar));

        // Tuesday, January 2, 2024
        REQUIRE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{2}}, emptyCalendar));

        // Wednesday, January 3, 2024
        REQUIRE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{3}}, emptyCalendar));

        // Thursday, January 4, 2024
        REQUIRE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{4}}, emptyCalendar));

        // Friday, January 5, 2024
        REQUIRE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{5}}, emptyCalendar));
    }

    SECTION("Weekends are not business days") {
        // Saturday, January 6, 2024
        REQUIRE_FALSE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{6}}, emptyCalendar));

        // Sunday, January 7, 2024
        REQUIRE_FALSE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{7}}, emptyCalendar));
    }
}

TEST_CASE("isBusinessDay with holidays", "[isBusinessDay]") {
    datelib::HolidayCalendar calendar;

    // Add some fixed holidays
    calendar.addRule(std::make_unique<datelib::FixedDateRule>("New Year's Day", 1, 1));
    calendar.addRule(std::make_unique<datelib::FixedDateRule>("Independence Day", 7, 4));
    calendar.addRule(std::make_unique<datelib::FixedDateRule>("Christmas", 12, 25));

    SECTION("Holidays on weekdays are not business days") {
        // Monday, January 1, 2024 (New Year's Day)
        REQUIRE_FALSE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{1}}, calendar));

        // Thursday, July 4, 2024 (Independence Day)
        REQUIRE_FALSE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{7}, day{4}}, calendar));

        // Wednesday, December 25, 2024 (Christmas)
        REQUIRE_FALSE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{12}, day{25}}, calendar));
    }

    SECTION("Non-holiday weekdays are business days") {
        // Tuesday, January 2, 2024
        REQUIRE(datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{2}}, calendar));

        // Friday, July 5, 2024
        REQUIRE(datelib::isBusinessDay(year_month_day{year{2024}, month{7}, day{5}}, calendar));
    }

    SECTION("Weekends are still not business days") {
        // Saturday, January 6, 2024
        REQUIRE_FALSE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{6}}, calendar));

        // Sunday, January 7, 2024
        REQUIRE_FALSE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{7}}, calendar));
    }
}

TEST_CASE("isBusinessDay with Nth weekday holidays", "[isBusinessDay]") {
    datelib::HolidayCalendar calendar;

    // Add Thanksgiving (4th Thursday of November)
    calendar.addRule(std::make_unique<datelib::NthWeekdayRule>("Thanksgiving", 11, 4,
                                                               datelib::Occurrence::Fourth));

    SECTION("Thanksgiving 2024 is not a business day") {
        // Thursday, November 28, 2024 (Thanksgiving)
        REQUIRE_FALSE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{11}, day{28}}, calendar));
    }

    SECTION("Other Thursdays in November are business days") {
        // Thursday, November 7, 2024
        REQUIRE(datelib::isBusinessDay(year_month_day{year{2024}, month{11}, day{7}}, calendar));

        // Thursday, November 21, 2024
        REQUIRE(datelib::isBusinessDay(year_month_day{year{2024}, month{11}, day{21}}, calendar));
    }
}

TEST_CASE("isBusinessDay with explicit date holidays", "[isBusinessDay]") {
    datelib::HolidayCalendar calendar;

    // Add a one-time holiday
    calendar.addHoliday("Company Anniversary", year_month_day{year{2024}, month{6}, day{15}});

    SECTION("Explicit holiday on weekday is not a business day") {
        // Saturday, June 15, 2024 (Company Anniversary)
        REQUIRE_FALSE(
            datelib::isBusinessDay(year_month_day{year{2024}, month{6}, day{15}}, calendar));
    }

    SECTION("Same date in different year is a business day") {
        // Monday, June 15, 2026 (not a holiday)
        REQUIRE(datelib::isBusinessDay(year_month_day{year{2026}, month{6}, day{15}}, calendar));
    }
}

TEST_CASE("isBusinessDay real-world scenario", "[isBusinessDay]") {
    datelib::HolidayCalendar usHolidays;

    // Add US federal holidays
    usHolidays.addRule(std::make_unique<datelib::FixedDateRule>("New Year's Day", 1, 1));
    usHolidays.addRule(std::make_unique<datelib::FixedDateRule>("Independence Day", 7, 4));
    usHolidays.addRule(std::make_unique<datelib::FixedDateRule>("Christmas", 12, 25));
    usHolidays.addRule(
        std::make_unique<datelib::NthWeekdayRule>("Memorial Day", 5, 1, datelib::Occurrence::Last));
    usHolidays.addRule(
        std::make_unique<datelib::NthWeekdayRule>("Labor Day", 9, 1, datelib::Occurrence::First));
    usHolidays.addRule(std::make_unique<datelib::NthWeekdayRule>("Thanksgiving", 11, 4,
                                                                 datelib::Occurrence::Fourth));

    SECTION("Verify business days in a typical week") {
        // Week of January 2-8, 2024 (New Year's on Monday Jan 1)
        REQUIRE_FALSE(datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{1}},
                                             usHolidays)); // Monday - New Year's
        REQUIRE(datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{2}},
                                       usHolidays)); // Tuesday
        REQUIRE(datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{3}},
                                       usHolidays)); // Wednesday
        REQUIRE(datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{4}},
                                       usHolidays)); // Thursday
        REQUIRE(datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{5}},
                                       usHolidays)); // Friday
        REQUIRE_FALSE(datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{6}},
                                             usHolidays)); // Saturday
        REQUIRE_FALSE(datelib::isBusinessDay(year_month_day{year{2024}, month{1}, day{7}},
                                             usHolidays)); // Sunday
    }
}
