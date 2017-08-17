/*#include "test.h"

TEST_CASE("Module Documentation") {
	std::string code = R"(
		module Foo.Bar.Baz
		{
		}
	)";
	std::string path = "";
	std::unique_ptr<Program> program = jspp::parser::parse(
		code,
		path,
		{ jspp::parser::ParserOpt::PARSE_DOC_COMMENT }
	);

	jspp::docgen::OutputBuilder output;
	jspp::docgen::DocVisitor docvisitor(&output);
	program->accept(&docvisitor);

	SECTION("FQN") {
		REQUIRE("" == "");
	}
}*/