#include "test.h"
#include <vector>

#include <iostream>

TEST_CASE("jspp::docgen::utils") {
	SECTION("join") {
		REQUIRE(jspp::docgen::utils::join({ "" }) == "");
		REQUIRE(jspp::docgen::utils::join({ "" }, ".") == "");
		REQUIRE(jspp::docgen::utils::join({ "System" }) == "System");
		REQUIRE(jspp::docgen::utils::join({ "System" }, ".") == "System");
		REQUIRE(jspp::docgen::utils::join({ "System", "Boolean" }) == "SystemBoolean");
		REQUIRE(jspp::docgen::utils::join({ "System", "Boolean" }, ".") == "System.Boolean");
	}
	SECTION("split") {
		REQUIRE(
			jspp::docgen::utils::split("foo\nbar\nbaz", "\n")
			==
			std::vector<std::string>({ "foo", "bar", "baz" })
		);
		REQUIRE(
			jspp::docgen::utils::split("foo\r\nbar\r\nbaz", "\r\n")
			==
			std::vector<std::string>({ "foo", "bar", "baz" })
		);
		REQUIRE(
			jspp::docgen::utils::split("foo,bar,baz", ",")
			==
			std::vector<std::string>({ "foo", "bar", "baz" })
		);
		REQUIRE(
			jspp::docgen::utils::split("foo,,bar,,baz", ",,")
			==
			std::vector<std::string>({ "foo", "bar", "baz" })
		);
		REQUIRE(
			jspp::docgen::utils::split("foo,,bar,,baz", "")
			==
			std::vector<std::string>({ "foo,,bar,,baz" })
		);
		REQUIRE(
			jspp::docgen::utils::split("foo\nbar\nbaz", "")
			==
			std::vector<std::string>({ "foo\nbar\nbaz" })
		);
	}
	SECTION("splitLines") {
		REQUIRE(
			jspp::docgen::utils::splitLines("foo\nbar\nbaz")
			==
			std::vector<std::string>({ "foo", "bar", "baz" })
		);
		REQUIRE(
			jspp::docgen::utils::splitLines("foo\rbar\rbaz")
			==
			std::vector<std::string>({ "foo", "bar", "baz" })
		);
		REQUIRE(
			jspp::docgen::utils::splitLines("foo\r\nbar\r\nbaz")
			==
			std::vector<std::string>({ "foo", "bar", "baz" })
		);

		REQUIRE(
			jspp::docgen::utils::splitLines("foo\n\n\nbar\n\n\nbaz")
			==
			std::vector<std::string>({ "foo", "", "", "bar", "", "", "baz" })
		);
		REQUIRE(
			jspp::docgen::utils::splitLines("foo\r\n\r\n\r\nbar\r\n\r\n\r\nbaz")
			==
			std::vector<std::string>({ "foo", "", "", "bar", "", "", "baz" })
		);
		REQUIRE(
			jspp::docgen::utils::splitLines("foo bar\nbaz")
			==
			std::vector<std::string>({ "foo bar", "baz" })
		);

		// \n\r is not a valid line ending on any OS so it will split on \n
		// and treat the following \r as an empty line
		REQUIRE(
			jspp::docgen::utils::splitLines("foo\n\rbar\n\rbaz")
			==
			std::vector<std::string>({ "foo", "", "bar", "", "baz" })
		);
	}
	SECTION("trimWhitespace") {
		using namespace jspp::docgen::utils;

		REQUIRE(trimWhitespace("") == "");
		REQUIRE(trimWhitespace("foobar") == "foobar");

		REQUIRE(trimWhitespace("\tfoo") == "foo");
		REQUIRE(trimWhitespace("foo\t") == "foo");
		REQUIRE(trimWhitespace("\tfoo\t") == "foo");
		REQUIRE(trimWhitespace("\t\tfoo\t") == "foo");
		REQUIRE(trimWhitespace("\tfoo\t\t") == "foo");

		REQUIRE(trimWhitespace("          foo") == "foo");
		REQUIRE(trimWhitespace("foo          ") == "foo");
		REQUIRE(trimWhitespace("          foo          ") == "foo");
		REQUIRE(trimWhitespace("          foo    ") == "foo");

		REQUIRE(trimWhitespace("\t\tfoo    ") == "foo");
		REQUIRE(trimWhitespace("     foo\t\t") == "foo");

		REQUIRE(trimWhitespace("          ") == "");
		REQUIRE(trimWhitespace("\t\t") == "");
		REQUIRE(trimWhitespace("\t  \t   ") == "");

		REQUIRE(trimWhitespace("\nfoo") == "foo");
		REQUIRE(trimWhitespace("foo\n") == "foo");
		REQUIRE(trimWhitespace("\rfoo\n") == "foo");
		REQUIRE(trimWhitespace("\r\n\r\nfoo\r\n") == "foo");
		REQUIRE(trimWhitespace("\r\nfoo\r\n\r\n") == "foo");

		REQUIRE(trimWhitespace(" ... Before the first tag. ") == "... Before the first tag.");
	}
}