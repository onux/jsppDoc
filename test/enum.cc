#include "test.h"

TEST_CASE("Enum Name") {
    auto xml = generate(
        R"(
            /**
             */
            enum Foo { BAR, BAZ }
        )"
    );

    SECTION("Ensure only one enum node gets documented") {
        size_t count = xml->select_nodes("enum").size();
        REQUIRE(count == 1);
    }
    SECTION("Enum Name") {
        REQUIRE(std::string(xml->child("enum").child_value("title")) == "Foo");
    }
}

TEST_CASE("Undocumented enum generates nothing") {
    auto xml = generate(
        R"(
            enum Foo { BAR, BAZ }
        )"
    );

    SECTION("No documentation generated") {
        size_t count = xml->select_nodes("enum").size();
        REQUIRE(count == 0);
    }
}

TEST_CASE("Enum Underlying Type") {
    auto xml = generate(
        R"(
            /**
             */
            enum Foo : unsigned int { BAR, BAZ }
        )"
    );

    SECTION("Underlying type is 'unsigned int'") {
        const std::string type = xml->child("enum").attribute("type").value();
        REQUIRE(type == "unsigned int");
    }
}

TEST_CASE("Enum Summary") {
    auto xml = generate(
        R"(
            /**
             * This is the long description.
             *
             * @summary This is the summary.
             */
            enum Foo { BAR, BAZ }
        )"
    );

    SECTION("Summary") {
        std::string summary = xml->child("enum").child_value("summary");
        REQUIRE(summary == "<p>This is the summary.</p>");
    }
}

TEST_CASE("Enum member description") {
    SKIP_TEST();
    auto xml = generate(
        R"(
            /**
             */
            enum Foo {
                /**
                 * This is the long description #1.
                 */
                BAR,
                BAZ
            }
        )"
    );

    SECTION("Description 1") {
        auto it = xml->child("enum").child("members").children("member").begin();
        const std::string description = it->child_value("description");
        REQUIRE(description == "<p>This is the long description #1.</p>");
    }

    SECTION("Description 2 does not exist") {
        size_t count = xml->child("enum").child("members").select_nodes("member").size();
        const bool onlyOneMember = count == 1;
        REQUIRE(onlyOneMember);
    }

    auto xml2 = generate(
        R"(
            /**
             */
            enum Foo {
                /**
                 * This is the long description #1.
                 */
                BAR,
                /**
                 * This is the long description #2.
                 */
                BAZ
            }
        )"
    );

    SECTION("Description 1") {
        auto it = xml->child("enum").child("members").children("member").begin();
        const std::string description = it->child_value("description");
        REQUIRE(description == "<p>This is the long description #1.</p>");
    }

    SECTION("Description 2") {
        auto it = xml->child("enum").child("members").children("member").begin();
        ++it;
        const std::string description = it->child_value("description");
        REQUIRE(description == "<p>This is the long description #2.</p>");
    }
}

TEST_CASE("Documented enum members aren't generated for undocumented enums") {
    auto xml = generate(
        R"(
            enum Foo {
                /**
                 * This is the long description.
                 */
                BAR,
                BAZ
            }
        )"
    );

    SECTION("No documentation generated") {
        size_t count = xml->select_nodes("enum").size();
        REQUIRE(count == 0);
    }
}