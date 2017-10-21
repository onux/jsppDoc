#include "test.h"
#include <cassert>

using namespace jspp::docgen;
using namespace jspp::parser;

std::unique_ptr<pugi::xml_document> generate(const std::string& code) {
    std::string path = "";
    std::unique_ptr<Program> program = jspp::parser::parse(
        code,
        path,
        { jspp::parser::ParserOpt::PARSE_DOC_COMMENT }
    );

    jspp::docgen::OutputBuilder builder;
    jspp::docgen::DocVisitor docvisitor;
    program->accept(&docvisitor);

    auto documents = docvisitor.getDocuments();
    while (documents.size() != 0) {
        std::unique_ptr<CommentData>& document = documents.back();

        if (document->is<ModuleCommentData>()) {
            auto module_doc = CommentData::dynamic_unique_ptr_cast<ModuleCommentData>(
                std::move(document)
            );
            builder.buildModule(*module_doc);
        }
        if (document->is<ClassCommentData>()) {
            auto class_doc = CommentData::dynamic_unique_ptr_cast<ClassCommentData>(
                std::move(document)
            );
            builder.buildClass(*class_doc);
        }
        if (document->is<MethodCommentData>()) {
            auto method_doc = CommentData::dynamic_unique_ptr_cast<MethodCommentData>(
                std::move(document)
            );
            builder.buildFunctions(*method_doc);
        }
        if (document->is<OverloadedMethodCommentData>()) {
            auto method_doc = CommentData::dynamic_unique_ptr_cast<OverloadedMethodCommentData>(
                std::move(document)
            );
            builder.buildFunctions(*method_doc);
        }
        if (document->is<ConstructorCommentData>()) {
            auto ctor_doc = CommentData::dynamic_unique_ptr_cast<ConstructorCommentData>(
                std::move(document)
            );
            builder.buildFunctions(*ctor_doc);
        }
        if (document->is<OverloadedConstructorCommentData>()) {
            auto ctor_doc = CommentData::dynamic_unique_ptr_cast<OverloadedConstructorCommentData>(
                std::move(document)
            );
            builder.buildFunctions(*ctor_doc);
        }
        if (document->is<FieldCommentData>()) {
            auto field_doc = CommentData::dynamic_unique_ptr_cast<FieldCommentData>(
                std::move(document)
            );
            builder.buildField(*field_doc);
        }

        documents.pop_back();
    }
    std::string generated = builder.getOutput();

    auto xml = std::unique_ptr<pugi::xml_document>(new pugi::xml_document);
    xml->load_string(generated.c_str());

    return xml;
}