### Narrow character support for UTF8 encoding

In the Linux and web worlds, `UTF-8` is the dominant character encoding.

Note that (at least in MSVS) you cannot open a Windows file with a Unicode name using the standard 
```cpp
std::fstream fs(const char* filename)
```
Instead you need to use the non standard Microsoft extension
```cpp
std::fstream fs(const wchar_t* filename)
```

#### Unicode escaping
```cpp
string inputStr("[\"\\u0040\\u0040\\u0000\\u0011\"]");
std::cout << "Input:    " << inputStr << std::endl;

json arr = json::parse(inputStr);
std::string str = arr[0].as<std::string>();
std::cout << "Hex dump: [";
for (std::size_t i = 0; i < str.size(); ++i)
{
    unsigned int val = static_cast<unsigned int>(str[i]);
    if (i != 0)
    {
        std::cout << " ";
    }
    std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << val;
}
std::cout << "]" << std::endl;

std::ostringstream os;
os << arr;
std::cout << "Output:   " << os.str() << std::endl;
```

Output:

```
Input:    ["\u0040\u0040\u0000\u0011"]
Hex dump: [0x40 0x40 0x00 0x11]
Output:   ["@@\u0000\u0011"]
```
Note that just the two control characters are escaped on output.

#### Reading escaped unicode into utf8 encodings and writing back escaped unicode
```cpp
string inputStr("[\"\\u007F\\u07FF\\u0800\"]");
std::cout << "Input:    " << inputStr << std::endl;

json arr = json::parse(inputStr);
std::string s = arr[0].as<string>();
std::cout << "Hex dump: [";
for (std::size_t i = 0; i < s.size(); ++i)
{
    if (i != 0)
        std::cout << " ";
    unsigned int u(s[i] >= 0 ? s[i] : 256 + s[i] );
    std::cout << "0x"  << std::hex<< std::setfill('0') << std::setw(2) << u;
}
std::cout << "]" << std::endl;

std::ostringstream os;
auto options = json_options{}
    .escape_all_non_ascii(true);
os << print(arr,options);
std::string outputStr = os.str();
std::cout << "Output:   " << os.str() << std::endl;

json arr2 = json::parse(outputStr);
std::string s2 = arr2[0].as<string>();
std::cout << "Hex dump: [";
for (std::size_t i = 0; i < s2.size(); ++i)
{
    if (i != 0)
        std::cout << " ";
    unsigned int u(s2[i] >= 0 ? s2[i] : 256 + s2[i] );
    std::cout << "0x"  << std::hex<< std::setfill('0') << std::setw(2) << u;
}
std::cout << "]" << std::endl;
```

Output:

```
Input:    ["\u007F\u07FF\u0800"]
Hex dump: [0x7f 0xdf 0xbf 0xe0 0xa0 0x80]
Output:   ["\u007F\u07FF\u0800"]
Hex dump: [0x7f 0xdf 0xbf 0xe0 0xa0 0x80]
```
Since the escaped unicode consists of a control character (0x7f) and non-ascii, we get back the same text as what we started with.

#### Reading escaped unicode into utf8 encodings and writing back escaped unicode (with continuations)
```cpp
string input = "[\"\\u8A73\\u7D30\\u95B2\\u89A7\\uD800\\uDC01\\u4E00\"]";
json value = json::parse(input);
auto options = json_options{}
    .escape_all_non_ascii(true);
string output;
value.dump(output,options);

std::cout << "Input:" << std::endl;
std::cout << input << std::endl;
std::cout << std::endl;
std::cout << "Output:" << std::endl;
std::cout << output << std::endl;
```
Since all of the escaped unicode is non-ascii, we get back the same text as what we started with.
```
Input:
["\u8A73\u7D30\u95B2\u89A7\uD800\uDC01\u4E00"]

Output:
["\u8A73\u7D30\u95B2\u89A7\uD800\uDC01\u4E00"]
```

