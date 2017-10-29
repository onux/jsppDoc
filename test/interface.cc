#include "test.h"

TEST_CASE("Interface Name") {
    auto xml = generate(
        R"(
            /**
             */
            interface IFoo
            {
            }
        )"
    );

    SECTION("Ensure only one interface node gets documented") {
        size_t count = xml->select_nodes("interface").size();
        REQUIRE(count == 1);
    }
    SECTION("Interface Name") {
        REQUIRE(std::string(xml->child("interface").child_value("title")) == "IFoo");
    }
}

TEST_CASE("Interface Name - inside module") {
    auto xml = generate(
        R"(
            module Foo
            {
                /**
                 */
                interface Bar
                {
                }
            }
        )"
    );

    SECTION("Ensure only one interface node gets documented") {
        size_t count = xml->select_nodes("interface").size();
        REQUIRE(count == 1);
    }
    SECTION("Interface Name") {
        REQUIRE(std::string(xml->child("interface").child_value("title")) == "Bar");
    }

    xml = generate(
        R"(
            module Foo.Bar.Baz
            {
                /**
                 */
                interface Qux
                {
                }
            }
        )"
    );

    SECTION("Ensure only one interface node gets documented") {
        size_t count = xml->select_nodes("interface").size();
        REQUIRE(count == 1);
    }
    SECTION("Interface Name") {
        REQUIRE(std::string(xml->child("interface").child_value("title")) == "Qux");
    }

    xml = generate(
        R"(
            module Foo
            {
                module Bar
                {
                    /**
                     */
                    interface Qux
                    {
                    }
                }
            }
        )"
    );

    SECTION("Ensure only one interface node gets documented") {
        size_t count = xml->select_nodes("interface").size();
        REQUIRE(count == 1);
    }
    SECTION("Interface Name") {
        REQUIRE(std::string(xml->child("interface").child_value("title")) == "Qux");
    }
}

TEST_CASE("Undocumented interface generates nothing") {
    auto xml = generate(
        R"(
            interface Foo
            {
            }
        )"
    );

    SECTION("No documentation generated") {
        size_t count = xml->select_nodes("interface").size();
        REQUIRE(count == 0);
    }
}

TEST_CASE("Interface Summary") {
    auto xml = generate(
        R"(
            /**
             * This is the long description.
             *
             * @summary This is the summary.
             */
            interface Foo
            {
            }
        )"
    );

    SECTION("Summary") {
        std::string summary = xml->child("interface").child_value("summary");
        REQUIRE(summary == "<p>This is the summary.</p>");
    }
}

TEST_CASE("Interface Description") {
    auto xml = generate(
        R"(
            /**
             * This is the long description.
             *
             * Some more text goes here.
             *
             * @summary This is the summary.
             */
            interface Foo
            {
            }
        )"
    );

    SECTION("Description") {
        std::string description = xml->child("interface").child_value("description");
        REQUIRE(
            description
            ==
            "<p>This is the long description.</p>"
            "\n\n"
            "<p>Some more text goes here.</p>"
        );
    }
}