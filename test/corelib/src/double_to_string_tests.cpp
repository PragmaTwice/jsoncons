// Copyright 2013-2024 Daniel Parker
// Distributed under Boost license

#include <jsoncons/json.hpp>
#include <jsoncons/json_encoder.hpp>
#include <jsoncons/extension_traits.hpp>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>
#include <catch/catch.hpp>

using namespace jsoncons;

template <typename CharT>
std::basic_string<CharT> float_to_string(double val, int precision)
{
    jsoncons::detail::write_double print(float_chars_format::general,precision);

    std::basic_string<CharT> s;
    jsoncons::string_sink<std::basic_string<CharT>> writer(s);
    print(val, writer);
    writer.flush();
    return s;
}

TEST_CASE("test_double_to_string")
{
    double x = 1.0e100;
    std::string s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    //std::cout << x << ": " << s << std::endl;
    CHECK(s == std::string("1e+100"));

    x = 1.0e-100;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("1e-100"));

    x = 0.123456789e-100;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("1.23456789e-101"));

    x = 0.123456789e100;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("1.23456789e+99"));

    x = 1234563;
    s = float_to_string<char>(x, 6);
    CHECK(s == std::string("1.23456e+06"));

    x = 0.0000001234563;
    s = float_to_string<char>(x, 6);
    CHECK(s == std::string("1.23456e-07"));

    x = -1.0e+100;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    std::cout << s << "\n";
    CHECK(s == std::string("-1e+100"));

    x = -1.0e-100;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("-1e-100"));

    x = 0;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("0.0"));

    x = -0;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("0.0"));

    x = 1;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("1.0"));

    x = 0.1;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("0.1"));

    x = 1.1;
    s = float_to_string<char>(x, 17);
    CHECK(s == std::string("1.1000000000000001"));

    x = -1;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("-1.0"));

    x = 10;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("10.0"));

    x = -10;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("-10.0"));

    x = 11;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("11.0"));

    x = -11;
    s = float_to_string<char>(x, std::numeric_limits<double>::digits10);
    CHECK(s == std::string("-11.0"));
}


