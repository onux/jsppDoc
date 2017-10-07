#include "test.h"
#include <cassert>

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
		auto document = documents.front();

		auto node = document->getNode();
		if (node->is<ModuleDeclaration>()) {
			builder.buildModule(document);
		}
		if (node->is<ClassDeclaration>()) {
			builder.buildClass(document);
		}

		documents.pop();
	}
	std::string generated = builder.getOutput();

	auto xml = std::unique_ptr<pugi::xml_document>(new pugi::xml_document);
	xml->load_string(generated.c_str());

	return xml;
}