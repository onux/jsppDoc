//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include "Application.h"

#include <cstdlib>
#include <fstream>
#include <unordered_set>
#include <jspp-common/Filesystem.h>

#include "Output/DocVisitor.h"
#include "Output/OutputBuilder.h"
#include "Output/MenuVisitor.h"
#include "Emitters/FileEmitter.h"
#include "ParsedData/includes.h"
#include "Utils.h"

using namespace jspp::docgen;
using namespace jspp::parser;
using namespace jspp::common;

void Application::parseCommandlineOptions(int argc,
                                          char* argv[],
                                          std::string& input,
                                          std::string& output)
{
    bool parsingOutput = false;
    for(int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

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

int Application::process(const std::string& input_argv,
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
    std::vector<std::string> files =
        Filesystem::getFilesRecursively(inputDir, ".jspp");
    for (const std::string& inputFile : files) {
        std::string relInputPath;
        if (inputIsDirectory) {
            relInputPath = this->removeDirectoryPrefix(inputFile, inputDir);
        }

        const std::string outputRootDir =
            Filesystem::dirName(output_argv + relInputPath) + "/";

        const bool outputRootDirExists = generatedDirs.find(outputRootDir) !=
                                         generatedDirs.end();
        if (!outputRootDirExists) {
            Filesystem::mkdirp(outputRootDir);
            generatedDirs.insert(outputRootDir);
        }

        std::cout << "INPUT:  " << inputFile << std::endl;
        this->processInput(inputFile, outputRootDir);
    }

    return EXIT_SUCCESS;
}

void Application::processInput(const std::string& inputPath,
                               const std::string& outputRootDir)
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

    std::vector<std::unique_ptr<CommentData>> documents = docvisitor.getDocuments();
    while (documents.size() != 0) {
        this->generateXML(documents.back(), outputRootDir, *program);
        documents.pop_back();
    }
}

void Application::generateXML(std::unique_ptr<CommentData>& document,
                              const std::string& outputRootDir,
                              Program& ast)
{
    jspp::docgen::OutputBuilder builder;
    jspp::docgen::FileEmitter emitter;

    std::string xml, filename, outputDir;
    std::string menuXml, menuFilename;
    if (document->is<ModuleCommentData>()) {
        auto module_doc = CommentData::dynamic_unique_ptr_cast<ModuleCommentData>(
            std::move(document)
        );

        const std::string fqn = module_doc->getFQN();

        builder.buildModule(*module_doc);
        xml = builder.getOutput();
        filename = "index";
        outputDir = outputDirectory(fqn, outputRootDir, false);

        jspp::docgen::MenuVisitor menuvisitor;
        ast.accept(&menuvisitor);
        menuXml = menuvisitor.getOutput();
        menuFilename = module_doc->getName();
    }
    if (document->is<ClassCommentData>()) {
        auto class_doc = CommentData::dynamic_unique_ptr_cast<ClassCommentData>(
            std::move(document)
        );

        const std::string fqn = class_doc->getFQN();

        builder.buildClass(*class_doc);
        xml = builder.getOutput();
        filename = "index";
        outputDir = outputDirectory(fqn, outputRootDir, false);

        jspp::docgen::MenuVisitor menuvisitor;
        ast.accept(&menuvisitor);
        menuXml = menuvisitor.getOutput();
        menuFilename = class_doc->getName();
    }
    if (document->is<FieldCommentData>()) {
        auto field_doc = CommentData::dynamic_unique_ptr_cast<FieldCommentData>(
            std::move(document)
        );

        const std::string identifier = field_doc->getName();
        const std::string fqn = field_doc->getFQN();

        builder.buildField(*field_doc);
        xml = builder.getOutput();
        filename = identifier;
        outputDir = outputDirectory(fqn, outputRootDir, true);
    }
    if (document->is<ConstructorCommentData>()) {
        auto ctor_doc = CommentData::dynamic_unique_ptr_cast<ConstructorCommentData>(
            std::move(document)
        );

        const std::string fqn = ctor_doc->getFQN();

        builder.buildFunctions(*ctor_doc);
        xml = builder.getOutput();
        filename = "constructor";
        outputDir = outputDirectory(fqn, outputRootDir, true);
    }
    if (document->is<OverloadedConstructorCommentData>()) {
        auto ctor_doc = CommentData::dynamic_unique_ptr_cast<OverloadedConstructorCommentData>(
            std::move(document)
        );

        const std::string fqn = ctor_doc->getFQN();

        builder.buildFunctions(*ctor_doc);
        xml = builder.getOutput();
        filename = "constructor";
        outputDir = outputDirectory(fqn, outputRootDir, true);
    }
    if (document->is<MethodCommentData>()) {
        auto method_doc = CommentData::dynamic_unique_ptr_cast<MethodCommentData>(
            std::move(document)
        );

        const std::string identifier = method_doc->getName();
        const std::string fqn = method_doc->getFQN();

        builder.buildFunctions(*method_doc);
        xml = builder.getOutput();
        filename = identifier;
        outputDir = outputDirectory(fqn, outputRootDir, true);
    }
    if (document->is<OverloadedMethodCommentData>()) {
        auto method_doc = CommentData::dynamic_unique_ptr_cast<OverloadedMethodCommentData>(
            std::move(document)
        );

        const std::string identifier = method_doc->getName();
        const std::string fqn = method_doc->getFQN();

        builder.buildFunctions(*method_doc);
        xml = builder.getOutput();
        filename = identifier;
        outputDir = outputDirectory(fqn, outputRootDir, true);
    }
    if (document->is<EnumCommentData>()) {
        auto enum_doc = CommentData::dynamic_unique_ptr_cast<EnumCommentData>(
            std::move(document)
        );

        const std::string identifier = enum_doc->getName();
        const std::string fqn = enum_doc->getFQN();

        builder.buildEnumeration(*enum_doc);
        xml = builder.getOutput();
        filename = identifier;
        outputDir = outputDirectory(fqn, outputRootDir, true);
    }
    if (document->is<InterfaceCommentData>()) {
        auto interface_doc =
            CommentData::dynamic_unique_ptr_cast<InterfaceCommentData>(
                std::move(document)
            );

        const std::string fqn = interface_doc->getFQN();

        builder.buildInterface(*interface_doc);
        xml = builder.getOutput();
        filename = "index";
        outputDir = outputDirectory(fqn, outputRootDir, false);

        jspp::docgen::MenuVisitor menuvisitor;
        ast.accept(&menuvisitor);
        menuXml = menuvisitor.getOutput();
        menuFilename = interface_doc->getName();
    }

    if (xml != "") {
        Filesystem::mkdirp(outputDir);

        const std::string outputPath = outputDir + filename + ".xml";

        std::cout << "OUTPUT: " << outputPath << std::endl;
        emitter.write(xml, outputPath);
    }

    if (menuXml != "") {
        Filesystem::mkdirp(Filesystem::resolvePath(outputDir + "../menus/"));

        const std::string menuOutputPath = Filesystem::resolvePath(outputDir + "../menus/" + menuFilename + ".xml");

        std::cout << "MENU:   " << menuOutputPath << std::endl;
        emitter.write(menuXml, menuOutputPath);
    }
}

std::string Application::outputDirectory(const std::string& fqn,
                                         const std::string& outputRootDir,
                                         const bool isClassMember) const
{
    std::string relativeDir(fqn);
    std::replace(relativeDir.begin(), relativeDir.end(), '.', '/');
    std::string absoluteDir = outputRootDir + relativeDir;

    if (isClassMember) {
        std::vector<std::string> tokens = utils::split(absoluteDir, "/");
        tokens.pop_back();
        absoluteDir = utils::join(tokens, "/");
    }

    absoluteDir += "/";

    return absoluteDir;
}

std::string Application::removeDirectoryPrefix(const std::string& path,
                                               const std::string& prefix) const
{
    std::string result = path;

    const bool hasDirectoryPrefix = path.find_first_of(prefix) !=
                                    std::string::npos;
    if (hasDirectoryPrefix) {
        result.erase(0, prefix.size());
    }

    return result;
}