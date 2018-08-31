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
     * Returns 'true' if the provided character is a whitespace character.
     */
    bool isWhitespace(const char32_t c);
    /**
     * Removes whitespace from the beginning and end of a string.
     */
    std::string trimWhitespace(const std::string &s);
    /**
     * Removes whitespace from the beginning and end of each line of a string vector.
     */
    void trimWhitespace(std::vector<std::string>& v);
    /**
     * Removes empty lines from the beginning and end of a string vector.
     */
    void trimEmptyLines(std::vector<std::string>& v);
    /**
     * Trims leading whitespace from each line based on the whitespace of the
     * first non-empty line.
     */
    void trimLeading(std::vector<std::string>& lines);
    /**
     * Escapes XML entities.
     */
    std::string escapeXML(const std::string &s);
    /**
     * Truncate a string until the sentences of the string are at least `count`
     * characters.
     *
     * @param s The string to truncate.
     * @param count The minimum characters of the resulting truncated string.
     */
    std::string truncate(const std::string& s, size_t count);
}
}
}

#endif