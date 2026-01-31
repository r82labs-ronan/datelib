#include "datelib/HolidayRule.h"

#include <stdexcept>

#include "catch2/catch.hpp"

using namespace std::chrono;

TEST_CASE("ExplicitDateRule construction", "[HolidayRule]") {
    SECTION("Valid explicit date rules") {
        year_month_day ymd{year{2024}, month{10}, day{31}};
        REQUIRE_NOTHROW(datelib::ExplicitDateRule("Halloween 2024", ymd));
    }

    SECTION("Invalid dates") {
        year_month_day invalid_ymd{year{2024}, month{2}, day{30}};
        REQUIRE_THROWS_AS(datelib::ExplicitDateRule("Invalid", invalid_ymd), std::invalid_argument);
    }
}

TEST_CASE("ExplicitDateRule calculates correct dates", "[HolidayRule]") {
    year_month_day halloween_2024{year{2024}, month{10}, day{31}};
    datelib::ExplicitDateRule halloween("Halloween 2024", halloween_2024);

    SECTION("Returns date for correct year") {
        auto date = halloween.calculateDate(2024);
        REQUIRE(date == halloween_2024);
        REQUIRE(halloween.getName() == "Halloween 2024");
    }

    SECTION("Throws for different year") {
        REQUIRE_THROWS_AS(halloween.calculateDate(2025), std::runtime_error);
    }
}

TEST_CASE("FixedDateRule construction", "[HolidayRule]") {
    SECTION("Valid fixed date rules") {
        REQUIRE_NOTHROW(datelib::FixedDateRule("Christmas", 12, 25));
        REQUIRE_NOTHROW(datelib::FixedDateRule("New Year's Day", 1, 1));
    }

    SECTION("Invalid months") {
        REQUIRE_THROWS_AS(datelib::FixedDateRule("Invalid", 0, 1), std::invalid_argument);
        REQUIRE_THROWS_AS(datelib::FixedDateRule("Invalid", 13, 1), std::invalid_argument);
    }

    SECTION("Invalid days") {
        REQUIRE_THROWS_AS(datelib::FixedDateRule("Invalid", 1, 0), std::invalid_argument);
        REQUIRE_THROWS_AS(datelib::FixedDateRule("Invalid", 1, 32), std::invalid_argument);
    }
}

TEST_CASE("FixedDateRule calculates correct dates", "[HolidayRule]") {
    datelib::FixedDateRule christmas("Christmas", 12, 25);

    SECTION("Calculates for different years") {
        auto date2024 = christmas.calculateDate(2024);
        REQUIRE(date2024 == year_month_day{year{2024}, month{12}, day{25}});

        auto date2025 = christmas.calculateDate(2025);
        REQUIRE(date2025 == year_month_day{year{2025}, month{12}, day{25}});
    }

    SECTION("Returns name") {
        REQUIRE(christmas.getName() == "Christmas");
    }
}

TEST_CASE("NthWeekdayRule construction", "[HolidayRule]") {
    SECTION("Valid Nth weekday rules") {
        REQUIRE_NOTHROW(datelib::NthWeekdayRule("Thanksgiving", 11, 4, 4));
        REQUIRE_NOTHROW(datelib::NthWeekdayRule("Labor Day", 9, 1, 1));
    }

    SECTION("Invalid parameters") {
        REQUIRE_THROWS_AS(datelib::NthWeekdayRule("Invalid", 0, 1, 1), std::invalid_argument);
        REQUIRE_THROWS_AS(datelib::NthWeekdayRule("Invalid", 1, 7, 1), std::invalid_argument);
        REQUIRE_THROWS_AS(datelib::NthWeekdayRule("Invalid", 1, 1, 0), std::invalid_argument);
    }
}

TEST_CASE("NthWeekdayRule calculates correct dates", "[HolidayRule]") {
    SECTION("Thanksgiving - 4th Thursday of November") {
        datelib::NthWeekdayRule thanksgiving("Thanksgiving", 11, 4, 4);

        auto date2024 = thanksgiving.calculateDate(2024);
        REQUIRE(date2024 == year_month_day{year{2024}, month{11}, day{28}});

        // Verify it's a Thursday
        sys_days sd{date2024};
        weekday wd{sd};
        REQUIRE(wd.c_encoding() == 4);
    }

    SECTION("Labor Day - 1st Monday of September") {
        datelib::NthWeekdayRule laborDay("Labor Day", 9, 1, 1);

        auto date2024 = laborDay.calculateDate(2024);
        REQUIRE(date2024 == year_month_day{year{2024}, month{9}, day{2}});

        // Verify it's a Monday
        sys_days sd{date2024};
        weekday wd{sd};
        REQUIRE(wd.c_encoding() == 1);
    }

    SECTION("Memorial Day - Last Monday of May") {
        datelib::NthWeekdayRule memorialDay("Memorial Day", 5, 1, -1);

        auto date2024 = memorialDay.calculateDate(2024);
        REQUIRE(date2024 == year_month_day{year{2024}, month{5}, day{27}});

        // Verify it's a Monday
        sys_days sd{date2024};
        weekday wd{sd};
        REQUIRE(wd.c_encoding() == 1);
    }
}

TEST_CASE("HolidayRule clone", "[HolidayRule]") {
    SECTION("ExplicitDateRule clone") {
        year_month_day ymd{year{2024}, month{10}, day{31}};
        datelib::ExplicitDateRule original("Halloween", ymd);
        auto cloned = original.clone();

        REQUIRE(cloned->getName() == original.getName());
        REQUIRE(cloned->calculateDate(2024) == original.calculateDate(2024));
    }

    SECTION("FixedDateRule clone") {
        datelib::FixedDateRule original("Christmas", 12, 25);
        auto cloned = original.clone();

        REQUIRE(cloned->getName() == original.getName());
        REQUIRE(cloned->calculateDate(2024) == original.calculateDate(2024));
    }

    SECTION("NthWeekdayRule clone") {
        datelib::NthWeekdayRule original("Thanksgiving", 11, 4, 4);
        auto cloned = original.clone();

        REQUIRE(cloned->getName() == original.getName());
        REQUIRE(cloned->calculateDate(2024) == original.calculateDate(2024));
    }
}
