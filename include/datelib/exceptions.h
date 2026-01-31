#pragma once

#include <stdexcept>
#include <string>

namespace datelib {

/**
 * @brief Exception thrown when unable to find a business day within a reasonable range
 */
class BusinessDaySearchException : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};

} // namespace datelib
