#include "test.h"

using namespace jspp::parser;

std::unique_ptr<pugi::xml_document> generate(const std::string& code) {
	std::string path = "";
	std::unique_ptr<Program> program = jspp::parser::parse(
		code,
		path,
		{ jspp::parser::ParserOpt::PARSE_DOC_COMMENT }
	);

	jspp::docgen::OutputBuilder output;
	jspp::docgen::DocVisitor docvisitor(&output);
	program->accept(&docvisitor);
	std::string generated = output.getOutput();

	auto xml = std::unique_ptr<pugi::xml_document>(new pugi::xml_document);
	xml->load_string(generated.c_str());

	return xml;
}