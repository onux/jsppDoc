#include "Utils.h"

#include <sstream>
#include <iterator>
#include <algorithm>
#include <regex>
#include <cstddef>

using namespace jspp::docgen;

std::string utils::join(const std::vector<std::string>& v, const std::string& delimiter/* = ""*/) {
    if (v.size() == 0) {
        return "";
    }
    std::ostringstream ss;

    std::copy(v.begin(), v.end() - 1, std::ostream_iterator<std::string>(ss, delimiter.c_str()));
    ss << v.back();

    return ss.str();
}

std::vector<std::string> utils::split(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> result;

    const bool emptyDelimiter  = delimiter.size() == 0;
    const bool emptyString     = s.size() == 0;
    const bool delimiterFound  = s.find(delimiter) != std::string::npos;
    const bool cannotTokenize  = emptyDelimiter ||
                                 emptyString    ||
                                 !delimiterFound;
    if (cannotTokenize) {
        return { s };
    }

    size_t searchIndex = 0, lastSearchIndex = 0, offset = delimiter.size();
    while ((searchIndex = s.find_first_of(delimiter, searchIndex + offset)) != std::string::npos) {
        size_t substr_start = lastSearchIndex == 0 ? 0 : lastSearchIndex + offset;
        size_t substr_len = searchIndex - substr_start;

        result.push_back(s.substr(substr_start, substr_len));

        lastSearchIndex = searchIndex;
    }

    size_t lastDelimiter = lastSearchIndex + offset,
           endOfString = s.size() - lastDelimiter;
    result.push_back(s.substr(lastDelimiter, endOfString));

    return result;
}

std::vector<std::string> utils::splitLines(const std::string& s) {
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

std::string utils::trimWhitespace(const std::string &s) {
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

void utils::trimWhitespace(std::vector<std::string>& v) {
    auto isEmptyLine = [](const std::string& line) {
        return line == "";
    };
    auto firstNonEmptyLine = std::find_if_not(v.begin(), v.end(), isEmptyLine);
    auto lastNonEmptyLine = std::find_if_not(v.rbegin(), v.rend(), isEmptyLine);

    std::vector<std::string> trimmed(firstNonEmptyLine, lastNonEmptyLine.base());
    v.swap(trimmed);
}

void utils::trimLeading(std::vector<std::string>& lines) {
    auto fn_isLeadingWhitespace = [](char32_t c) {
        const bool isWhitespace = c == ' ' ||
                                  c == '\t';

        return isWhitespace;
    };

    ptrdiff_t firstLineWhitespace = 0;
    bool setFirstLineWhitespace = false;
    for(size_t i = 0, len = lines.size(); i != len; ++i) {
        const std::string& line = lines[i];

        const bool isEmptyLine = utils::trimWhitespace(line) == "";
        if (isEmptyLine) {
            continue;
        }

        auto it_firstChar = std::find_if_not(
            line.begin(),
            line.end(),
            fn_isLeadingWhitespace
        );

        if (!setFirstLineWhitespace) {
            firstLineWhitespace = it_firstChar - line.begin();
            setFirstLineWhitespace = true;
        }

        auto it_leadingWhitespace = line.begin() + firstLineWhitespace;
        const bool isTrimRequired = firstLineWhitespace != 0 &&
                                    it_firstChar >= it_leadingWhitespace &&
                                    it_leadingWhitespace < line.end();
        if (isTrimRequired) {
            std::string trimmedLine(it_leadingWhitespace, line.end());
            lines[i] = trimmedLine;
        }
    }
}

std::string utils::escapeXML(const std::string &s) {
    if (s.size() == 0) return "";

    std::string buffer;
    buffer.reserve(s.size());
    for(size_t pos = 0; pos != s.size(); ++pos) {
        switch(s[pos]) {
            case '&':  buffer.append("&amp;");     break;
            case '\"': buffer.append("&quot;");    break;
            case '\'': buffer.append("&apos;");    break;
            case '<':  buffer.append("&lt;");      break;
            case '>':  buffer.append("&gt;");      break;
            default:   buffer.append(&s[pos], 1);  break;
        }
    }

    return buffer;
}