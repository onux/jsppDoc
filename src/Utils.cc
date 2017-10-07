#include "Utils.h"

#include <sstream>
#include <iterator>
#include <algorithm>
#include <regex>

std::string jspp::docgen::utils::join(const std::vector<std::string>& v, const std::string& delimiter/* = ""*/) {
    std::ostringstream ss;

    std::copy(v.begin(), v.end() - 1, std::ostream_iterator<std::string>(ss, delimiter.c_str()));
    if (v.size() != 0) {
        ss << v.back();
    }

    return ss.str();
}

std::vector<std::string> jspp::docgen::utils::split(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> result;

    if (delimiter.size() == 0) {
        return { s };
    }

    size_t searchIndex = 0, lastSearchIndex = 0, offset = delimiter.size();
    while ((searchIndex = s.find_first_of(delimiter, searchIndex + offset)) != std::string::npos) {
        size_t substr_start = lastSearchIndex == 0 ? 0 : lastSearchIndex + offset;
        size_t substr_len = searchIndex - substr_start;

        result.push_back(s.substr(substr_start, substr_len));

        lastSearchIndex = searchIndex;
    }

    size_t  lastDelimiter = lastSearchIndex + offset,
            endOfString = s.size() - lastDelimiter;
    result.push_back(s.substr(lastDelimiter, endOfString));

    return result;
}

std::vector<std::string> jspp::docgen::utils::splitLines(const std::string& s) {
    if (s.size() == 0) return {};

    std::vector<std::string> lines;

    const std::regex re(R"(\r\n|\n|\r)");
    std::sregex_token_iterator it(s.begin(), s.end(), re, -1);
    std::sregex_token_iterator end;

    for (; it != end; ++it) {
        lines.push_back(it->str());
    }

    trimWhitespace(lines);

    return lines;
}

std::string jspp::docgen::utils::trimWhitespace(const std::string &s) {
    if (s.size() == 0) return "";

    auto fn_isWhitespace = [](char32_t c) {
        // Feel free to handle all Unicode whitespace characters if desired
        const bool isWhitespace = c == ' '  ||
                                  c == '\t' ||
                                  c == '\n' ||
                                  c == '\r';
        return isWhitespace;
    };

    auto trimLeft = std::find_if_not(s.begin(), s.end(), fn_isWhitespace);
    auto trimRight = std::find_if_not(s.rbegin(), s.rend(), fn_isWhitespace);
    if (trimLeft == s.end() || trimRight == s.rend()) return "";

    return std::string(trimLeft, trimRight.base());
}

void jspp::docgen::utils::trimWhitespace(std::vector<std::string>& v) {
    auto isEmptyLine = [](const std::string& line) {
        return line == "";
    };
    auto firstNonEmptyLine = std::find_if_not(v.begin(), v.end(), isEmptyLine);
    auto lastNonEmptyLine = std::find_if_not(v.rbegin(), v.rend(), isEmptyLine);

    std::vector<std::string> trimmed(firstNonEmptyLine, lastNonEmptyLine.base());
    v.swap(trimmed);
}