#include "test.h"

TEST_CASE("Constructor Name") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                public Foo(int baz) {}
            }
        )"
    );

    SECTION("Ensure only one constructor node gets documented") {
        size_t count = xml->select_nodes("constructor").size();
        REQUIRE(count == 1);
    }
    SECTION("Ensure only one 'overload' node gets documented") {
        size_t count = xml->child("constructor").select_nodes("overload").size();
        REQUIRE(count == 1);
    }
    SECTION("Constructor Name") {
        const std::string title = xml->child("constructor").child("title").child_value();
        REQUIRE(title == "Foo (Constructor)");
    }
    SECTION("Class Name") {
        const std::string className = xml->child("constructor").child_value("class");
        REQUIRE(className == "Foo");
    }
}

TEST_CASE("Undocumented constructor generates nothing") {
    auto xml = generate(
        R"(
            class Foo
            {
                public Foo(int baz) {}
            }
        )"
    );

    SECTION("No documentation generated") {
        size_t count = xml->select_nodes("constructor").size();
        REQUIRE(count == 0);
    }
}

TEST_CASE("Constructor Summary") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * This is the long description.
                 *
                 * @summary This is the summary.
                 */
                public Foo(int baz) {}
            }
        )"
    );

    SECTION("Summary") {
        const std::string summary = xml->child("constructor").child_value("summary");
        REQUIRE(summary == "This is the summary.");
    }
}

TEST_CASE("Constructor Description") {
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
                public Foo(int baz) {}
            }
        )"
    );

    SECTION("Description") {
        auto it = xml->child("constructor").children("overload").begin();
        const std::string description = it->child_value("description");
        REQUIRE(description == "This is the long description.\n\nSome more text goes here.");
    }
}

TEST_CASE("Constructor with No Parameters") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                public Foo() {}
            }
        )"
    );

    SECTION("First overload has no parameters") {
        auto constructor = xml->child("constructor").child("overload");
        REQUIRE(constructor.child("params").select_nodes("param").size() == 0);
    }
}

TEST_CASE("Constructor Parameter Types") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @param qux ...
                 * @param quux ...
                 */
                public Foo(int qux, string[] quux) {}
            }
        )"
    );

    SECTION("Constructor has two parameters") {
        auto constructor = xml->child("constructor").children("overload").begin();
        REQUIRE(constructor->child("params").select_nodes("param").size() == 2);
    }
    SECTION("Verify constructor's parameter types") {
        auto it = xml->child("constructor").children("overload").begin();

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.attribute("type").value()) == "int");
        REQUIRE(std::string(second_param.attribute("type").value()) == "string[]");
    }
}

TEST_CASE("Constructor Parameter Documentation") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @param baz First param
                 * @param qux Second param
                 */
                public Foo(int baz, string qux) {}
            }
        )"
    );

    SECTION("Return type documentation text") {
        auto it = xml->child("constructor").children("overload").begin();

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "First param");
        REQUIRE(std::string(second_param.child_value()) == "Second param");
    }
}

TEST_CASE("Constructor Modifiers") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                public static Foo(bool a, bool b) {}
            }
        )"
    );

    SECTION("Constructor has exactly two modifiers") {
        auto it = xml->child("constructor").children("overload").begin();
        size_t count = it->child("modifiers").select_nodes("modifier").size();
        REQUIRE(count == 2);
    }

    SECTION("Modifier names") {
        auto it = xml->child("constructor").children("overload").begin();
        auto modifiers_it = it->child("modifiers").children("modifier").begin();

        const std::string modifier1 = modifiers_it->attribute("name").value();
        REQUIRE(modifier1 == "public");
        ++modifiers_it;
        const std::string modifier2 = modifiers_it->attribute("name").value();
        REQUIRE(modifier2 == "static");
    }
}

TEST_CASE("Constructor @example tag") {
    auto xml = generate(
        R"(
            class Foo
            {
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
                public Foo(bool a, bool b) {}
            }
        )"
    );

    SECTION("Generates two <example> nodes") {
        auto constructor = xml->child("constructor").children("overload").begin();
        size_t count = constructor->child("examples").select_nodes("example").size();
        REQUIRE(count == 2);
    }

    SECTION("Example 1") {
        auto constructor = xml->child("constructor").children("overload").begin();
        auto example_it = constructor->child("examples").children("example").begin();

        const std::string title = example_it->attribute("name").value();
        REQUIRE(title == "First Example");

        const std::string code = example_it->child_value();
        REQUIRE(code == "import System;\n\nConsole.log(\"Example1\");");
    }

    SECTION("Example 2") {
        auto constructor = xml->child("constructor").children("overload").begin();
        auto example_it = constructor->child("examples").children("example").begin();
        ++example_it;

        const std::string title = example_it->attribute("name").value();
        REQUIRE(title == "Second Example");
        const std::string code = example_it->child_value();
        REQUIRE(code == "import System;\n\nConsole.log(\"Example2\");");
    }
}

TEST_CASE("Constructor @see tag") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @see Naming Conventions Developers/JavaScript-PP/Language-Guide/naming-conventions
                 */
                public Foo(bool a, bool b) {}
            }
        )"
    );

    SECTION("See Also") {
        auto it = xml->child("constructor").children("overload").begin();
        auto overload_it = it->child("see").children("ref").begin();

        const std::string ref_value = overload_it->child_value();
        REQUIRE(ref_value == "Naming Conventions");
        const std::string ref_tag = overload_it->attribute("to").value();
        REQUIRE(ref_tag == "Developers/JavaScript-PP/Language-Guide/naming-conventions");
    }
}