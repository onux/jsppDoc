#include "test.h"

TEST_CASE("Overload Name") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                void bar() {}
                /**
                 */
                void bar(int baz) {}
            }
        )"
    );

    SECTION("Ensure all overloads stored in one method node") {
        size_t count = xml->select_nodes("method").size();
        REQUIRE(count == 1);
    }
    SECTION("Method Name") {
        REQUIRE(std::string(xml->child("method").child_value("title")) == "bar");
    }
}

TEST_CASE("@overload tag with all overloads documented") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload bar
                 */
                /**
                 */
                void bar() {}
                /**
                 */
                void bar(int baz) {}
            }
        )"
    );

    SECTION("Ensure all overloads stored in one method node") {
        size_t count = xml->select_nodes("method").size();
        REQUIRE(count == 1);
    }
    SECTION("Method Name") {
        REQUIRE(std::string(xml->child("method").child_value("title")) == "bar");
    }
}

TEST_CASE("Overload Return Types") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                int bar() {}
                /**
                 */
                string bar() {}
            }
        )"
    );

    SECTION("First overload") {
        auto it = xml->child("method").children("overload").begin();
        REQUIRE(std::string(it->child("return").attribute("type").value()) == "int");
    }
    SECTION("Second overload") {
        auto it = std::next(xml->child("method").children("overload").begin());
        REQUIRE(std::string(it->child("return").attribute("type").value()) == "string");
    }
}

TEST_CASE("Overload Return Documentation - without @overload") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @return A number
                 */
                int bar() {}
                /**
                 */
                string bar() {}
            }
        )"
    );

    SECTION("First overload") {
        auto it = xml->child("method").children("overload").begin();
        REQUIRE(std::string(it->child("return").child_value()) == "<p>A number</p>");
    }
    SECTION("Second overload") {
        auto it = std::next(xml->child("method").children("overload").begin());
        REQUIRE(std::string(it->child("return").child_value()) == "");
    }
}

TEST_CASE("@overload Return Documentation") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload bar
                 * @return Some data
                 */
                /**
                 */
                int bar() {}
                /**
                 */
                string bar() {}
            }
        )"
    );

    SECTION("First overload") {
        auto it = xml->child("method").children("overload").begin();
        REQUIRE(std::string(it->child("return").child_value()) == "<p>Some data</p>");
    }
    SECTION("Second overload") {
        auto it = std::next(xml->child("method").children("overload").begin());
        REQUIRE(std::string(it->child("return").child_value()) == "<p>Some data</p>");
    }
}

TEST_CASE("Overload Parameter Types") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                void bar() {}
                /**
                 * @param baz ...
                 * @param qux ...
                 */
                void bar(int baz, ...string qux) {}
            }
        )"
    );

    SECTION("First overload has no parameters") {
        auto first_overload = xml->child("method").child("overload");
        REQUIRE(first_overload.child("params").select_nodes("param").size() == 0);
    }
    SECTION("Second overload has two parameters") {
        auto second_overload = std::next(
            xml->child("method").children("overload").begin()
        );
        REQUIRE(second_overload->child("params").select_nodes("param").size() == 2);
    }
    SECTION("Verify second overload's parameter types") {
        auto it = std::next(xml->child("method").children("overload").begin());

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.attribute("type").value()) == "int");
        REQUIRE(std::string(second_param.attribute("type").value()) == "...string");
    }
}

TEST_CASE("Overload Parameter Documentation - without @overload") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                void bar() {}
                /**
                 * @param baz First param
                 * @param qux Second param
                 */
                void bar(int baz, string qux) {}
            }
        )"
    );

    SECTION("Return type documentation text") {
        auto it = std::next(xml->child("method").children("overload").begin());

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "<p>First param</p>");
        REQUIRE(std::string(second_param.child_value()) == "<p>Second param</p>");
    }
}

TEST_CASE("@overload Parameter Documentation") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload bar
                 *
                 * @param baz First param
                 * @param qux Second param
                 */
                /**
                 */
                void bar(bool a, bool b) {}
                /**
                 * @param quux Third param
                 */
                void bar(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("Return type documentation text for first overload") {
        auto it = xml->child("method").children("overload").begin();

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "<p>First param</p>");
        REQUIRE(std::string(second_param.child_value()) == "<p>Second param</p>");
    }

    SECTION("Return type documentation text for second overload") {
        auto it = xml->child("method").children("overload").begin();
        ++it;

        auto first_param = it->child("params").first_child();
        auto second_param = first_param.next_sibling("param");
        auto third_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "<p>First param</p>");
        REQUIRE(std::string(second_param.child_value()) == "<p>Second param</p>");
        REQUIRE(std::string(third_param.child_value()) == "<p>Third param</p>");
    }
}

TEST_CASE("Regression: @overload documentation gets cleared and doesn't get applied directly to the next method") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload bar
                 *
                 * @param baz First param
                 * @param qux Second param
                 */
                void bar(bool a, bool b) {}
                /**
                 * @param quux Third param
                 */
                void bar(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("Return type documentation text for first overload") {
        auto it = xml->child("method").children("overload").begin();

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "<p>First param</p>");
        REQUIRE(std::string(second_param.child_value()) == "<p>Second param</p>");
    }

    SECTION("Return type documentation text for second overload") {
        auto it = xml->child("method").children("overload").begin();
        ++it;

        auto first_param = it->child("params").first_child();
        auto second_param = first_param.next_sibling("param");
        auto third_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "<p>First param</p>");
        REQUIRE(std::string(second_param.child_value()) == "<p>Second param</p>");
        REQUIRE(std::string(third_param.child_value()) == "<p>Third param</p>");
    }
}

TEST_CASE("@overload tag with no specific overloads documented") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload bar
                 */
                void bar() {}
                void bar(int baz) {}
            }
        )"
    );

    SECTION("Ensure all overloads stored in one method node") {
        size_t count = xml->select_nodes("method").size();
        REQUIRE(count == 1);
    }
    SECTION("Method Name") {
        REQUIRE(std::string(xml->child("method").child_value("title")) == "bar");
    }
    SECTION("Ensure exactly two overloads documented") {
        size_t count = xml->child("method").select_nodes("overload").size();
        REQUIRE(count == 2);
    }
}

TEST_CASE("@overload @summary tag") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload bar
                 *
                 * @summary Global summary for all
                 */
                /**
                 */
                void bar(bool a, bool b) {}
                /**
                 */
                void bar(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("@overload summary is applied to main <method> tag") {
        const std::string summary = xml->child("method").child("summary").child_value();
        REQUIRE(summary == "<p>Global summary for all</p>");
    }
}

TEST_CASE("Overloaded Method Modifiers") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                public static void bar(bool a, bool b) {}
                /**
                 */
                private void bar(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("First overload has exactly two modifiers") {
        auto it = xml->child("method").children("overload").begin();
        size_t count = it->child("modifiers").select_nodes("modifier").size();
        REQUIRE(count == 2);
    }

    SECTION("Second overload has exactly one modifier") {
        auto it = xml->child("method").children("overload").begin();
        ++it;
        size_t count = it->child("modifiers").select_nodes("modifier").size();
        REQUIRE(count == 1);
    }

    SECTION("Modifiers of first overload") {
        auto it = xml->child("method").children("overload").begin();
        auto modifiers_it = it->child("modifiers").children("modifier").begin();

        const std::string modifier1 = modifiers_it->attribute("name").value();
        REQUIRE(modifier1 == "public");
        ++modifiers_it;
        const std::string modifier2 = modifiers_it->attribute("name").value();
        REQUIRE(modifier2 == "static");
    }

    SECTION("Modifiers of second overload") {
        auto it = xml->child("method").children("overload").begin();
        ++it;
        auto modifiers_it = it->child("modifiers").children("modifier").begin();

        const std::string modifier1 = modifiers_it->attribute("name").value();
        REQUIRE(modifier1 == "private");
    }
}

TEST_CASE("@overload @example tag") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload baz
                 *
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
                /**
                 */
                void baz(bool a, bool b) {}
                /**
                 */
                void baz(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("Generates two <example> nodes") {
        auto it = xml->child("method").children("overload").begin();
        size_t count1 = it->child("examples").select_nodes("example").size();
        REQUIRE(count1 == 2);

        ++it;
        size_t count2 = it->child("examples").select_nodes("example").size();
        REQUIRE(count2 == 2);
    }

    SECTION("Example 1 for First Overload") {
        auto overload_it = xml->child("method").children("overload").begin();
        auto example_it = overload_it->child("examples").children("example").begin();

        const std::string title = example_it->attribute("name").value();
        REQUIRE(title == "First Example");
        const std::string code = example_it->child_value();
        REQUIRE(code == "import System;\n\nConsole.log(\"Example1\");");
    }

    SECTION("Example 2 for First Overload") {
        auto overload_it = xml->child("method").children("overload").begin();
        auto example_it = overload_it->child("examples").children("example").begin();
        ++example_it;

        const std::string title = example_it->attribute("name").value();
        REQUIRE(title == "Second Example");
        const std::string code = example_it->child_value();
        REQUIRE(code == "import System;\n\nConsole.log(\"Example2\");");
    }

    SECTION("Example 1 for Second Overload") {
        auto overload_it = xml->child("method").children("overload").begin();
        ++overload_it;
        auto example_it = overload_it->child("examples").children("example").begin();

        const std::string title = example_it->attribute("name").value();
        REQUIRE(title == "First Example");
        const std::string code = example_it->child_value();
        REQUIRE(code == "import System;\n\nConsole.log(\"Example1\");");
    }

    SECTION("Example 2 for Second Overload") {
        auto overload_it = xml->child("method").children("overload").begin();
        ++overload_it;
        auto example_it = overload_it->child("examples").children("example").begin();
        ++example_it;

        const std::string title = example_it->attribute("name").value();
        REQUIRE(title == "Second Example");
        const std::string code = example_it->child_value();
        REQUIRE(code == "import System;\n\nConsole.log(\"Example2\");");
    }
}

TEST_CASE("@overload @see tag") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload bar
                 *
                 * @see Naming Conventions Developers/JavaScript-PP/Language-Guide/naming-conventions
                 */
                /**
                 */
                void bar(bool a, bool b) {}
                /**
                 */
                void bar(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("See Also 1") {
        auto it = xml->child("method").children("overload").begin();
        auto overload_it = it->child("see").children("ref").begin();

        const std::string ref_value = overload_it->child_value();
        REQUIRE(ref_value == "Naming Conventions");
        const std::string ref_tag = overload_it->attribute("to").value();
        REQUIRE(ref_tag == "Developers/JavaScript-PP/Language-Guide/naming-conventions");
    }

    SECTION("See Also 2") {
        auto it = xml->child("method").children("overload").begin();
        ++it;
        auto overload_it = it->child("see").children("ref").begin();

        const std::string ref_value = overload_it->child_value();
        REQUIRE(ref_value == "Naming Conventions");
        const std::string ref_tag = overload_it->attribute("to").value();
        REQUIRE(ref_tag == "Developers/JavaScript-PP/Language-Guide/naming-conventions");
    }
}

TEST_CASE("Overload Description - without @overload") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * First description
                 */
                void baz(bool a, bool b) {}
                /**
                 * Second description
                 */
                void baz(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("Description for first overload") {
        auto it = xml->child("method").children("overload").begin();
        const std::string description = it->child("description").child_value();
        REQUIRE(description == "<p>First description</p>");
    }

    SECTION("Description for second overload") {
        auto it = xml->child("method").children("overload").begin();
        ++it;
        const std::string description = it->child("description").child_value();
        REQUIRE(description == "<p>Second description</p>");
    }
}

TEST_CASE("@overload Description") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload baz
                 *
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
                /**
                 */
                void baz(bool a, bool b) {}
                /**
                 */
                void baz(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("Description for first overload") {
        auto it = xml->child("method").children("overload").begin();
        const std::string description = it->child("description").child_value();
        REQUIRE(
            description
            ==
            "<p>This is the long description.</p>"
            "\n\n"
            "<p>Some more text goes here.</p>"
        );
    }

    SECTION("Description for second overload") {
        auto it = xml->child("method").children("overload").begin();
        ++it;
        const std::string description = it->child("description").child_value();
        REQUIRE(
            description
            ==
            "<p>This is the long description.</p>"
            "\n\n"
            "<p>Some more text goes here.</p>"
        );
    }
}