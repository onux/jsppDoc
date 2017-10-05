//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_TEST_H
#define JSPP_DOCGEN_TEST_H

#include <catch.hpp>
#include <pugixml.hpp>
#include "CommentData.h"
#include "CommentParser.h"
#include "DocCommentTags.h"
#include "DocVisitor.h"
#include "NopEmitter.h"
#include "OutputBuilder.h"
#include "Utils.h"

std::unique_ptr<pugi::xml_document> generate(const std::string& code);

#endif