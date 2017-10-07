//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_UTILS_H
#define JSPP_DOCGEN_UTILS_H

#include <vector>
#include <string>

namespace jspp
{
namespace docgen
{
namespace utils
{
    /**
     * Joins a vector of strings into a string with an optionally-specified delimiter.
     */
    std::string join(const std::vector<std::string>& v, const std::string& delimiter = "");
    /**
     * Splits a string based on the specified delimiter.
     */
    std::vector<std::string> split(const std::string& s, const std::string& delimiter);
    /**
     * Platform-agnostic line splitting for UNIX/Windows/MacOS9 line endings.
     */
    std::vector<std::string> splitLines(const std::string& s);
    /**
     * Removes whitespace from the beginning and end of a string.
     */
    std::string trimWhitespace(const std::string &s);
    /**
     * Removes empty lines from the beginning and end of a string vector.
     */
    void trimWhitespace(std::vector<std::string>& v);
}
}
}

#endif