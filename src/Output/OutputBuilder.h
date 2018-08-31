//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_OUTPUTBUILDER_H
#define JSPP_DOCGEN_OUTPUTBUILDER_H

#include "ParsedData/includes.h"
#include "DocCommentData/Modifiers.h"
#include "DocCommentData/DocCommentTags.h"
#include <sstream>
#include <memory>

namespace jspp
{
namespace docgen
{
    /**
     * The `OutputBuilder` class is responsible for building the XML output.
     */
    class OutputBuilder
    {
    public:
        /**
         * Gets the output XML.
         */
        std::string getOutput() const;

        /**
         * Builds the XML for a JS++ `module`.
         *
         * @param comment The processed comment data. See `ModuleCommentData`.
         */
        void buildModule(const ModuleCommentData& comment);
        /**
         * Builds the XML for a JS++ `class`.
         *
         * @param comment The processed comment data. See `ClassCommentData`.
         */
        void buildClass(const ClassCommentData& comment);
        /**
         * Builds the XML for a JS++ `interface`.
         *
         * @param comment The processed comment data. See `InterfaceCommentData`.
         */
        void buildInterface(const InterfaceCommentData& comment);
        /**
         * Builds the XML for JS++ class/interface methods.
         *
         * @param comment The processed comment data. See `MethodCommentData`.
         */
        void buildFunctions(const MethodCommentData& comment);
        /**
         * Builds the XML for JS++ overloaded class/interface methods.
         *
         * @param comment The processed comment data. See `OverloadedMethodCommentData`.
         */
        void buildFunctions(const OverloadedMethodCommentData& comment);
        /**
         * Builds the XML for JS++ class constructors.
         *
         * @param comment The processed comment data. See `ConstructorCommentData`.
         */
        void buildFunctions(const ConstructorCommentData& comment);
        /**
         * Builds the XML for JS++ overloaded class constructors.
         *
         * @param comment The processed comment data. See `OverloadedConstructorCommentData`.
         */
        void buildFunctions(const OverloadedConstructorCommentData& comment);
        /**
         * Builds the XML for a JS++ class field.
         *
         * @param comment The processed comment data. See `FieldCommentData`.
         */
        void buildField(const FieldCommentData& comment);
        /**
         * Builds the XML for a JS++ enum declaration.
         *
         * @param comment The processed comment data. See `EnumCommentData`.
         */
        void buildEnumeration(const EnumCommentData& comment);

    private:
        std::ostringstream output;

        /**
         * Wraps the string in an XML CDATA section.
         */
        std::string cdata(const std::string& s) const;
        /**
         * Adds a <title> tag to the output XML document.
         *
         * @param title The text for the <title> tag.
         */
        void addTitle(const std::string& title);
        /**
         * Adds a <summary> tag to the output XML document.
         */
        void addSummary(const std::string& text);
        /**
         * Adds a <description> tag to the output XML document.
         */
        void addDescription(const std::string& text);
        /**
         * Adds a <example> tag to the output XML document.
         *
         * @param title The title of the example.
         * @param code The code for the example.
         */
        void addExample(const std::string& title, const std::string& code);
        /**
         * Adds a <deprecated> tag with the specified reason to the output XML
         * document.
         *
         * @param reason The reason the documented node is deprecated.
         */
        void addDeprecated(const std::string& reason);
        /**
         * Adds a <see> ("See Also") tag with the specified page to the output
         * XML document.
         *
         * @param title The link text for the suggested page.
         * @param page The fully-qualified name (FQN) or URL to suggest.
         */
        void addSeeAlso(const std::string& title, const std::string& page);
        /**
         * Adds the modifiers to the output XML document.
         *
         * @param modifiers The modifiers applied to the documented AST node.
         */
        void addModifiers(const jspp::docgen::Modifiers& modifiers);
        /**
         * Adds <param> tags to the output XML document.
         *
         * @param commentData The data for the associated documentation comment.
         * @param tags The parsed documentation tags and data.
         */
        void addParameters(const jspp::docgen::OverloadableCommentData& commentData,
                           const jspp::docgen::DocCommentTags& tags);
        /**
         * Adds a <class> tag to the output XML document for constructor class
         * names.
         *
         * @param text The text for the <class> tag.
         */
        void addClass(const std::string& text);
        /**
         * Returns the Markdown-formatted text for a given string.
         */
        std::string markdown(const std::string& text) const;
        /**
         * Generates a summary for the output XML document.
         *
         * If 'summary' is provided, the value is returned. Otherwise, a
         * truncated value of 'description' is returned.
         *
         * @param summary The user-provided summary from the @summary tag.
         * @param description The user-provided documentation body text.
         */
        std::string generateSummary(const std::string& summary,
                                    const std::string& description) const;
        /**
         * When generating the summary and the user did not provide an @summary
         * tag, then we have to truncate the description. This method will merge
         * all the truncated description lines into one line.
         */
        std::string mergeDescriptionLinesForSummary(const std::vector<std::string>& lines) const;
        /**
         * Function for generating a single merged description line for the
         * summary. If the line does not end with a whitespace character, we
         * must add it so:
         *
         * the
         * fox
         *
         * becomes:
         *
         * the fox
         *
         * rather than:
         *
         * thefox
         */
        std::string mergeSingleDescriptionLineForSummary(const std::string& line) const;
    };
}
}

#endif