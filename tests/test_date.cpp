#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "datelib/date.h"

using namespace datelib;

TEST_CASE("hello_world returns greeting", "[hello]") {
    std::string result = hello_world();
    REQUIRE(result == "Hello, World!");
}
