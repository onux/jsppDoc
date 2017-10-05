#include "FilesystemEmitter.h"
#include <fstream>

using namespace jspp::docgen;

void FilesystemEmitter::write(const std::string& text, const std::string& path) {
	std::ofstream out(path, std::ifstream::out | std::ifstream::binary);
	out << text;
	out.close();
}