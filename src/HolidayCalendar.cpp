#include "datelib/HolidayCalendar.h"

#include <algorithm>

namespace datelib {

using namespace std::chrono;

HolidayCalendar::HolidayCalendar(const HolidayCalendar& other) {
    // Deep copy the rules
    rules_.reserve(other.rules_.size());
    for (const auto& rule : other.rules_) {
        rules_.push_back(rule->clone());
    }
}

HolidayCalendar& HolidayCalendar::operator=(const HolidayCalendar& other) {
    if (this != &other) {
        // Deep copy the rules
        rules_.clear();
        rules_.reserve(other.rules_.size());
        for (const auto& rule : other.rules_) {
            rules_.push_back(rule->clone());
        }
    }
    return *this;
}

void HolidayCalendar::addHoliday(const std::string& name, const year_month_day& date) {
    rules_.push_back(std::make_unique<ExplicitDateRule>(name, date));
}

void HolidayCalendar::addRule(std::unique_ptr<HolidayRule> rule) {
    if (rule) {
        rules_.push_back(std::move(rule));
    }
}

bool HolidayCalendar::isHoliday(const year_month_day& date) const {
    int year = static_cast<int>(date.year());
    for (const auto& rule : rules_) {
        try {
            year_month_day ruleDate = rule->calculateDate(year);
            if (ruleDate == date) {
                return true;
            }
        } catch (...) {
            // Rule might not be applicable for this year, skip it
            continue;
        }
    }

    return false;
}

std::vector<year_month_day> HolidayCalendar::getHolidays(int year) const {
    std::vector<year_month_day> holidays;

    // Collect all holidays from rules
    for (const auto& rule : rules_) {
        try {
            year_month_day ruleDate = rule->calculateDate(year);
            holidays.push_back(ruleDate);
        } catch (...) {
            // Rule might not be applicable for this year, skip it
            continue;
        }
    }

    // Sort and remove duplicates
    std::sort(holidays.begin(), holidays.end());
    holidays.erase(std::unique(holidays.begin(), holidays.end()), holidays.end());

    return holidays;
}

std::vector<std::string> HolidayCalendar::getHolidayNames(const year_month_day& date) const {
    std::vector<std::string> names;

    int year = static_cast<int>(date.year());
    for (const auto& rule : rules_) {
        try {
            year_month_day ruleDate = rule->calculateDate(year);
            if (ruleDate == date) {
                names.push_back(rule->getName());
            }
        } catch (...) {
            // Rule might not be applicable for this year, skip it
            continue;
        }
    }

    return names;
}

void HolidayCalendar::clear() {
    rules_.clear();
}

} // namespace datelib
