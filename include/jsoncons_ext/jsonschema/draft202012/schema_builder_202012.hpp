// Copyright 2013-2023 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSONSCHEMA_DRAFT202012_SCHEMA_BUILDER_202012_HPP
#define JSONCONS_JSONSCHEMA_DRAFT202012_SCHEMA_BUILDER_202012_HPP

#include <jsoncons/uri.hpp>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpointer/jsonpointer.hpp>
#include <jsoncons_ext/jsonschema/common/compilation_context.hpp>
#include <jsoncons_ext/jsonschema/json_schema.hpp>
#include <jsoncons_ext/jsonschema/common/keyword_validators.hpp>
#include <jsoncons_ext/jsonschema/common/schema_builder.hpp>
#include <jsoncons_ext/jsonschema/draft202012/schema_draft202012.hpp>
#include <cassert>
#include <set>
#include <sstream>
#include <iostream>
#include <cassert>
#if defined(JSONCONS_HAS_STD_REGEX)
#include <regex>
#endif

namespace jsoncons {
namespace jsonschema {
namespace draft202012 {

    template <class Json>
    class schema_builder_202012 : public schema_builder<Json> 
    {
    public:
        using keyword_validator_type = typename std::unique_ptr<keyword_validator<Json>>;
        using schema_validator_pointer = schema_validator<Json>*;
        using schema_validator_type = typename std::unique_ptr<schema_validator<Json>>;
        using dynamic_ref_validator_type = dynamic_ref_validator<Json>;

        using keyword_factory_type = std::function<keyword_validator_type(const compilation_context& context, const Json& sch, const Json& parent)>;

        std::unordered_map<std::string,keyword_factory_type> keyword_factory_map_;

    public:
        schema_builder_202012(const uri_resolver<Json>& resolver) noexcept
            : schema_builder<Json>(resolver)
        {
            init();
        }

        schema_builder_202012(const schema_builder_202012&) = delete;
        schema_builder_202012& operator=(const schema_builder_202012&) = delete;
        schema_builder_202012(schema_builder_202012&&) = default;
        schema_builder_202012& operator=(schema_builder_202012&&) = default;

        void init()
        {
            keyword_factory_map_.emplace("type", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_type_validator(context, sch);});
            keyword_factory_map_.emplace("contentEncoding", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_content_encoding_validator(context, sch);});
            keyword_factory_map_.emplace("contentMediaType", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_content_media_type_validator(context, sch);});
            keyword_factory_map_.emplace("format", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_format_validator(context, sch);});
#if defined(JSONCONS_HAS_STD_REGEX)
            keyword_factory_map_.emplace("pattern", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_pattern_validator(context, sch);});
#endif
            keyword_factory_map_.emplace("maxItems", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_max_items_validator(context, sch);});
            keyword_factory_map_.emplace("minItems", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_min_items_validator(context, sch);});
            keyword_factory_map_.emplace("contains", 
                [&](const compilation_context& context, const Json& sch, const Json& parent){return this->make_contains_validator(context, sch, parent);});
            keyword_factory_map_.emplace("uniqueItems", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_unique_items_validator(context, sch);});
            keyword_factory_map_.emplace("maxLength", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_max_length_validator(context, sch);});
            keyword_factory_map_.emplace("minLength", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_min_length_validator(context, sch);});
            keyword_factory_map_.emplace("not", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_not_validator(context, sch);});
            keyword_factory_map_.emplace("maximum", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_maximum_validator(context, sch);});
            keyword_factory_map_.emplace("exclusiveMaximum", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_exclusive_maximum_validator(context, sch);});
            keyword_factory_map_.emplace("minimum", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_minimum_validator(context, sch);});
            keyword_factory_map_.emplace("exclusiveMinimum", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_exclusive_minimum_validator(context, sch);});
            keyword_factory_map_.emplace("multipleOf", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_multiple_of_validator(context, sch);});
            keyword_factory_map_.emplace("const", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_const_validator(context, sch);});
            keyword_factory_map_.emplace("enum", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_enum_validator(context, sch);});
            keyword_factory_map_.emplace("allOf", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_all_of_validator(context, sch);});
            keyword_factory_map_.emplace("anyOf", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_any_of_validator(context, sch);});
            keyword_factory_map_.emplace("oneOf", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_one_of_validator(context, sch);});
            keyword_factory_map_.emplace("dependencies", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_dependencies_validator(context, sch);});
            keyword_factory_map_.emplace("propertyNames", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_property_names_validator(context, sch);});
            keyword_factory_map_.emplace("required", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_required_validator(context, sch);});
            keyword_factory_map_.emplace("dependentRequired", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_dependent_required_validator(context, sch);});
            keyword_factory_map_.emplace("dependentSchemas", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_dependent_schemas_validator(context, sch);});
            keyword_factory_map_.emplace("unevaluatedProperties", 
                [&](const compilation_context& context, const Json& sch, const Json&){return this->make_unevaluated_properties_validator(context, sch);});
        }

        const char* schema_version() const noexcept final
        {
            return "https://json-schema.org/draft/2019-09/schema";
        }

        schema_validator_type make_schema_validator(const compilation_context& context, 
            const Json& sch, jsoncons::span<const std::string> keys) override
        {
            auto new_context = make_compilation_context(context, sch, keys);
            //std::cout << "make_schema_validator " << context.get_absolute_uri().string() << ", " << new_context.get_absolute_uri().string() << "\n\n";

            schema_validator_type schema_validator_ptr;

            switch (sch.type())
            {
                case json_type::bool_value:
                {
                    uri schema_path = new_context.get_absolute_uri();
                    if (sch.template as<bool>())
                    {
                        schema_validator_ptr = jsoncons::make_unique<boolean_schema_validator<Json>>( 
                            schema_path, sch.template as<bool>());
                    }
                    else
                    {
                        schema_validator_ptr = jsoncons::make_unique<boolean_schema_validator<Json>>( 
                          schema_path, false);
                    }
                    schema_validator<Json>* p = schema_validator_ptr.get();
                    for (const auto& uri : new_context.uris()) 
                    { 
                        this->insert_schema(uri, p);
                    }          
                    break;
                }
                case json_type::object_value:
                {
                    std::set<std::string> known_keywords;

                    auto it = sch.find("definitions");
                    if (it != sch.object_range().end()) 
                    {
                        for (const auto& def : it->value().object_range())
                        {
                            std::string sub_keys[] = { "definitions", def.key() };
                            this->save_schema(make_schema_validator(
                                new_context, def.value(), sub_keys));
                        }
                        known_keywords.insert("definitions");
                    }
                    it = sch.find("$defs");
                    if (it != sch.object_range().end()) 
                    {
                        for (const auto& def : it->value().object_range())
                        {
                            std::string sub_keys[] = { "$defs", def.key() };
                            this->save_schema(make_schema_validator(
                                new_context, def.value(), sub_keys));
                        }
                        known_keywords.insert("definitions");
                    }
                    schema_validator_ptr = make_object_schema_validator(new_context, sch);
                    schema_validator<Json>* p = schema_validator_ptr.get();
                    for (const auto& uri : new_context.uris()) 
                    { 
                        this->insert_schema(uri, p);
                        for (const auto& item : sch.object_range())
                        {
                            if (known_keywords.find(item.key()) == known_keywords.end())
                            {
                                this->insert_unknown_keyword(uri, item.key(), item.value()); // save unknown keywords for later reference
                            }
                        }
                    }          
                    break;
                }
                default:
                    JSONCONS_THROW(schema_error("invalid JSON-type for a schema for " + new_context.get_absolute_uri().string() + ", expected: boolean or object"));
                    break;
            }
            
            return schema_validator_ptr;
        }

        schema_validator_type make_object_schema_validator( 
            const compilation_context& context, const Json& sch)
        {
            Json default_value{ jsoncons::null_type() };
            std::vector<keyword_validator_type> validators;
            std::set<std::string> known_keywords;
            std::unique_ptr<dynamic_anchor_validator<Json>> dynamic_anchor;

            auto it = sch.find("$dynamicAnchor"); 
            if (it != sch.object_range().end()) 
            {
                std::string value = it->value().template as<std::string>();
                jsoncons::uri new_uri(context.get_base_uri(), uri_fragment_part, value);

                uri_wrapper relative("$dynamicAnchor");
                auto id = relative.resolve(uri_wrapper{ context.get_base_uri() });
                dynamic_anchor = jsoncons::make_unique<dynamic_anchor_validator<Json>>(id.uri(),
                    new_uri);
            }

            it = sch.find("default");
            if (it != sch.object_range().end()) 
            {
                default_value = it->value();
                known_keywords.insert("default");
            }

            it = sch.find("$ref");
            if (it != sch.object_range().end()) // this schema has a reference
            {
                uri_wrapper relative(it->value().template as<std::string>()); 
                auto id = relative.resolve(uri_wrapper{ context.get_base_uri() });
                validators.push_back(this->get_or_create_reference(id));
            }

            it = sch.find("$dynamicRef");
            if (it != sch.object_range().end()) // this schema has a reference
            {
                std::string value = it->value().template as<std::string>();
                uri_wrapper relative(value); 
                auto id = relative.resolve(uri_wrapper{ context.get_base_uri() });
                auto orig = jsoncons::make_unique<dynamic_ref_validator_type>(id.uri().base(), id.uri());
                validators.push_back(std::move(orig));
            }

            for (const auto& key_value : sch.object_range())
            {
                auto factory_it = keyword_factory_map_.find(key_value.key());
                if (factory_it != keyword_factory_map_.end())
                {
                    auto validator = factory_it->second(context, key_value.value(), sch);
                    if (validator)
                    {   
                        validators.emplace_back(std::move(validator));
                    }
                }
            }

            it = sch.find("if");
            if (it != sch.object_range().end()) 
            {
                validators.push_back(make_conditional_validator(context, it->value(), sch));
                known_keywords.insert("if");
                // sch["if"] is object and has id, can be looked up
            }
            else
            {
                auto then_it = sch.find("then");
                if (then_it != sch.object_range().end()) 
                {
                    std::string sub_keys[] = { "then" };
                    this->save_schema(make_schema_validator(context, then_it->value(), sub_keys));
                    known_keywords.insert("then");
                }

                auto else_it = sch.find("else");
                if (else_it != sch.object_range().end()) 
                {
                    std::string sub_keys[] = { "else" };
                    this->save_schema(make_schema_validator(context, else_it->value(), sub_keys));
                    known_keywords.insert("else");
                }
            }

            // Object validators

            std::unique_ptr<properties_validator<Json>> properties;
            it = sch.find("properties");
            if (it != sch.object_range().end()) 
            {
                properties = make_properties_validator(context, it->value());
            }
            std::unique_ptr<pattern_properties_validator<Json>> pattern_properties;

    #if defined(JSONCONS_HAS_STD_REGEX)
            it = sch.find("patternProperties");
            if (it != sch.object_range().end())
            {
                pattern_properties = make_pattern_properties_validator(context, it->value());
            }
    #endif

            it = sch.find("additionalProperties");
            if (it != sch.object_range().end()) 
            {
                validators.emplace_back(make_additional_properties_validator(context, it->value(), 
                    std::move(properties), std::move(pattern_properties)));
            }
            else
            {
                if (properties)
                {
                    validators.emplace_back(std::move(properties));
                }
#if defined(JSONCONS_HAS_STD_REGEX)
                if (pattern_properties)
                {
                    //validators.emplace_back(std::move(pattern_properties));
                }
#endif
            }

            it = sch.find("prefixItems");
            if (it != sch.object_range().end()) 
            {

                if (it->value().type() == json_type::array_value) 
                {
                    validators.emplace_back(make_prefix_items_validator(context, sch, it->value()));
                } 
                else if (it->value().type() == json_type::object_value ||
                           it->value().type() == json_type::bool_value)
                {
                    validators.emplace_back(make_items_object_validator(context, sch, it->value()));
                }
            }

            it = sch.find("items");
            if (it != sch.object_range().end()) 
            {
                if (it->value().type() == json_type::object_value || it->value().type() == json_type::bool_value)
                {
                    validators.emplace_back(make_items_object_validator(context, sch, it->value()));
                }
            }

            
            return jsoncons::make_unique<object_schema_validator<Json>>(
                context.get_absolute_uri(),
                std::move(validators), std::move(default_value), std::move(dynamic_anchor));
        }

        std::unique_ptr<prefix_items_validator<Json>> make_prefix_items_validator(const compilation_context& context, 
            const Json& parent, const Json& sch)
        {
            std::vector<schema_validator_type> item_validators;
            schema_validator_type additional_items_validator = nullptr;

            uri schema_path{context.make_schema_path_with("prefixItems")};

            if (sch.type() == json_type::array_value) 
            {
                size_t c = 0;
                for (const auto& subsch : sch.array_range())
                {
                    std::string sub_keys[] = {"prefixItems", std::to_string(c++)};

                    item_validators.emplace_back(make_schema_validator(context, subsch, sub_keys));
                }

                auto it = parent.find("items");
                if (it != parent.object_range().end()) 
                {
                    std::string sub_keys[] = {"items"};
                    additional_items_validator = make_schema_validator(context, it->value(), sub_keys);
                }
            }

            return jsoncons::make_unique<prefix_items_validator<Json>>( schema_path, 
                std::move(item_validators), std::move(additional_items_validator));
        }

        std::unique_ptr<items_object_validator<Json>> make_items_object_validator(const compilation_context& context, 
            const Json& /* parent */, const Json& sch)
        {
            uri schema_path{context.make_schema_path_with("items")};

            std::string sub_keys[] = {"items"};

            return jsoncons::make_unique<items_object_validator<Json>>( schema_path, 
                make_schema_validator(context, sch, sub_keys));
        }

        std::unique_ptr<conditional_validator<Json>> make_conditional_validator(const compilation_context& context,
            const Json& sch_if, const Json& sch)
        {
            std::string schema_path = context.get_absolute_uri().string();
            schema_validator_type if_validator(nullptr);
            schema_validator_type then_validator(nullptr);
            schema_validator_type else_validator(nullptr);

            std::string if_key[] = { "if" };
            if_validator = make_schema_validator(context, sch_if, if_key);

            auto then_it = sch.find("then");
            if (then_it != sch.object_range().end()) 
            {
                std::string then_key[] = { "then" };
                then_validator = make_schema_validator(context, then_it->value(), then_key);
            }

            auto else_it = sch.find("else");
            if (else_it != sch.object_range().end()) 
            {
                std::string else_key[] = { "else" };
                else_validator = make_schema_validator(context, else_it->value(), else_key);
            }

            return jsoncons::make_unique<conditional_validator<Json>>( std::move(schema_path),
                std::move(if_validator), std::move(then_validator), std::move(else_validator));
        }
                
        std::unique_ptr<properties_validator<Json>> make_properties_validator(const compilation_context& context, 
            const Json& sch)
        {
            uri schema_path = context.get_absolute_uri();
            std::map<std::string, schema_validator_type> properties;

            for (const auto& prop : sch.object_range())
            {
                std::string sub_keys[] =
                {"properties", prop.key()};
                properties.emplace(std::make_pair(prop.key(), 
                    make_schema_validator(context, prop.value(), sub_keys)));
            }

            return jsoncons::make_unique<properties_validator<Json>>(std::move(schema_path), 
                std::move(properties));
        }

#if defined(JSONCONS_HAS_STD_REGEX)
                
        std::unique_ptr<pattern_properties_validator<Json>> make_pattern_properties_validator( const compilation_context& context, 
            const Json& sch)
        {
            uri schema_path = context.get_absolute_uri();
            std::vector<std::pair<std::regex, schema_validator_type>> pattern_properties;
            
            for (const auto& prop : sch.object_range())
            {
                std::string sub_keys[] = {prop.key()};
                pattern_properties.emplace_back(
                    std::make_pair(
                        std::regex(prop.key(), std::regex::ECMAScript),
                        make_schema_validator(context, prop.value(), sub_keys)));
            }

            return jsoncons::make_unique<pattern_properties_validator<Json>>( std::move(schema_path),
                std::move(pattern_properties));
        }
#endif       

        std::unique_ptr<additional_properties_validator<Json>> make_additional_properties_validator( 
            const compilation_context& context, const Json& sch, 
            std::unique_ptr<properties_validator<Json>>&& properties, std::unique_ptr<pattern_properties_validator<Json>>&& pattern_properties)
        {
            uri schema_path = context.get_absolute_uri();
            std::vector<keyword_validator_type> validators;
            schema_validator_type additional_properties;

            std::string sub_keys[] = {"additionalProperties"};
            additional_properties = make_schema_validator(context, sch, sub_keys);

            return jsoncons::make_unique<additional_properties_validator<Json>>( std::move(schema_path),
                std::move(properties), std::move(pattern_properties),
                std::move(additional_properties));
        }

    private:

        compilation_context make_compilation_context(const compilation_context& parent, 
            const Json& sch, jsoncons::span<const std::string> keys) const override
        {
            // Exclude uri's that are not plain name identifiers
            std::vector<uri_wrapper> new_uris;
            for (const auto& uri : parent.uris())
            {
                if (!uri.has_plain_name_fragment())
                {
                    new_uris.push_back(uri);
                }
            }

            if (new_uris.empty())
            {
                new_uris.emplace_back("#");
            }

            // Append the keys for this sub-schema to the uri's
            for (const auto& key : keys)
            {
                for (auto& uri : new_uris)
                {
                    auto new_u = uri.append(key);
                    uri = uri_wrapper(new_u);
                }
            }
            if (sch.is_object())
            {
                auto it = sch.find("$id"); // If $id is found, this schema can be referenced by the id
                if (it != sch.object_range().end()) 
                {
                    std::string id = it->value().template as<std::string>(); 
                    uri_wrapper relative(id); 
                    if (relative.has_fragment())
                    {
                        JSONCONS_THROW(schema_error("Draft 2019-09 does not allow $id with fragment"));
                    }
                    uri_wrapper new_uri = relative.resolve(uri_wrapper{ parent.get_base_uri() });
                    //std::cout << "$id: " << id << ", " << new_uri.string() << "\n";
                    // Add it to the list if it is not already there
                    if (std::find(new_uris.begin(), new_uris.end(), new_uri) == new_uris.end())
                    {
                        new_uris.emplace_back(new_uri); 
                    }
                }
                it = sch.find("$anchor"); 
                if (it != sch.object_range().end()) 
                {
                    auto value = it->value().template as<std::string>();
                    if (validate_anchor(value))
                    {
                        auto uri = !new_uris.empty() ? new_uris.back().uri() : jsoncons::uri{"#"};
                        jsoncons::uri new_uri(uri, uri_fragment_part, value);
                        uri_wrapper identifier{ new_uri };
                        if (std::find(new_uris.begin(), new_uris.end(), identifier) == new_uris.end())
                        {
                            new_uris.emplace_back(std::move(identifier)); 
                        }
                    }
                }
            }
/*
            std::cout << "Absolute URI: " << parent.get_absolute_uri().string() << "\n";
            for (const auto& uri : new_uris)
            {
                std::cout << "    " << uri.string() << "\n";
            }
*/
            return compilation_context(new_uris);
        }

        static bool validate_anchor(const std::string& s)
        {
            if (s.empty())
            {
                return false;
            }
            if (!((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')))
            {
                return false;
            }

            for (std::size_t i = 1; i < s.size(); ++i)
            {
                switch (s[i])
                {
                    case '-':
                    case '_':
                    case ':':
                    case '.':
                        break;
                    default:
                        if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9')))
                        {
                            return false;
                        }
                        break;
                }
            }
            return true;
        }

    };

} // namespace draft202012
} // namespace jsonschema
} // namespace jsoncons

#endif // JSONCONS_JSONSCHEMA_DRAFT7_KEYWORD_FACTORY_HPP
