//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_COMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_COMMENTDATA_H

#include <memory>

namespace jspp
{
namespace docgen
{
    /**
     * The `CommentData` class is the base class representing all data
     * associated with a documentation comment *and* the AST node it documents.
     */
    struct CommentData
    {
        virtual ~CommentData() {}

        template<typename T>
        bool is() {
            return nullptr != dynamic_cast<T*>(this);
        }

        template<typename T>
        static std::unique_ptr<T> dynamic_unique_ptr_cast(std::unique_ptr<CommentData> p) {
            T* tmp = dynamic_cast<T*>(p.get());
            if (nullptr == tmp) {
                return std::unique_ptr<T>(nullptr);
            }

            std::unique_ptr<T> result;
            p.release();
            result.reset(tmp);

            return result;
        }
    };
}
}

#endif