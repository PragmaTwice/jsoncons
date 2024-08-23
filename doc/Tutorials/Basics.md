## Examples

The examples below illustrate the use of the [json](../ref/basic_json.md) class and [json_query](../ref/jsonpath/json_query.md) function.

### json construction

```cpp
#include <jsoncons/json.hpp>

// For convenience
using jsoncons::json;

// Construct a book object
json book1;

book1["category"] = "Fiction";
book1["title"] = "A Wild Sheep Chase: A Novel";
book1["author"] = "Haruki Murakami";
book1["date"] = "2002-04-09";
book1["price"] = 9.01;
book1["isbn"] = "037571894X";  

// Construct another using the insert_or_assign function
json book2;

book2.insert_or_assign("category", "History");
book2.insert_or_assign("title", "Charlie Wilson's War");
book2.insert_or_assign("author", "George Crile");
book2.insert_or_assign("date", "2007-11-06");
book2.insert_or_assign("price", 10.50);
book2.insert_or_assign("isbn", "0802143415");  

// Use insert_or_assign again, but more efficiently
json book3;

// Reserve memory, to avoid reallocations
book3.reserve(6);

// Insert in name alphabetical order
// Give insert_or_assign a hint where to insert the next member
auto hint = book3.insert_or_assign(book3.object_range().begin(),"author", "Haruki Murakami");
hint = book3.insert_or_assign(hint, "category", "Fiction");
hint = book3.insert_or_assign(hint, "date", "2006-01-03");
hint = book3.insert_or_assign(hint, "isbn", "1400079276");  
hint = book3.insert_or_assign(hint, "price", 13.45);
hint = book3.insert_or_assign(hint, "title", "Kafka on the Shore");

// Construct a fourth from a string
json book4 = json::parse(R"(
{
    "category" : "Fiction",
    "title" : "Pulp",
    "author" : "Charles Bukowski",
    "date" : "2004-07-08",
    "price" : 22.48,
    "isbn" : "1852272007"  
}
)");

// Construct a booklist array

json booklist(json_array_arg);

// For efficiency, reserve memory, to avoid reallocations
booklist.reserve(4);

// For efficency, tell jsoncons to move the contents 
// of the four book objects into the array
booklist.add(std::move(book1));    
booklist.add(std::move(book2));    

// Add the third book to the front
auto pos = booklist.add(booklist.array_range().begin(),std::move(book3));

// and the last one immediately after
booklist.add(pos+1,std::move(book4));    

// See what's left of book1, 2, 3 and 4 (expect nulls)
std::cout << book1 << "," << book2 << "," << book3 << "," << book4 << std::endl;


++
//Loop through the booklist elements using a range-based for loop    
for (const auto& book : booklist.array_range())
{
    std::cout << book["title"].as<std::string>()
              << ","
              << book["price"].as<double>() << std::endl;
}

// The second book
json& book = booklist[1];

//Loop through the book's name-value pairs using a range-based for loop    
for (const auto& member : book.object_range())
{
    std::cout << member.key()
              << ","
              << member.value() << std::endl;
}

auto it = book.find("author");
if (it != book.object_range().end())
{
    // member "author" found
}

if (book.contains("author"))
{
    // book has a member "author"
}

book.get("author", "author unknown").as<std::string>();
// Returns author if found, otherwise "author unknown"

try
{
    book["ratings"].as<std::string>();
}
catch (const std::out_of_range& e)
{
    // member "ratings" not found
}

// Add ratings
book["ratings"]["*****"] = 4;
book["ratings"]["*"] = 2;

// Delete one-star ratings
book["ratings"].erase("*");

```
```cpp  
    // Serialize the booklist to a file
    std::ofstream os("store.json");
    os << pretty_print(booklist);
```

The JSON output `store.json`
```json
[
    {
        "author":"Haruki Murakami",
        "category":"Fiction",
        "date":"2006-01-03",
        "isbn":"1400079276",
        "price":13.45,
        "title":"Kafka on the Shore"
    },
    {
        "author":"Charles Bukowski",
        "category":"Fiction",
        "date":"2004-07-08",
        "isbn":"1852272007",
        "price":22.48,
        "ratings":
        {
            "*****":4
        },
        "title":"Pulp"
    },
    {
        "author":"Haruki Murakami",
        "category":"Fiction",
        "date":"2002-04-09",
        "isbn":"037571894X",
        "price":9.01,
        "title":"A Wild Sheep Chase: A Novel"
    },
    {
        "author":"George Crile",
        "category":"History",
        "date":"2007-11-06",
        "isbn":"0802143415",
        "price":10.5,
        "title":"Charlie Wilson's War"
    }
]
```
### json query

```cpp
#include <fstream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpath/jsonpath.hpp>

// For convenience
using jsoncons::json;
using jsoncons::jsonpath::json_query;

// Deserialize the booklist
std::ifstream is("store.json");
json booklist;
is >> booklist;

// Use a JSONPath expression to find 
//  
// (1) The authors of books that cost less than $12
json result = json_query(booklist, "$[*][?(@.price < 12)].author");
std::cout << result << std::endl;

// (2) The number of books
result = json_query(booklist, "$.length");
std::cout << result << std::endl;

// (3) The third book
result = json_query(booklist, "$[2]");
std::cout << std::endl << pretty_print(result) << std::endl;

// (4) The authors of books that were published in 2004
result = json_query(booklist, "$[*][?(@.date =~ /2004.*?/)].author");
std::cout << result << std::endl;

// (5) The titles of all books that have ratings
result = json_query(booklist, "$[*][?(@.ratings)].title");
std::cout << result << std::endl;

// (6) All authors and titles of books
result = json_query(booklist, "$..['author','title']");
std::cout << pretty_print(result) << std::endl;
```
Result:
```json
(1) ["Haruki Murakami","George Crile"]
(2) [4]
(3)
[
    {
        "author":"Haruki Murakami",
        "category":"Fiction",
        "date":"2002-04-09",
        "isbn":"037571894X",
        "price":9.01,
        "title":"A Wild Sheep Chase: A Novel"
    }
]
(4) ["Charles Bukowski"]
(5) ["Pulp"]
(6) 
[
    "Nigel Rees",
    "Sayings of the Century",
    "Evelyn Waugh",
    "Sword of Honour",
    "Herman Melville",
    "Moby Dick",
    "J. R. R. Tolkien",
    "The Lord of the Rings"
]
```

