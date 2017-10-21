#include "test.h"

TEST_CASE("Method Name") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                void bar(int baz) {}
            }
        )"
    );

    SECTION("Ensure only one method node gets documented") {
        size_t count = xml->select_nodes("method").size();
        REQUIRE(count == 1);
    }
    SECTION("Ensure only one 'overload' node gets documented") {
        size_t count = xml->child("method").select_nodes("overload").size();
        REQUIRE(count == 1);
    }
    SECTION("Method Name") {
        REQUIRE(std::string(xml->child("method").child("title").child_value()) == "bar");
    }
}

TEST_CASE("Undocumented method generates nothing") {
    auto xml = generate(
        R"(
            class Foo
            {
                void bar(int baz) {}
            }
        )"
    );

    SECTION("No documentation generated") {
        size_t count = xml->select_nodes("method").size();
        REQUIRE(count == 0);
    }
}

TEST_CASE("Method Summary") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * This is the long description.
                 *
                 * @summary This is the summary.
                 */
                void bar(int baz) {}
            }
        )"
    );

    SECTION("Summary") {
        const std::string summary = xml->child("method").child_value("summary");
        REQUIRE(summary == "This is the summary.");
    }
}

TEST_CASE("Method Description") {
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
                void bar(int baz) {}
            }
        )"
    );

    SECTION("Description") {
        auto it = xml->child("method").children("overload").begin();
        const std::string description = it->child_value("description");
        REQUIRE(description == "This is the long description.\n\nSome more text goes here.");
    }
}

TEST_CASE("Method Return Type") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                unsigned int bar() {}
            }
        )"
    );

    SECTION("First overload") {
        auto it = xml->child("method").children("overload").begin();
        REQUIRE(std::string(it->child("return").attribute("type").value()) == "unsigned int");
    }
}

TEST_CASE("Method with No Parameters") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                void baz() {}
            }
        )"
    );

    SECTION("First overload has no parameters") {
        auto method = xml->child("method").child("overload");
        REQUIRE(method.child("params").select_nodes("param").size() == 0);
    }
}

TEST_CASE("Method Parameter Types") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @param qux ...
                 * @param quux ...
                 */
                void baz(int qux, ...string quux) {}
            }
        )"
    );

    SECTION("Method has two parameters") {
        auto method = xml->child("method").children("overload").begin();
        REQUIRE(method->child("params").select_nodes("param").size() == 2);
    }
    SECTION("Verify method's parameter types") {
        auto it = xml->child("method").children("overload").begin();

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.attribute("type").value()) == "int");
        REQUIRE(std::string(second_param.attribute("type").value()) == "...string");
    }
}

TEST_CASE("Method Parameter Documentation") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @param baz First param
                 * @param qux Second param
                 */
                void bar(int baz, string qux) {}
            }
        )"
    );

    SECTION("Return type documentation text") {
        auto it = xml->child("method").children("overload").begin();

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "First param");
        REQUIRE(std::string(second_param.child_value()) == "Second param");
    }
}

TEST_CASE("Method @example tag") {
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
                void baz(bool a, bool b) {}
            }
        )"
    );

    SECTION("Generates two <example> nodes") {
        auto method = xml->child("method").children("overload").begin();
        size_t count = method->child("examples").select_nodes("example").size();
        REQUIRE(count == 2);
    }

    SECTION("Example 1") {
        auto method = xml->child("method").children("overload").begin();
        auto example_it = method->child("examples").children("example").begin();

        const std::string title = example_it->attribute("name").value();
        REQUIRE(title == "First Example");

        const std::string code = example_it->child_value();
        REQUIRE(code == "import System;\n\nConsole.log(\"Example1\");");
    }

    SECTION("Example 2") {
        auto method = xml->child("method").children("overload").begin();
        auto example_it = method->child("examples").children("example").begin();
        ++example_it;

        const std::string title = example_it->attribute("name").value();
        REQUIRE(title == "Second Example");
        const std::string code = example_it->child_value();
        REQUIRE(code == "import System;\n\nConsole.log(\"Example2\");");
    }
}

TEST_CASE("Method @see tag") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @see Naming Conventions Developers/JavaScript-PP/Language-Guide/naming-conventions
                 */
                void bar(bool a, bool b) {}
            }
        )"
    );

    SECTION("See Also") {
        auto it = xml->child("method").children("overload").begin();
        auto overload_it = it->child("see").children("ref").begin();

        const std::string ref_value = overload_it->child_value();
        REQUIRE(ref_value == "Naming Conventions");
        const std::string ref_tag = overload_it->attribute("to").value();
        REQUIRE(ref_tag == "Developers/JavaScript-PP/Language-Guide/naming-conventions");
    }
}