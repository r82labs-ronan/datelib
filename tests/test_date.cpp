#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "datelib/date.h"

TEST_CASE("hello_world returns greeting", "[hello]") {
    std::string result = datelib::hello_world();
    REQUIRE(result == "Hello, World!");
}
