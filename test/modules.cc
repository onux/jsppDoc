#include "test.h"

TEST_CASE("Module Name - One module") {
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

TEST_CASE("Module Name - Multiple modules separated by dot (.)") {
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

TEST_CASE("Module Summary") {
	auto xml = generate(
		R"(
			/**
			 * This is the long description.
			 *
			 * @summary This is the summary.
			 */
			module Foo
			{
			}
		)"
	);

	SECTION("FQN") {
		std::string summary = xml->child("module").child_value("summary");
		REQUIRE(summary == "This is the summary.");
	}
}

TEST_CASE("Module Summary - If @summary tag is unavailable, use snippet of description") {
	// TODO
	SKIP_TEST();
}

TEST_CASE("Module Description") {
	auto xml = generate(
		R"(
			/**
			 * This is the long description.
			 *
			 * Some more text goes here.
			 *
			 * @summary This is the summary.
			 */
			module Foo
			{
			}
		)"
	);

	SECTION("FQN") {
		std::string description = xml->child("module").child_value("description");
		REQUIRE(description == "This is the long description.\n\nSome more text goes here.");
	}
}

TEST_CASE("Module Examples") {
	auto xml = generate(
		R"(
			/**
			 * This is the long description.
			 *
			 * Some more text goes here.
			 *
			 * @example First Example
			 * import System;
			 *
			 * Console.log("Example1");
			 *
			 * @example Second Example
			 * import System;
			 *
			 * Console.log("Example2");
			 */
			module Foo
			{
			}
		)"
	);

	SECTION("Generates two <example> nodes") {
		size_t count = xml->child("module").child("examples").select_nodes("example").size();
		REQUIRE(count == 2);
	}

	SECTION("Example 1") {
		auto it = xml->child("module").child("examples").children("example").begin();

		std::string title = it->attribute("name").value();
		REQUIRE(title == "First Example");

		std::string code = it->child_value();
		REQUIRE(code == "import System;\n\nConsole.log(\"Example1\");");
	}

	SECTION("Example 2") {
		auto it = xml->child("module").child("examples").children("example").begin();
		++it;

		std::string title = it->attribute("name").value();
		REQUIRE(title == "Second Example");
		std::string code = it->child_value();
		REQUIRE(code == "import System;\n\nConsole.log(\"Example2\");");
	}
}