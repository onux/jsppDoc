#include "test.h"
#include <vector>

TEST_CASE("jspp::docgen::CommentParser") {
    jspp::docgen::CommentParser parser;

    SECTION("parseDocCommentText") {
        REQUIRE(
            parser.parseDocCommentText(
                R"(
                    /*
                     * This is not a doc comment.
                     */
                )"
            )
            ==
            ""
        );
        REQUIRE(
            parser.parseDocCommentText(
                R"(
                    /**
                     * This *IS* a doc comment.
                     */
                )"
            )
            ==
            " This *IS* a doc comment."
        );
        REQUIRE(
            parser.parseDocCommentText(
                R"(
                    /** This *IS* a doc comment. */
                )"
            )
            ==
            "This *IS* a doc comment."
        );
    }

    SECTION("parseDocCommentTags - @overload") {
        std::unique_ptr<jspp::docgen::DocCommentTags> tags = parser.parseDocCommentTags(
            "@overload foobar"
        );
        REQUIRE(tags->overload == "foobar");
    }

    SECTION("parseDocCommentTags - @deprecated") {
        std::unique_ptr<jspp::docgen::DocCommentTags> tags1 = parser.parseDocCommentTags(
            "@deprecated"
        );
        REQUIRE(tags1->isDeprecated);
        REQUIRE(tags1->deprecated_reason == "");

        std::unique_ptr<jspp::docgen::DocCommentTags> tags2 = parser.parseDocCommentTags(
            "@deprecated Deprecation reason"
        );
        REQUIRE(tags2->isDeprecated);
        REQUIRE(tags2->deprecated_reason == "Deprecation reason");

        std::unique_ptr<jspp::docgen::DocCommentTags> tags3 = parser.parseDocCommentTags(
            "@summary This is the summary.\n"
            "\n"
            "@deprecated\n"
            "\n"
            "@return This is some description."
        );
        REQUIRE(tags3->isDeprecated);
        REQUIRE(tags3->deprecated_reason == "");
    }

    SECTION("parseDocCommentTags - all other tags") {
        std::unique_ptr<jspp::docgen::DocCommentTags> tags = parser.parseDocCommentTags(
            "@summary This is the summary.\n"
            "\n"
            "@param x The X coordinate.\n"
            "@param y The Y coordinate.\n"
            "\n"
            "@return The coordinate object.\n"
            "\n"
            "@see CoordinateXYZ path/to/page1\n"
            "@see MyApp.CoordinateZ https://www.onux.com/\n"
            "\n"
            "@example Basic Usage\n"
            "\n"
            "auto coord = new Coordinate(100, 100);\n"
            "\n"
            "Console.log(coord.getX());\n"
            "Console.log(coord.getY());\n"
            "\n"
            "@example With explicit type annotation\n"
            "Coordinate coord = new Coordinate(100, 100);\n"
        );

        REQUIRE(tags->summary == "This is the summary.");

        REQUIRE(tags->params.size() == 2);
        REQUIRE(tags->params.at(0)->name == "x");
        REQUIRE(tags->params.at(0)->description == "The X coordinate.");
        REQUIRE(tags->params.at(1)->name == "y");
        REQUIRE(tags->params.at(1)->description == "The Y coordinate.");

        REQUIRE(tags->return_info == "The coordinate object.");

        REQUIRE(tags->see_also.size() == 2);
        REQUIRE(tags->see_also.at(0)->title == "CoordinateXYZ");
        REQUIRE(tags->see_also.at(0)->path == "path/to/page1");
        REQUIRE(tags->see_also.at(1)->title == "MyApp.CoordinateZ");
        REQUIRE(tags->see_also.at(1)->path == "https://www.onux.com/");

        REQUIRE(tags->examples.size() == 2);
        REQUIRE(tags->examples.at(0)->title == "Basic Usage");
        REQUIRE(
            tags->examples.at(0)->code
            ==
            "auto coord = new Coordinate(100, 100);\n"
            "\n"
            "Console.log(coord.getX());\n"
            "Console.log(coord.getY());"
        );
        REQUIRE(tags->examples.at(1)->title == "With explicit type annotation");
        REQUIRE(
            tags->examples.at(1)->code
            ==
            "Coordinate coord = new Coordinate(100, 100);"
        );
    }

    SECTION("parseDocCommentBodyText") {
        std::string body1 = parser.parseDocCommentBodyText(
            parser.parseDocCommentText(
                R"(
                /** One line */
                )"
            )
        );
        REQUIRE(body1 == "One line");

        std::string body2 = parser.parseDocCommentBodyText(
            parser.parseDocCommentText(
                R"(
                /**
                One line
                */
                )"
            )
        );
        REQUIRE(body2 == "One line");

        std::string body3 = parser.parseDocCommentBodyText(
            parser.parseDocCommentText(
                R"(
                /**
                * One line
                */
                )"
            )
        );
        REQUIRE(body3 == "One line");

        std::string body4 = parser.parseDocCommentBodyText(
            parser.parseDocCommentText(
                R"(
                /**
                 * My description goes here.
                 *
                 * More description.
                 *
                 * More text. Even more text.
                 *
                 * And even more text.
                 */
                )"
            )
        );

        REQUIRE(
            body4
            ==
            "My description goes here.\n"
            "\n"
            "More description.\n"
            "\n"
            "More text. Even more text.\n"
            "\n"
            "And even more text."
        );

        std::string body5 = parser.parseDocCommentBodyText(
            parser.parseDocCommentText(
                "/**\n"
                "My description goes here.\n"
                "\n"
                "More description.\n"
                "More text.\n"
                "Even more text.\n"
                "\n"
                "And even more text.\n"
                "*/"
            )
        );

        REQUIRE(
            body5
            ==
            "My description goes here.\n"
            "\n"
            "More description.\n"
            "More text.\n"
            "Even more text.\n"
            "\n"
            "And even more text."
        );

        std::string body6 = parser.parseDocCommentBodyText(
            parser.parseDocCommentText(
                R"(
                /**
                 * My description.
                 *
                 * ... Before the first tag.
                 *
                 * @return Some tag info.
                 */
                )"
            )
        );
        REQUIRE(
            body6
            ==
            "My description.\n"
            "\n"
            "... Before the first tag."
        );

        std::string body7 = parser.parseDocCommentBodyText(
            parser.parseDocCommentText(
                R"(
                /**
                 * My description.
                 * @return Some tag info.
                 */
                )"
            )
        );
        REQUIRE(body7 == "My description.");
    }
}