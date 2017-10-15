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
         * Get the documentation tags and their parsed associated data.
         */
        std::shared_ptr<jspp::docgen::DocCommentTags> getTags() const;

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
        std::shared_ptr<jspp::docgen::DocCommentTags> tags;
    };
}
}

#endif