//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <jspp-parser/jspp-parser.h>

#include "DocVisitor.h"
#include "OutputBuilder.h"

using namespace jspp::parser;

void parseDocumentation(const std::string& path, const std::string& code) {
	try {
		std::unique_ptr<Program> program = jspp::parser::parse(
			code,
			path,
			{ jspp::parser::ParserOpt::PARSE_DOC_COMMENT }
		);

		jspp::docgen::OutputBuilder output;
		jspp::docgen::DocVisitor docvisitor(&output);
		program->accept(&docvisitor);

		std::cout << output.getOutput() << std::endl;
	}
	catch(const jspp::parser::ParseError& e) {
		std::cerr << e.description << std::endl;
	}
}

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		std::cerr << "Please provide at least one input file or directory." << std::endl;
		return EXIT_FAILURE;
	}

	for (int i = 1; i < argc; ++i) {
		std::string path(argv[i]);

		std::ifstream file(path, std::ifstream::in | std::ifstream::binary);
		if (file.bad()) {
			continue;
		}
		std::string code(
			(std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>()
		);

		parseDocumentation(path, code);
	}

	return EXIT_SUCCESS;
}