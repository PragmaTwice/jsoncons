// Copyright 2013-2024 Daniel Parker
// Distributed under Boost license

#if defined(_MSC_VER)
#include "windows.h" // test no inadvertant macro expansions
#endif
#include <catch/catch.hpp>
#include <jsoncons/json.hpp>
#include <vector>

using jsoncons::json_type_traits;
using jsoncons::json;
using jsoncons::decode_json;
using jsoncons::encode_json;

namespace encode_traits_tests {

    struct book
    {
        std::string author;
        std::string title;
        double price;

        book() = default;
        book(const book&) = default;
        book(book&&) = default;

        book(const std::string& author, const std::string& title, double price)
            : author(author), title(title), price(price)
        {
        }
    };
} // namespace encode_traits_tests

namespace ns = encode_traits_tests;

JSONCONS_ALL_MEMBER_TRAITS(ns::book,author,title,price)

TEST_CASE("decode_traits std::pair tests")
{
    SECTION("test 1")
    {
        auto p = std::make_pair<int,std::string>(1,"foo");

        std::string buf;
        encode_json(p,buf);

        auto p2 = decode_json<std::pair<int,std::string>>(buf);

        CHECK(p2 == p);
    }
    SECTION("test 2")
    {
        auto p = std::make_pair<int,std::string>(1,"foo");

        std::string buf;
        encode_json(p,buf);

        auto p2 = decode_json<std::pair<int,std::string>>(buf);

        CHECK(p2 == p);
    }
    SECTION("test 3")
    {
        ns::book book{"Haruki Murakami","Kafka on the Shore",25.17};

        auto p = std::make_pair(1,book);

        std::string buf;
        encode_json(p,buf);

        auto p2 = decode_json<std::pair<int,ns::book>>(buf);

        CHECK(p2.second.author == book.author);
        CHECK(p2.second.title == book.title);
        CHECK(p2.second.price == book.price);
    }
}

