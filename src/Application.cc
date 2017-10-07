//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include "Application.h"

#include <cstdlib>
#include <fstream>
#include <unordered_set>
#include <jspp-parser/jspp-parser.h>
#include <jspp-common/Filesystem.h>

#include "DocVisitor.h"
#include "OutputBuilder.h"
#include "FileEmitter.h"

using namespace jspp::docgen;
using namespace jspp::parser;
using namespace jspp::common;

void Application::parseCommandlineOptions(	int argc,
											char* argv[],
											std::string& input,
											std::string& output)
{
	bool parsingOutput = false;
	for(int i = 1; i < argc; ++i) {
		std::string arg = argv[i];

		if (std::string(arg) == "-o") {
			parsingOutput = true;
			continue;
		}

		if (parsingOutput) {
			output = Filesystem::resolvePath(arg);
			parsingOutput = false;
		}
		else {
			input = Filesystem::resolvePath(arg);
		}
	}
}

int Application::process(	const std::string& input_argv,
							const std::string& output_argv)
{
	if (!Filesystem::isDirectory(output_argv)) {
		std::cerr << "ERROR: Output must be a directory." << std::endl;
		return EXIT_FAILURE;
	}

	Filesystem::mkdirp(output_argv);

	const bool inputIsDirectory = Filesystem::isDirectory(input_argv);

	std::string inputDir;
	if (inputIsDirectory) {
		inputDir = input_argv;
	}

	std::unordered_set<std::string> generatedDirs;
	std::vector<std::string> files = Filesystem::getFilesRecursively(inputDir, ".jspp");
	for (const std::string& inputFile : files) {
		std::string relInputPath;
		if (inputIsDirectory) {
			relInputPath = this->removeDirectoryPrefix(inputFile, inputDir);
		}

		const std::string outputDir = Filesystem::dirName(output_argv + relInputPath) + "/";

		const bool outputDirExists = 	generatedDirs.find(outputDir) !=
										generatedDirs.end();
		if (!outputDirExists) {
			Filesystem::mkdirp(outputDir);
			generatedDirs.insert(outputDir);
		}

		std::cout << "INPUT:  " << inputFile << std::endl;
		this->processInput(inputFile, outputDir);
	}

	return EXIT_SUCCESS;
}


void Application::processInput(	const std::string& inputPath,
								const std::string& outputDir)
{
	std::ifstream file(inputPath, std::ifstream::in | std::ifstream::binary);
	if (file.bad()) {
		std::cerr << "ERROR: Unable to open " << inputPath << std::endl;
		return;
	}
	std::string code(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);

	std::unique_ptr<Program> program;
	jspp::docgen::DocVisitor docvisitor;
	try {
		program = jspp::parser::parse(
			code,
			inputPath,
			{ jspp::parser::ParserOpt::PARSE_DOC_COMMENT }
		);
		program->accept(&docvisitor);
	}
	catch(const jspp::parser::ParseError& e) {
		std::cerr << e.description << std::endl;
	}

	auto documents = docvisitor.getDocuments();
	while (documents.size() != 0) {
		this->generateXML(documents.front(), outputDir);
		documents.pop();
	}
}

void Application::generateXML(	std::shared_ptr<CommentData> document,
								const std::string& outputDir)
{
	jspp::docgen::OutputBuilder builder;
	jspp::docgen::FileEmitter emitter;

	std::string relativeDir = document->getFQN();
	std::replace(relativeDir.begin(), relativeDir.end(), '.', '/');
	std::string absoluteDir = outputDir + relativeDir + "/";

	Filesystem::mkdirp(absoluteDir);

	std::string xml, identifier;
	auto node = document->getNode();
	if (node->is<ModuleDeclaration>()) {
		builder.buildModule(document);
		xml = builder.getOutput();
		identifier = "index";
	}
	if (node->is<ClassDeclaration>()) {
		builder.buildClass(document);
		xml = builder.getOutput();
		identifier = "index";
	}

	if (xml != "") {
		const std::string outputPath = absoluteDir + identifier + ".xml";

		std::cout << "OUTPUT: " << outputPath << std::endl;
		emitter.write(xml, outputPath);
	}
}

std::string Application::removeDirectoryPrefix(	const std::string& path,
												const std::string& prefix)
{
	std::string result = path;

	const bool hasDirectoryPrefix = path.find_first_of(prefix) !=
									std::string::npos;
	if (hasDirectoryPrefix) {
		result.erase(0, prefix.size());
	}

	return result;
}