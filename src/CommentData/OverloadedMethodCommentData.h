//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_OVERLOADEDMETHODCOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_OVERLOADEDMETHODCOMMENTDATA_H

#include "CommentData.h"
#include "MethodCommentData.h"
#include "OverloadTagCommentData.h"
#include "Mixins/Overload.h"
#include <string>
#include <vector>
#include <memory>

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
                                        public NameMixin,
                                        public OverloadMixin
    {
    public:
        OverloadedMethodCommentData(const std::string& name,
                                    const std::string& fqn,
                                    std::vector<std::unique_ptr<MethodCommentData>> methods);
        OverloadedMethodCommentData(const std::string& name,
                                    const std::string& fqn,
                                    std::vector<std::unique_ptr<MethodCommentData>> methods,
                                    const OverloadTagCommentData* const overload_tag);
    };
}
}

#endif