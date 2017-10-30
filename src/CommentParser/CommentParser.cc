#include "CommentParser.h"
#include "Utils/Utils.h"
#include <pcrecpp.h>

using namespace jspp::docgen;

std::unique_ptr<Modifiers> CommentParser::parseModifiers(const std::bitset<10>& modifiers) const {
    auto parsed = std::unique_ptr<Modifiers>(new Modifiers);
    parsed->isPublic    = modifiers[0];
    parsed->isProtected = modifiers[1];
    parsed->isPrivate   = modifiers[2];
    parsed->isStatic    = modifiers[3];
    parsed->isFinal     = modifiers[4];
    parsed->isInline    = modifiers[5];
    parsed->isProperty  = modifiers[6];
    parsed->isAbstract  = modifiers[7];
    parsed->isVirtual   = modifiers[8];
    parsed->isOverride  = modifiers[9];

    static_assert(static_cast<int>(jspp::parser::Modifiers::PUBLIC)    == 0, "Expected Modifiers::PUBLIC == 0"   );
    static_assert(static_cast<int>(jspp::parser::Modifiers::PROTECTED) == 1, "Expected Modifiers::PROTECTED == 1");
    static_assert(static_cast<int>(jspp::parser::Modifiers::PRIVATE)   == 2, "Expected Modifiers::PRIVATE == 2"  );
    static_assert(static_cast<int>(jspp::parser::Modifiers::STATIC)    == 3, "Expected Modifiers::STATIC == 3"   );
    static_assert(static_cast<int>(jspp::parser::Modifiers::FINAL)     == 4, "Expected Modifiers::FINAL == 4"    );
    static_assert(static_cast<int>(jspp::parser::Modifiers::INLINE)    == 5, "Expected Modifiers::INLINE == 5"   );
    static_assert(static_cast<int>(jspp::parser::Modifiers::PROPERTY)  == 6, "Expected Modifiers::PROPERTY == 6" );
    static_assert(static_cast<int>(jspp::parser::Modifiers::ABSTRACT)  == 7, "Expected Modifiers::ABSTRACT == 7" );
    static_assert(static_cast<int>(jspp::parser::Modifiers::VIRTUAL)   == 8, "Expected Modifiers::VIRTUAL == 8"  );
    static_assert(static_cast<int>(jspp::parser::Modifiers::OVERRIDE)  == 9, "Expected Modifiers::OVERRIDE == 9" );

    return parsed;
}

std::string CommentParser::parseDocCommentText(const std::string& docComment) const {
    std::string textOnly;

    std::string text_noCommentDelimiters;
    pcrecpp::RE re_extractEnclosingComments(
        "(?# Match start of string)^"
        "(?# Skip over whitespace)\\s*"
        "(?# Match start of documentation comment: /**)\\/\\*\\*"
        "(?# Match - and capture for extraction - the documentation text)([\\s\\S]*)"
        "(?# Match close of documentation comment: */)\\*\\/"
        "(?# Skip over whitespace)\\s*"
        "(?# Match end of string)$"
    );
    re_extractEnclosingComments.FullMatch(docComment, &text_noCommentDelimiters);

    pcrecpp::RE re_extractLines(
        "(?# Match start of line)^"
        "(?# Skip over whitespace)\\s*"
        "(?# Skip over * characters at the beginning of a line)\\*"
        "(?# Match text until the end of the line)([^\r\n]*)"
        "(?# Match end of line)$"
        ,
        pcrecpp::RE_Options(PCRE_MULTILINE)
    );
    pcrecpp::StringPiece extractor(text_noCommentDelimiters);
    std::string extractedLine;
    std::vector<std::string> lines;
    while (re_extractLines.Consume(&extractor, &extractedLine)) {
        lines.push_back(extractedLine);
    }

    const bool isDocumented = text_noCommentDelimiters != "";
    const bool isMultiline = lines.size() != 0;
    if (isDocumented && !isMultiline) {
        const std::string singleLineText = utils::trimWhitespace(text_noCommentDelimiters);
        lines.push_back(singleLineText);
    }

    textOnly.assign(utils::join(lines, "\n"));
    return textOnly;
}

std::unique_ptr<DocCommentTags> CommentParser::parseDocCommentTags(const std::string& text) const {
    auto tags = std::unique_ptr<DocCommentTags>(new DocCommentTags);

    pcrecpp::RE re_tag(
        "(?# Match doc comment tag start operator)@"
        "(?# Match doc comment tag name)([a-z]+)"
        "(?# Skip whitespace)\\s*"
        "(?# Begin capture)("
        "(?# Capture all text for the 'tag description' until the beginning of a new tag with @name syntax)(?:(?!@[a-z]+)[\\s\\S])+"
        "(?# End capture))"
        "(?# Make the captured text, the 'tag description', optional)?"
        ,
        pcrecpp::RE_Options(PCRE_MULTILINE)
    );

    pcrecpp::StringPiece extractor(text);
    std::string tagName, tagText;
    while (re_tag.FindAndConsume(&extractor, &tagName, &tagText)) {
        tagText = utils::trimWhitespace(tagText);

        if (tagName == "summary") {
            tags->summary = tagText;
        }
        if (tagName == "example") {
            std::vector<std::string> lines = utils::splitLines(tagText);

            if (lines.size() == 0) continue;
            std::string exampleTitle = lines.front();
            lines.erase(lines.begin());

            if (lines.size() == 0) continue;
            utils::trimWhitespace(lines);
            utils::trimLeading(lines);
            std::string exampleCode = utils::join(lines, "\n");

            auto example = std::unique_ptr<Example>(
                new Example(exampleTitle, exampleCode)
            );
            tags->examples.push_back(std::move(example));
        }
        if (tagName == "overload") {
            std::vector<std::string> lines = utils::splitLines(tagText);
            if (lines.size() == 0) continue;

            std::string firstWord = lines[0].substr(0, lines[0].find(" "));
            tags->overload = firstWord;
        }
        if (tagName == "deprecated") {
            tags->isDeprecated = true;
            tags->deprecated_reason = tagText;
        }
        if (tagName == "return") {
            tags->return_info = tagText;
        }
        if (tagName == "see") {
            std::vector<std::string> tokens = utils::split(tagText, " ");
            
            if (tokens.size() < 2) continue;
            std::string path = tokens.back();
            tokens.pop_back();
            std::string title = utils::join(tokens, " ");

            auto see_also = std::unique_ptr<SeeAlso>(new SeeAlso(title, path));
            tags->see_also.push_back(std::move(see_also));
        }
        if (tagName == "param") {
            pcrecpp::RE re_param(
                "(?# Match start of string)^"
                "(?# Match the parameter name)(\\S+)"
                "(?# Skip over whitespace)\\s+"
                "(?# Match the parameter description text)([\\s\\S]+)"
                "(?# Match end of string)$"
            );

            std::string paramName, paramDescription;
            re_param.FullMatch(tagText, &paramName, &paramDescription);
            paramDescription = paramDescription;

            auto param = std::unique_ptr<Parameter>(
                new Parameter(paramName, paramDescription)
            );
            tags->params.push_back(std::move(param));
        }
    }

    return tags;
}

std::string CommentParser::parseDocCommentBodyText(const std::string& text) const {
    std::string raw;
    pcrecpp::RE re_body(
        "(?# Require the body text match to start from the very beginning)^"
        "(?# Consume @overload tags)(?:\\s*@overload\\s*\\w+)?"
        "(?# Start group A)("
        "(?# Start group B)(?:"
        "(?# Capture any character until the first @tag)(?!@[a-z]+)[\\s\\S]"
        "(?# End group B))"
        "(?# Capture one or more of everything specified inside capture group B)+"
        "(?# End group A))"
    );
    re_body.PartialMatch(text, &raw);

    std::vector<std::string> lines = utils::splitLines(raw);
    if (lines.size() == 1) {
        return utils::trimWhitespace(lines[0]);
    }

    utils::trimLeading(lines);
    std::string body = utils::join(lines, "\n");

    return utils::trimWhitespace(body);
}