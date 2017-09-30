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

TEST_CASE("Module Documentation - One module") {
	auto xml = generate(
		R"(
			/**
			 */
			module Foo
			{
			}
		)"
	);

	SECTION("Ensure only one module node gets documented") {
		size_t count = xml->select_nodes("module").size();
		REQUIRE(count == 1);
	}
	SECTION("FQN") {
		REQUIRE(std::string(xml->child("module").child_value("title")) == "Foo");
	}
}

TEST_CASE("Module Documentation - Multiple modules separated by dot (.)") {
	auto xml = generate(
		R"(
			/**
			 */
			module Foo.Bar.Baz
			{
			}
		)"
	);

	SECTION("Ensure only one module node gets documented") {
		size_t count = xml->select_nodes("module").size();
		REQUIRE(count == 1);
	}
	SECTION("FQN") {
		REQUIRE(std::string(xml->child("module").child_value("title")) == "Baz");
	}
}