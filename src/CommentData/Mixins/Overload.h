//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_MIXINS_OVERLOAD_H
#define JSPP_DOCGEN_COMMENTDATA_MIXINS_OVERLOAD_H

#include <string>
#include <vector>
#include <memory>
#include "../OverloadTagCommentData.h"
#include "../MethodCommentData.h"

namespace jspp
{
namespace docgen
{
    /**
     * The `OverloadMixin` class represents all data associated
     * with documentation comment(s) for all overloaded methods/constructors and
     * their associated AST nodes.
     */
    class OverloadMixin
    {
    public:
        OverloadMixin(std::vector<std::unique_ptr<MethodCommentData>> methods);
        OverloadMixin(std::vector<std::unique_ptr<MethodCommentData>> methods,
                      const OverloadTagCommentData* const overload_tag);

        /**
         * Gets the summary describing the overloaded methods.
         */
        const std::string getSummary() const;

        /**
         * Gets the individual overloads for the method.
         */
        const std::vector<std::unique_ptr<MethodCommentData>>& getOverloads() const;

    protected:
        typedef std::vector<std::unique_ptr<MethodCommentData>> methods_t;
        typedef const OverloadTagCommentData* const overload_tag_t;

        std::string summary;
        methods_t overloads;

        const std::string extractSummary(const methods_t& methods) const;
        const std::string extractSummary(const methods_t& methods, overload_tag_t overload_tag) const;

        void mergeOverloadedDocs(const OverloadTagCommentData& overload_tag) const;
    };
}
}

#endif