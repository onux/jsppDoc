#include "test.h"

TEST_CASE("Field Name") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                int bar = 1;
            }
        )"
    );

    SECTION("Ensure only one field node gets documented") {
        size_t count = xml->select_nodes("field").size();
        REQUIRE(count == 1);
    }
    SECTION("Field Name") {
        REQUIRE(std::string(xml->child("field").child_value("title")) == "bar");
    }
}

TEST_CASE("Multiple variable declarations don't get documented") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                int bar = 1, baz = 2;
            }
        )"
    );

    SECTION("No documentation generated") {
        size_t count = xml->select_nodes("field").size();
        REQUIRE(count == 0);
    }
}

TEST_CASE("Undocumented field generates nothing") {
    auto xml = generate(
        R"(
            class Foo
            {
                int bar = 1;
            }
        )"
    );

    SECTION("No documentation generated") {
        size_t count = xml->select_nodes("field").size();
        REQUIRE(count == 0);
    }
}

TEST_CASE("Field Summary") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * This is the long description.
                 *
                 * @summary This is the summary.
                 */
                int bar;
            }
        )"
    );

    SECTION("Summary") {
        std::string summary = xml->child("field").child_value("summary");
        REQUIRE(summary == "<p>This is the summary.</p>");
    }
}

TEST_CASE("Field Summary - If @summary tag is unavailable, use snippet of description") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * This is the long description.
                 *
                 * Some more text goes here.
                 */
                int bar;
            }
        )"
    );

    SECTION("Summary") {
        std::string summary = xml->child("field").child_value("summary");
        REQUIRE(summary == "<p>This is the long description.</p>");
    }
}

TEST_CASE("Field Description") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * This is the long description.
                 *
                 * Some more text goes here.
                 *
                 * @summary This is the summary.
                 */
                int bar;
            }
        )"
    );

    SECTION("Description") {
        std::string description = xml->child("field").child_value("description");
        REQUIRE(
            description
            ==
            "<p>This is the long description.</p>"
            "\n\n"
            "<p>Some more text goes here.</p>"
        );
    }
}

TEST_CASE("Field Modifiers") {
    auto xml = generate(
        R"(
            public module Foo
            {
                protected class Bar
                {
                    /**
                     */
                    private final int baz = 1;
                }
            }
        )"
    );
    SECTION("Modifier count") {
        size_t count = xml->child("field").child("modifiers").select_nodes("modifier").size();
        REQUIRE(count == 2);
    }

    SECTION("Modifier names") {
        auto it = xml->child("field").child("modifiers").children("modifier").begin();
        std::string modifier1 = it->attribute("name").value();
        REQUIRE(modifier1 == "private");

        ++it;
        std::string modifier2 = it->attribute("name").value();
        REQUIRE(modifier2 == "final");
    }
}