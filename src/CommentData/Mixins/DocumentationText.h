//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_MIXINS_DOCUMENTATIONTEXT_H
#define JSPP_DOCGEN_COMMENTDATA_MIXINS_DOCUMENTATIONTEXT_H

#include "../../DocCommentTags.h"
#include <memory>
#include <string>

namespace jspp
{
namespace docgen
{
    /**
     * The `DocumentationTextMixin` mixin provides functions for handling
     * documentation comment text and tags.
     */
    class DocumentationTextMixin
    {
    public:
        /**
         * Return the raw, unformatted documentation text.
         */
        std::string getText() const;
        /**
         * Get the documentation body text.
         */
        std::string getBodyText() const;
        /**
         * Set the documentation body text.
         *
         * @param text The new documentation body text to replace with.
         */
        void setBodyText(const std::string& text);
        /**
         * Get the documentation tags and their parsed associated data.
         */
        const jspp::docgen::DocCommentTags& tags() const;
        /**
         * Returns writable (non-const) documentation tags and their parsed
         * associated data.
         */
        jspp::docgen::DocCommentTags& writableTags() const;

    protected:
        /**
         * Constructs a `DocumentationTextMixin`.
         *
         * @param docComment The raw, unformatted documentation text to parse.
         */
        DocumentationTextMixin(const std::string& docComment);

    private:
        std::string text;
        std::string body_text;
        std::unique_ptr<jspp::docgen::DocCommentTags> documentation_tags;
    };
}
}

#endif