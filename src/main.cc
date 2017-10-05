//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include "Application.h"

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		std::cerr
			<< "Please provide at least one input file or directory."
			<< std::endl
			<< std::endl
			<< "Usage: js++-doc <input> [-o <output-directory>]"
			<< std::endl;

		return EXIT_FAILURE;
	}

	jspp::docgen::Application jsppDoc;
	std::string input, output;

	jsppDoc.parseCommandlineOptions(argc, argv, input, output);
	return jsppDoc.processFiles(input, output);
}