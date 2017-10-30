//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_OUTPUTEMITTER_FILESYSTEM_H
#define JSPP_DOCGEN_OUTPUTEMITTER_FILESYSTEM_H

#include "OutputEmitter.h"

namespace jspp
{
namespace docgen
{
    /**
     * A class for writing output to the file system.
     */
    struct FileEmitter : OutputEmitter
    {
        /**
         * Writes the data to the specified path.
         *
         * @param text The text to write to the output path.
         * @param path The file path to write the text to.
         */
        void write(const std::string& text, const std::string& path) override;
    };
}
}

#endif