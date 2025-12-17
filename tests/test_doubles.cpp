#include <cmath>
#include <limits>
#include <string>
#include <catch_amalgamated.hpp>
#include "doubles.hpp"

TEST_CASE("parse_double handles various numeric formats and edge cases", "[parse_double]") {
    
    SECTION("Basic valid cases with comma separator") {
        REQUIRE_THAT(parse_double("1,36115", ','), 
                     Catch::Matchers::WithinRel(1.36115, 0.00001));
        REQUIRE_THAT(parse_double("29059,0996", ','), 
                     Catch::Matchers::WithinRel(29059.0996, 0.0001));
        REQUIRE_THAT(parse_double("0,5", ','), 
                     Catch::Matchers::WithinRel(0.5, 0.00001));
        REQUIRE_THAT(parse_double("42", ','), 
                     Catch::Matchers::WithinRel(42.0, 0.00001));
    }
    
    SECTION("Basic valid cases with dot separator") {
        REQUIRE_THAT(parse_double("1.36115", '.'), 
                     Catch::Matchers::WithinRel(1.36115, 0.00001));
        REQUIRE_THAT(parse_double("29059.0996", '.'), 
                     Catch::Matchers::WithinRel(29059.0996, 0.0001));
        REQUIRE_THAT(parse_double("0.5", '.'), 
                     Catch::Matchers::WithinRel(0.5, 0.00001));
        REQUIRE_THAT(parse_double("42", '.'), 
                     Catch::Matchers::WithinRel(42.0, 0.00001));
    }
    
    SECTION("Negative numbers") {
        REQUIRE_THAT(parse_double("-1,36115", ','), 
                     Catch::Matchers::WithinRel(-1.36115, 0.00001));
        REQUIRE_THAT(parse_double("-29059.0996", '.'), 
                     Catch::Matchers::WithinRel(-29059.0996, 0.0001));
        REQUIRE_THAT(parse_double("-42", '.'), 
                     Catch::Matchers::WithinRel(-42.0, 0.00001));
    }
    
    SECTION("Zero and special zero values") {
        REQUIRE_THAT(parse_double("0", '.'), 
                     Catch::Matchers::WithinRel(0.0, 0.00001));
        REQUIRE_THAT(parse_double("0.0", '.'), 
                     Catch::Matchers::WithinRel(0.0, 0.00001));
        REQUIRE_THAT(parse_double("0,0", ','), 
                     Catch::Matchers::WithinRel(0.0, 0.00001));
        REQUIRE_THAT(parse_double("-0.0", '.'), 
                     Catch::Matchers::WithinRel(-0.0, 0.00001));
    }
    
    SECTION("Positive sign") {
        REQUIRE_THAT(parse_double("+1,36115", ','), 
                     Catch::Matchers::WithinRel(1.36115, 0.00001));
        REQUIRE_THAT(parse_double("+1.36115", '.'), 
                     Catch::Matchers::WithinRel(1.36115, 0.00001));
    }
    
    SECTION("Scientific notation") {
        REQUIRE_THAT(parse_double("1.5e10", '.'), 
                     Catch::Matchers::WithinRel(1.5e10, 0.00001));
        REQUIRE_THAT(parse_double("1.5E10", '.'), 
                     Catch::Matchers::WithinRel(1.5E10, 0.00001));
        REQUIRE_THAT(parse_double("1,5e10", ','), 
                     Catch::Matchers::WithinRel(1.5e10, 0.00001));
        REQUIRE_THAT(parse_double("1.5e-10", '.'), 
                     Catch::Matchers::WithinRel(1.5e-10, 0.00001));
    }
    
    SECTION("Leading and trailing whitespace") {
        REQUIRE_THAT(parse_double("  1.5", '.'), 
                     Catch::Matchers::WithinRel(1.5, 0.00001));
        REQUIRE_THAT(parse_double("1.5  ", '.'), 
                     Catch::Matchers::WithinRel(1.5, 0.00001));
        REQUIRE_THAT(parse_double("  1.5  ", '.'), 
                     Catch::Matchers::WithinRel(1.5, 0.00001));
    }
    
    SECTION("High precision decimals") {
        REQUIRE_THAT(parse_double("3.141592653589793", '.'), 
                     Catch::Matchers::WithinRel(3.141592653589793, 0.000000000000001));
        REQUIRE_THAT(parse_double("3,141592653589793", ','), 
                     Catch::Matchers::WithinRel(3.141592653589793, 0.000000000000001));
    }
    
    SECTION("Very large and very small numbers") {
        REQUIRE_THAT(parse_double("1e308", '.'), 
                     Catch::Matchers::WithinRel(1e308, 0.00001));
        // Use a value within the valid range (min is ~2.225e-308)
        REQUIRE_THAT(parse_double("2.5e-308", '.'), 
                     Catch::Matchers::WithinRel(2.5e-308, 0.00001));
        REQUIRE_THAT(parse_double("1e-300", '.'), 
                     Catch::Matchers::WithinRel(1e-300, 0.00001));
    }
    
    SECTION("Invalid input throws std::invalid_argument") {
        REQUIRE_THROWS_AS(parse_double("", '.'), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_double("abc", '.'), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_double(".", '.'), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_double(",", ','), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_double("1.2.3", '.'), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_double("1,2,3", ','), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_double("1,5", '.'), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_double("123abc", '.'), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_double("12.3xyz", '.'), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_double("1.2$", '.'), std::invalid_argument);
    }
    
    SECTION("Out of range throws std::out_of_range") {
        REQUIRE_THROWS_AS(parse_double("1e309", '.'), std::out_of_range);
        REQUIRE_THROWS_AS(parse_double("-1e309", '.'), std::out_of_range);
    }
    
    SECTION("Separator replacement behavior") {
        REQUIRE_THAT(parse_double("100", ','), 
                     Catch::Matchers::WithinRel(100.0, 0.00001));
        REQUIRE_THAT(parse_double("1.5", ','), 
                     Catch::Matchers::WithinRel(1.5, 0.00001));
    }
    
    SECTION("Boundary values") {
        std::string maxDouble = std::to_string(std::numeric_limits<double>::max());
        REQUIRE_NOTHROW(parse_double(maxDouble, '.'));
        
        std::string minDouble = std::to_string(std::numeric_limits<double>::min());
        REQUIRE_NOTHROW(parse_double(minDouble, '.'));
        
        std::string lowestDouble = std::to_string(std::numeric_limits<double>::lowest());
        REQUIRE_NOTHROW(parse_double(lowestDouble, '.'));
    }
    
    SECTION("Infinity representation") {
        double result = parse_double("inf", '.');
        REQUIRE(std::isinf(result));
        REQUIRE(result > 0);
        
        result = parse_double("-inf", '.');
        REQUIRE(std::isinf(result));
        REQUIRE(result < 0);
        
        result = parse_double("infinity", '.');
        REQUIRE(std::isinf(result));
    }
    
    SECTION("NaN representation") {
        double result = parse_double("nan", '.');
        REQUIRE(std::isnan(result));
        
        result = parse_double("nan(123)", '.');
        REQUIRE(std::isnan(result));
    }
}
// end
