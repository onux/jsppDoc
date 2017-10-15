//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_DOCCOMMENTTAGS_H
#define JSPP_DOCGEN_DOCCOMMENTTAGS_H

#include <string>
#include <vector>
#include <memory>

namespace jspp
{
namespace docgen
{
    struct Parameter;
    struct Example;
    struct SeeAlso;

    struct DocCommentTags
    {
        std::string summary;
        std::string overload_name;
        std::string return_info;
        bool isDeprecated = false;
        std::string deprecated_reason;
        std::vector<std::shared_ptr<Parameter>> params;
        std::vector<std::shared_ptr<Example>> examples;
        std::vector<std::shared_ptr<SeeAlso>> see_also;
    };

    /**
     * Used for representing the JS++ \@param documentation tag.
     */
    struct Parameter
    {
        std::string name;
        std::string description;

        Parameter(const std::string& name, const std::string& description)
            :
            name(name),
            description(description)
        {
        }
    };

    /**
     * Used for representing the JS++ \@example documentation tag.
     */
    struct Example
    {
        std::string title;
        std::string code;

        Example(const std::string& title, const std::string& code)
            :
            title(title),
            code(code)
        {
        }
    };

    /**
     * Used for representing the JS++ \@see documentation tag.
     */
    struct SeeAlso
    {
        std::string title;
        std::string path;

        SeeAlso(const std::string& title, const std::string& path)
            :
            title(title),
            path(path)
        {
        }
    };
}
}

#endif