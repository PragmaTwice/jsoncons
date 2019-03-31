// Copyright 2017 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSON_CONVERSION_HPP
#define JSONCONS_JSON_CONVERSION_HPP

#include <iostream>
#include <string>
#include <tuple>
#include <memory>
#include <istream> // std::basic_istream
#include <jsoncons/json_conversion_traits.hpp>
#include <jsoncons/json_pull_reader.hpp>

namespace jsoncons {

// decode_json 

template <class T, class CharT>
T decode_json(const std::basic_string<CharT>& s)
{
    basic_json_pull_reader<CharT> reader(s);
    T val;
    decode_stream(basic_json<CharT>(), reader, val);
    return val;
}

template <class T, class CharT>
T decode_json(const std::basic_string<CharT>& s,
              const basic_json_decode_options<CharT>& options)
{
    basic_json_pull_reader<CharT> reader(s, options);
    T val;
    decode_stream(basic_json<CharT>(), reader, val);
    return val;
}

template <class T, class CharT>
T decode_json(std::basic_istream<CharT>& is)
{
    basic_json_pull_reader<CharT> reader(is);
    T val;
    decode_stream(basic_json<CharT>(), reader, val);
    return val;
}

template <class T, class CharT>
T decode_json(std::basic_istream<CharT>& is,
              const basic_json_decode_options<CharT>& options)
{
    basic_json_pull_reader<CharT> reader(is, options);
    T val;
    decode_stream(basic_json<CharT>(), reader, val);
    return val;
}

template <class T, class CharT, class ImplementationPolicy, class Allocator>
T decode_json(const std::basic_string<CharT>& s, const basic_json<CharT,ImplementationPolicy,Allocator>& j)
{
    basic_json_pull_reader<CharT> reader(s);
    T val;
    decode_stream(j, reader, val);
    return val;
}

template <class T, class CharT, class ImplementationPolicy, class Allocator>
T decode_json(const std::basic_string<CharT>& s,
              const basic_json_decode_options<CharT>& options, 
              const basic_json<CharT,ImplementationPolicy,Allocator>& j)
{
    basic_json_pull_reader<CharT> reader(s, options);
    T val;
    decode_stream(j, reader, val);
    return val;
}

template <class T, class CharT, class ImplementationPolicy, class Allocator>
T decode_json(std::basic_istream<CharT>& is, const basic_json<CharT,ImplementationPolicy,Allocator>& j)
{
    basic_json_pull_reader<CharT> reader(is);
    T val;
    decode_stream(j, reader, val);
    return val;
}

template <class T, class CharT, class ImplementationPolicy, class Allocator>
T decode_json(std::basic_istream<CharT>& is,
              const basic_json_decode_options<CharT>& options, 
              const basic_json<CharT,ImplementationPolicy,Allocator>& j)
{
    basic_json_pull_reader<CharT> reader(is, options);
    T val;
    decode_stream(j, reader, val);
    return val;
}

// encode_json

template <class T, class CharT>
void encode_json(const T& val, 
                 basic_json_content_handler<CharT>& receiver)
{
    encode_stream(basic_json<CharT>(), val, receiver);
    receiver.flush();
}

// to stream

template <class T, class CharT>
void encode_json(const T& val,
                 std::basic_ostream<CharT>& os, 
                 const basic_json_encode_options<CharT>& options = basic_json_options<CharT>(), 
                 indenting line_indent = indenting::no_indent)
{
    if (line_indent == indenting::indent)
    {
        basic_json_encoder<CharT> encoder(os, options);
        encode_json(val, encoder);
    }
    else
    {
        basic_json_compressed_encoder<CharT> encoder(os, options);
        encode_json(val, encoder);
    }
}

template <class T, class CharT>
void encode_json(const T& val, 
                 std::basic_ostream<CharT>& os, 
                 indenting line_indent)
{
    encode_json(val, os, basic_json_options<CharT>(), line_indent);
}

// to string

template <class T, class CharT>
void encode_json(const T& val, 
                 std::basic_string<CharT>& s, 
                 indenting line_indent = indenting::no_indent)
{
    encode_json(val, s, basic_json_options<CharT>(), line_indent);
}

template <class T, class CharT>
void encode_json(const T& val,
                 std::basic_string<CharT>& s, 
                 const basic_json_encode_options<CharT>& options, 
                 indenting line_indent = indenting::no_indent)
{
    if (line_indent == indenting::indent)
    {
        basic_json_encoder<CharT,jsoncons::string_result<std::basic_string<CharT>>> encoder(s, options);
        encode_json(val, encoder);
    }
    else
    {
        basic_json_compressed_encoder<CharT, jsoncons::string_result<std::basic_string<CharT>>> encoder(s, options);
        encode_json(val, encoder);
    }
}

template <class T, class CharT, class ImplementationPolicy, class Allocator>
void encode_json(const basic_json<CharT, ImplementationPolicy, Allocator>& j, 
                 const T& val,
                 basic_json_content_handler<CharT>& receiver)
{
    encode_stream(j, val, receiver);
    receiver.flush();
}

template <class T, class CharT, class ImplementationPolicy, class Allocator>
void encode_json(const basic_json<CharT,ImplementationPolicy,Allocator>& j,
                 const T& val,
                 std::basic_ostream<CharT>& os, 
                 const basic_json_encode_options<CharT>& options = basic_json_options<CharT>(), 
                 indenting line_indent = indenting::no_indent)
{
    if (line_indent == indenting::indent)
    {
        basic_json_encoder<CharT> encoder(os, options);
        encode_json(j, val, encoder);
    }
    else
    {
        basic_json_compressed_encoder<CharT> encoder(os, options);
        encode_json(j, val, encoder);
    }
}

template <class T, class CharT, class ImplementationPolicy, class Allocator>
void encode_json(const basic_json<CharT,ImplementationPolicy,Allocator>& j,
                 const T& val,
                 std::basic_ostream<CharT>& os, 
                 indenting line_indent)
{
    if (line_indent == indenting::indent)
    {
        basic_json_encoder<CharT> encoder(os, basic_json_options<CharT>());
        encode_json(j, val, encoder);
    }
    else
    {
        basic_json_compressed_encoder<CharT> encoder(os, basic_json_options<CharT>());
        encode_json(j, val, encoder);
    }
}

template <class T, class CharT, class ImplementationPolicy, class Allocator>
void encode_json(const basic_json<CharT,ImplementationPolicy,Allocator>& j,
                 const T& val,
                 std::basic_string<CharT>& s, 
                 const basic_json_encode_options<CharT>& options = basic_json_options<CharT>(), 
                 indenting line_indent = indenting::no_indent)
{ 
    if (line_indent == indenting::indent)
    {
        basic_json_encoder<CharT,jsoncons::string_result<std::basic_string<CharT>>> encoder(s, options);
        encode_json(j, val, encoder);
    }
    else
    {
        basic_json_compressed_encoder<CharT,jsoncons::string_result<std::basic_string<CharT>>> encoder(s, options);
        encode_json(j, val, encoder);
    }
}

template <class T, class CharT, class ImplementationPolicy, class Allocator>
void encode_json(const basic_json<CharT,ImplementationPolicy,Allocator>& j,
                 const T& val,
                 std::basic_string<CharT>& s, 
                 indenting line_indent)
{ 
    if (line_indent == indenting::indent)
    {
        basic_json_encoder<CharT,jsoncons::string_result<std::basic_string<CharT>>> encoder(s, basic_json_options<CharT>());
        encode_json(j, val, encoder);
    }
    else
    {
        basic_json_compressed_encoder<CharT,jsoncons::string_result<std::basic_string<CharT>>> encoder(s, basic_json_options<CharT>());
        encode_json(j, val, encoder);
    }
}

}

#endif

