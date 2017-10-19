//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_OVERLOADEDMETHODCOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_OVERLOADEDMETHODCOMMENTDATA_H

#include "CommentData.h"
#include "MethodCommentData.h"
#include "OverloadTagCommentData.h"
#include <string>
#include <vector>

namespace jspp
{
namespace docgen
{
    /**
     * The `OverloadedMethodCommentData` class represents all data associated
     * with documentation comment(s) for all overloads of a method *and* their
     * associated FunctionDeclaration AST nodes.
     */
    class OverloadedMethodCommentData : public CommentData,
                                        public NameMixin
    {
    public:
        OverloadedMethodCommentData(const std::string& name,
                                    const std::string& fqn,
                                    std::vector<std::unique_ptr<MethodCommentData>> methods);
        OverloadedMethodCommentData(const std::string& name,
                                    const std::string& fqn,
                                    std::vector<std::unique_ptr<MethodCommentData>> methods,
                                    const OverloadTagCommentData* const overload_tag);

        /**
         * Gets the summary describing the overloaded methods.
         */
        std::string getSummary() const;

        /**
         * Gets the individual overloads for the method.
         */
        const std::vector<std::unique_ptr<MethodCommentData>>& getOverloads() const;

    private:
        typedef std::vector<std::unique_ptr<MethodCommentData>> methods_t;
        typedef const OverloadTagCommentData* const overload_tag_t;

        std::string summary;
        methods_t overloads;

        std::string extractSummary(const methods_t& methods) const;
        std::string extractSummary(const methods_t& methods, overload_tag_t overload_tag) const;

        void mergeOverloadedDocs(const OverloadTagCommentData& overload_tag) const;
    };
}
}

#endif