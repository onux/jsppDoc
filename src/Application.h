//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_APPLICATION_H
#define JSPP_DOCGEN_APPLICATION_H

#include <string>
#include <memory>
#include <jspp-parser/jspp-parser.h>
#include "CommentData/CommentData.h"

namespace jspp
{
namespace docgen
{
    /**
     * `Application` is responsible for providing functions used by the main
     * js++Doc application.
     */
    class Application
    {
    public:
        /**
         * Parses the command-line arguments provided to js++Doc.
         *
         * @param argc[in] The number of arguments to the program.
         * @param argv[in] An array of the arguments to the program.
         * @param input[out] The user-specified file/directory input argument
         *                   is written to this argument.
         * @param output[out] The user-specified output argument is written to
         *                    this argument.
         */
        void parseCommandlineOptions(int argc,
                                     char* argv[],
                                     std::string& input,
                                     std::string& output);
        /**
         * Reads the input file(s) and calls `processInput` for each
         * file.
         *
         * @param input_argv The input file (from argv).
         * @param output_argv The output directory (from argv).
         * @return EXIT_SUCCESS if documentation successfully generated and
         *         EXIT_FAILURE otherwise.
         */
        int process(const std::string& input_argv,
                    const std::string& output_argv);
        /**
         * Parses the specified JS++ input file and calls `generateXML`.
         *
         * @param inputPath The JS++ input file to generate documentation for.
         * @param outputDir The directory to write the write output files to.
         */
        void processInput(const std::string& inputPath,
                          const std::string& outputDir);
        /**
         * Generates the final XML documentation page.
         *
         * @param document The data for the documentation comment.
         * @param outputRootDir The root directory to write the write output
         *                      files and folders to.
         * @param ast The abstract syntax tree (AST) of the input program.
         */
        void generateXML(std::unique_ptr<CommentData>& document,
                         const std::string& outputRootDir,
                         jspp::parser::Program& ast);

    private:
        /**
         * Generates the output directory path based on the FQN (fully-qualified
         * name) of the documented node.
         *
         * @param fqn The fully-qualified name (FQN) of the documented node.
         * @param outputRootDir The root directory to write the write output
         *                      files and folders to.
         * @param isClassMember Is the documented node a member of a class?
         */
        std::string outputDirectory(const std::string& fqn,
                                    const std::string& outputRootDir,
                                    const bool isClassMember) const;
        /**
         * Removes the specified prefix from the specified path.
         *
         * @param path The path to remove the prefix from.
         * @param prefix The prefix to remove. A directory path is expected.
         * @return The original path with the specified prefix removed.
         */
        std::string removeDirectoryPrefix(const std::string& path,
                                          const std::string& prefix) const;
    };
}
}

#endif