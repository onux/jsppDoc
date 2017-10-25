#include "test.h"

TEST_CASE("Class Name") {
    auto xml = generate(
        R"(
            /**
             */
            class Foo
            {
            }
        )"
    );

    SECTION("Ensure only one class node gets documented") {
        size_t count = xml->select_nodes("class").size();
        REQUIRE(count == 1);
    }
    SECTION("Class Name") {
        REQUIRE(std::string(xml->child("class").child_value("title")) == "Foo");
    }
}

TEST_CASE("Class Name - inside class") {
    auto xml = generate(
        R"(
            module Foo
            {
                /**
                 */
                class Bar
                {
                }
            }
        )"
    );

    SECTION("Ensure only one class node gets documented") {
        size_t count = xml->select_nodes("class").size();
        REQUIRE(count == 1);
    }
    SECTION("Class Name") {
        REQUIRE(std::string(xml->child("class").child_value("title")) == "Bar");
    }

    xml = generate(
        R"(
            module Foo.Bar.Baz
            {
                /**
                 */
                class Qux
                {
                }
            }
        )"
    );

    SECTION("Ensure only one class node gets documented") {
        size_t count = xml->select_nodes("class").size();
        REQUIRE(count == 1);
    }
    SECTION("Class Name") {
        REQUIRE(std::string(xml->child("class").child_value("title")) == "Qux");
    }

    xml = generate(
        R"(
            module Foo
            {
                module Bar
                {
                    /**
                     */
                    class Qux
                    {
                    }
                }
            }
        )"
    );

    SECTION("Ensure only one class node gets documented") {
        size_t count = xml->select_nodes("class").size();
        REQUIRE(count == 1);
    }
    SECTION("Class Name") {
        REQUIRE(std::string(xml->child("class").child_value("title")) == "Qux");
    }
}

TEST_CASE("Undocumented class generates nothing") {
    auto xml = generate(
        R"(
            class Foo
            {
            }
        )"
    );

    SECTION("No documentation generated") {
        size_t count = xml->select_nodes("class").size();
        REQUIRE(count == 0);
    }
}

TEST_CASE("Class Summary") {
    auto xml = generate(
        R"(
            /**
             * This is the long description.
             *
             * @summary This is the summary.
             */
            class Foo
            {
            }
        )"
    );

    SECTION("Summary") {
        std::string summary = xml->child("class").child_value("summary");
        REQUIRE(summary == "<p>This is the summary.</p>");
    }
}

TEST_CASE("Class Summary - If @summary tag is unavailable, use snippet of description") {
    auto xml = generate(
        R"(
            /**
             * This is the long description.
             *
             * Some more text goes here.
             */
            class Foo
            {
            }
        )"
    );

    SECTION("Summary") {
        std::string summary = xml->child("class").child_value("summary");
        REQUIRE(
            summary
            ==
            "<p>This is the long description.</p>"
            "\n\n"
            "<p>Some more text goes here.</p>"
        );
    }
}

TEST_CASE("Class Description") {
    auto xml = generate(
        R"(
            /**
             * This is the long description.
             *
             * Some more text goes here.
             *
             * @summary This is the summary.
             */
            class Foo
            {
            }
        )"
    );

    SECTION("Description") {
        std::string description = xml->child("class").child_value("description");
        REQUIRE(
            description
            ==
            "<p>This is the long description.</p>"
            "\n\n"
            "<p>Some more text goes here.</p>"
        );
    }
}

TEST_CASE("Deprecated Classes") {
    auto xml = generate(
        R"(
            /**
             * This is the long description.
             *
             * Some more text goes here.
             *
             * @deprecated This class is replaced by a new one.
             */
            class Foo
            {
            }
        )"
    );

    SECTION("Deprecated Reason") {
        std::string reason = xml->child("class").child_value("deprecated");
        REQUIRE(reason == "<p>This class is replaced by a new one.</p>");
    }
}

TEST_CASE("Class Examples") {
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
            class Foo
            {
            }
        )"
    );

    SECTION("Generates two <example> nodes") {
        size_t count = xml->child("class").child("examples").select_nodes("example").size();
        REQUIRE(count == 2);
    }

    SECTION("Example 1") {
        auto it = xml->child("class").child("examples").children("example").begin();

        std::string title = it->attribute("name").value();
        REQUIRE(title == "First Example");

        std::string code = it->child_value();
        REQUIRE(code == "import System;\n\nConsole.log(\"Example1\");");
    }

    SECTION("Example 2") {
        auto it = xml->child("class").child("examples").children("example").begin();
        ++it;

        std::string title = it->attribute("name").value();
        REQUIRE(title == "Second Example");
        std::string code = it->child_value();
        REQUIRE(code == "import System;\n\nConsole.log(\"Example2\");");
    }
}

TEST_CASE("Class See Also") {
    auto xml = generate(
        R"(
            /**
             * This is the long description.
             *
             * @see Naming Conventions Developers/JavaScript-PP/Language-Guide/naming-conventions
             * @see Scopes/Scoping https://docs.onux.com/en-US/Developers/JavaScript-PP/Language-Guide/scopes-scoping
             */
            class Foo
            {
            }
        )"
    );

    SECTION("See Also") {
        auto it = xml->child("class").child("see").children("ref").begin();
        std::string ref_tag1 = it->attribute("to").value();
        REQUIRE(ref_tag1 == "Developers/JavaScript-PP/Language-Guide/naming-conventions");

        ++it;
        std::string ref_tag2 = it->attribute("to").value();
        REQUIRE(ref_tag2 == "https://docs.onux.com/en-US/Developers/JavaScript-PP/Language-Guide/scopes-scoping");
    }
}

TEST_CASE("Class Modifiers") {
    auto xml = generate(
        R"(
            /**
             */
            public module Foo
            {
                private class Bar {}
            }
        )"
    );
    SECTION("Modifier count") {
        size_t count = xml->child("class").child("modifiers").select_nodes("modifier").size();
        REQUIRE(count == 1);
    }

    SECTION("Modifier name") {
        auto it = xml->child("class").child("modifiers").children("modifier").begin();
        std::string modifier = it->attribute("name").value();
        REQUIRE(modifier == "private");
    }

    xml = generate(
        R"(
            /**
             */
            protected class Bar {}
        )"
    );
    SECTION("Modifier count") {
        size_t count = xml->child("class").child("modifiers").select_nodes("modifier").size();
        REQUIRE(count == 1);
    }

    SECTION("Modifier name") {
        auto it = xml->child("class").child("modifiers").children("modifier").begin();
        std::string modifier = it->attribute("name").value();
        REQUIRE(modifier == "protected");
    }
}