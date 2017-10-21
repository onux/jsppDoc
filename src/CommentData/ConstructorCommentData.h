//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_CONSTRUCTORCOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_CONSTRUCTORCOMMENTDATA_H

#include "OverloadableCommentData.h"
#include <bitset>
#include <vector>
#include <string>

namespace jspp
{
namespace docgen
{
    /**
     * The `ConstructorCommentData` class represents the data associated with
     * the documentation comment for a constructor *and* the
     * ConstructorDeclaration AST node it documents.
     *
     * This class only represents a single constructor. Objects of this class
     * are eventually combined into `OverloadedConstructorCommentData` to
     * represent all overloads of the constructor.
     */
    class ConstructorCommentData : public OverloadableCommentData
    {
    public:
        ConstructorCommentData(const std::string& className,
                               const std::string& fqn,
                               const std::vector<std::string>& param_types,
                               const std::string& docComment,
                               const std::bitset<10>& modifiers);

        /**
         * Gets the name (identifier) of the class the constructor belongs to.
         */
        std::string getClassName() const;

        /**
         * Gets the fully-qualified name (FQN) of the constructor.
         */
        std::string getFQN() const;

    private:
        std::string className;
        std::string fqn;
    };
}
}

#endif