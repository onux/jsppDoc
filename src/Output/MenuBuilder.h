//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_MENUBUILDER_H
#define JSPP_DOCGEN_MENUBUILDER_H

#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <jspp-parser/jspp-parser.h>

namespace jspp
{
namespace docgen
{
    /**
     * The `MenuBuilder` class provides methods for generating menu XML
     * files for documentation navigation.
     */
    class MenuBuilder
    {
    public:
        /**
         * Gets the output XML.
         */
        const std::string getOutput() const;

    protected:
        std::ostringstream output;
        std::map<std::string, std::string> members;

        std::unordered_set<std::string> overloadTags;
    };
}
}

#endif