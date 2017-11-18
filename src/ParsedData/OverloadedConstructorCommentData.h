//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_OVERLOADEDCONSTRUCTORCOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_OVERLOADEDCONSTRUCTORCOMMENTDATA_H

#include "CommentData.h"
#include "ConstructorCommentData.h"
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
     * The `OverloadedConstructorCommentData` class represents all data
     * associated with documentation comment(s) for all overloads of a
     * constructor *and* their associated ConstructorDeclaration AST nodes.
     */
    class OverloadedConstructorCommentData : public CommentData,
                                             public OverloadMixin
    {
    public:
        OverloadedConstructorCommentData(const std::string& className,
                                         const std::string& fqn,
                                         std::vector<std::unique_ptr<OverloadableCommentData>>&& constructors);
        OverloadedConstructorCommentData(const std::string& className,
                                         const std::string& fqn,
                                         std::vector<std::unique_ptr<OverloadableCommentData>>&& constructors,
                                         const OverloadTagCommentData* const overload_tag);

        /**
         * Gets the name (identifier) of the class the constructors belong to.
         */
        std::string getClassName() const;

        /**
         * Gets the fully-qualified name (FQN) of the constructors.
         */
        std::string getFQN() const;

    private:
        std::string className;
        std::string fqn;
    };
}
}

#endif