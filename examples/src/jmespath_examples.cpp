// Copyright 2013 Daniel Parker
// Distributed under Boost license

#include <string>
#include <fstream>
#include <cassert>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jmespath/jmespath.hpp>

// for brevity
using jsoncons::json; 
namespace jmespath = jsoncons::jmespath;

namespace {

    void search_example() 
    {
        std::string doc = R"(
        {
          "locations": [
            {"name": "Seattle", "state": "WA"},
            {"name": "New York", "state": "NY"},
            {"name": "Bellevue", "state": "WA"},
            {"name": "Olympia", "state": "WA"}
          ]
        }        
        )";

        std::string expr = R"(
          locations[?state == 'WA'].name | sort(@) | {WashingtonCities: join(', ', @)}
        )";

        json jdoc = json::parse(doc);

        json result = jmespath::search(jdoc, expr);

        std::cout << pretty_print(result) << "\n\n";
    }

    void jmespath_expression_example()
    { 
        std::string doc = R"(
        {
          "locations": [
            {"name": "Seattle", "state": "WA"},
            {"name": "New York", "state": "NY"},
            {"name": "Bellevue", "state": "WA"},
            {"name": "Olympia", "state": "WA"}
          ]
        }        
        )";

        std::string s = R"(
          locations[?state == 'WA'].name | sort(@) | {WashingtonCities: join(', ', @)}
        )";

        auto expr = jmespath::jmespath_expression::compile(s);

        auto jdoc = json::parse(doc);

        auto result = expr.evaluate(jdoc);

        std::cout << pretty_print(result) << "\n\n";
    }

} // namespace

void jmespath_examples()
{
    std::cout << "\nJMESPath examples\n\n";
    search_example();
    jmespath_expression_example();

    std::cout << "\n";
}
