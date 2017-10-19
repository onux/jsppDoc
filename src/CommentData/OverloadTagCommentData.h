//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_OVERLOADCOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_OVERLOADCOMMENTDATA_H

#include "CommentData.h"
#include "Mixins/DocumentationText.h"

namespace jspp
{
namespace docgen
{
    /**
     * The `OverloadCommentData` class represents documentation comments using
     * the @overload tag.
     */
    struct OverloadTagCommentData : public CommentData,
                                    public DocumentationTextMixin
    {
        OverloadTagCommentData(const std::string& docComment);
    };
}
}

#endif