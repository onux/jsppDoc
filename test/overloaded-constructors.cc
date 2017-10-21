#include "test.h"

TEST_CASE("Overloaded Constructor Name") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                public Foo() {}
                /**
                 */
                public Foo(int baz) {}
            }
        )"
    );

    SECTION("Ensure all overloads stored in one constructor node") {
        size_t count = xml->select_nodes("constructor").size();
        REQUIRE(count == 1);
    }
    SECTION("Constructor Name") {
        const std::string title = xml->child("constructor").child_value("title");
        REQUIRE(title == "Foo (Constructor)");
    }
    SECTION("Class Name") {
        const std::string className = xml->child("constructor").child_value("class");
        REQUIRE(className == "Foo");
    }
}

TEST_CASE("@overload tag with all overloaded constructors documented") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload Foo
                 */
                /**
                 */
                public Foo() {}
                /**
                 */
                public Foo(int baz) {}
            }
        )"
    );

    SECTION("Ensure all overloads stored in one constructor node") {
        size_t count = xml->select_nodes("constructor").size();
        REQUIRE(count == 1);
    }
    SECTION("Constructor Name") {
        const std::string title = xml->child("constructor").child_value("title");
        REQUIRE(title == "Foo (Constructor)");
    }
}

TEST_CASE("Overloaded Constructor Parameter Types") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                public Foo() {}
                /**
                 * @param baz ...
                 * @param qux ...
                 */
                public Foo(int baz, string() qux) {}
            }
        )"
    );

    SECTION("First overload has no parameters") {
        auto first_overload = xml->child("constructor").child("overload");
        REQUIRE(first_overload.child("params").select_nodes("param").size() == 0);
    }
    SECTION("Second overload has two parameters") {
        auto second_overload = std::next(
            xml->child("constructor").children("overload").begin()
        );
        REQUIRE(second_overload->child("params").select_nodes("param").size() == 2);
    }
    SECTION("Verify second overload's parameter types") {
        auto it = std::next(xml->child("constructor").children("overload").begin());

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.attribute("type").value()) == "int");
        REQUIRE(std::string(second_param.attribute("type").value()) == "string()");
    }
}

TEST_CASE("Overloaded Constructor Parameter Documentation - without @overload") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                public Foo() {}
                /**
                 * @param baz First param
                 * @param qux Second param
                 */
                public Foo(int baz, string qux) {}
            }
        )"
    );

    SECTION("Return type documentation text") {
        auto it = std::next(xml->child("constructor").children("overload").begin());

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "First param");
        REQUIRE(std::string(second_param.child_value()) == "Second param");
    }
}

TEST_CASE("@overload Constructor Parameter Documentation") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload Foo
                 *
                 * @param baz First param
                 * @param qux Second param
                 */
                /**
                 */
                public Foo(bool a, bool b) {}
                /**
                 * @param quux Third param
                 */
                public Foo(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("Documentation text for first overload") {
        auto it = xml->child("constructor").children("overload").begin();

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "First param");
        REQUIRE(std::string(second_param.child_value()) == "Second param");
    }

    SECTION("Documentation text for second overload") {
        auto it = xml->child("constructor").children("overload").begin();
        ++it;

        auto first_param = it->child("params").first_child();
        auto second_param = first_param.next_sibling("param");
        auto third_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "First param");
        REQUIRE(std::string(second_param.child_value()) == "Second param");
        REQUIRE(std::string(third_param.child_value()) == "Third param");
    }
}

TEST_CASE("@overload documentation gets cleared and doesn't get applied directly to the next constructor") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload Foo
                 *
                 * @param baz First param
                 * @param qux Second param
                 */
                public Foo(bool a, bool b) {}
                /**
                 * @param quux Third param
                 */
                public Foo(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("Return type documentation text for first overload") {
        auto it = xml->child("constructor").children("overload").begin();

        auto first_param = it->child("params").first_child();
        auto second_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "First param");
        REQUIRE(std::string(second_param.child_value()) == "Second param");
    }

    SECTION("Return type documentation text for second overload") {
        auto it = xml->child("constructor").children("overload").begin();
        ++it;

        auto first_param = it->child("params").first_child();
        auto second_param = first_param.next_sibling("param");
        auto third_param = it->child("params").last_child();
        REQUIRE(std::string(first_param.child_value()) == "First param");
        REQUIRE(std::string(second_param.child_value()) == "Second param");
        REQUIRE(std::string(third_param.child_value()) == "Third param");
    }
}

TEST_CASE("@overload tag with no specific overloaded constructors documented") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload Foo
                 */
                public Foo() {}
                public Foo(int baz) {}
            }
        )"
    );

    SECTION("Ensure all overloads stored in one constructor node") {
        size_t count = xml->select_nodes("constructor").size();
        REQUIRE(count == 1);
    }
    SECTION("Constructor Name") {
        const std::string title = xml->child("constructor").child_value("title");
        REQUIRE(title == "Foo (Constructor)");
    }
    SECTION("Ensure exactly two overloads documented") {
        size_t count = xml->child("constructor").select_nodes("overload").size();
        REQUIRE(count == 2);
    }
}

TEST_CASE("@overload Constructor @summary tag") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload Foo
                 *
                 * @summary Global summary for all
                 */
                /**
                 */
                public Foo(bool a, bool b) {}
                /**
                 */
                public Foo(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("@overload summary is applied to main <constructor> tag") {
        const std::string summary = xml->child("constructor").child("summary").child_value();
        REQUIRE(summary == "Global summary for all");
    }
}

TEST_CASE("Overloaded Constructor Modifiers") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 */
                public static Foo(bool a, bool b) {}
                /**
                 */
                private Foo(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("First overload has exactly two modifiers") {
        auto it = xml->child("constructor").children("overload").begin();
        size_t count = it->child("modifiers").select_nodes("modifier").size();
        REQUIRE(count == 2);
    }

    SECTION("Second overload has exactly one modifier") {
        auto it = xml->child("constructor").children("overload").begin();
        ++it;
        size_t count = it->child("modifiers").select_nodes("modifier").size();
        REQUIRE(count == 1);
    }

    SECTION("Modifiers of first overload") {
        auto it = xml->child("constructor").children("overload").begin();
        auto modifiers_it = it->child("modifiers").children("modifier").begin();

        const std::string modifier1 = modifiers_it->attribute("name").value();
        REQUIRE(modifier1 == "public");
        ++modifiers_it;
        const std::string modifier2 = modifiers_it->attribute("name").value();
        REQUIRE(modifier2 == "static");
    }

    SECTION("Modifiers of second overload") {
        auto it = xml->child("constructor").children("overload").begin();
        ++it;
        auto modifiers_it = it->child("modifiers").children("modifier").begin();

        const std::string modifier1 = modifiers_it->attribute("name").value();
        REQUIRE(modifier1 == "private");
    }
}

TEST_CASE("Overloaded Constructor Description - without @overload") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * First description
                 */
                public Foo(bool a, bool b) {}
                /**
                 * Second description
                 */
                public Foo(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("Description for first overload") {
        auto it = xml->child("constructor").children("overload").begin();
        const std::string description = it->child("description").child_value();
        REQUIRE(description == "First description");
    }

    SECTION("Description for second overload") {
        auto it = xml->child("constructor").children("overload").begin();
        ++it;
        const std::string description = it->child("description").child_value();
        REQUIRE(description == "Second description");
    }
}

TEST_CASE("@overload Constructor Description") {
    auto xml = generate(
        R"(
            class Foo
            {
                /**
                 * @overload Foo
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
                public Foo(bool a, bool b) {}
                /**
                 */
                public Foo(int baz, string qux, bool quux) {}
            }
        )"
    );

    SECTION("Description for first overload") {
        auto it = xml->child("constructor").children("overload").begin();
        const std::string description = it->child("description").child_value();
        REQUIRE(description == "This is the long description.\n\nSome more text goes here.");
    }

    SECTION("Description for second overload") {
        auto it = xml->child("constructor").children("overload").begin();
        ++it;
        const std::string description = it->child("description").child_value();
        REQUIRE(description == "This is the long description.\n\nSome more text goes here.");
    }
}