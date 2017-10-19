#include "OutputBuilder.h"
#include "DocCommentTags.h"
#include "Utils.h"
#include <cstddef>

using namespace jspp::docgen;

static std::string truncate(const std::string& s, size_t count);

std::string OutputBuilder::getOutput() const {
    return this->output.str();
}

void OutputBuilder::buildModule(const ModuleCommentData& comment) {
    const DocCommentTags& tags = comment.tags();
    const std::string description = comment.getBodyText();

    this->output << "<module>";
    this->addTitle(comment.getName());
    this->output << "<menu file=\"Developers\" />";
    this->output << "<modifiers>";
    this->addModifiers(*comment.getModifiers());
    this->output << "</modifiers>";
    this->addSummary(
        tags.summary != "" ? tags.summary : truncate(description, 250)
    );
    this->addDescription(description);
    if (tags.deprecated_reason != "") {
        this->addDeprecated(tags.deprecated_reason);
    }
    this->output << "<examples>";
    for(auto& example : tags.examples) {
        this->addExample(example->title, example->code);
    }
    this->output << "</examples>";
    this->output << "<see>";
    for (auto& see : tags.see_also) {
        this->addSeeAlso(see->title, see->path);
    }
    this->output << "</see>";
    this->output << "</module>";
}

void OutputBuilder::buildClass(const ClassCommentData& comment) {
    const DocCommentTags& tags = comment.tags();
    const std::string description = comment.getBodyText();

    this->output << "<class>";
    this->addTitle(comment.getName());
    this->output << "<menu file=\"Developers\" />";
    this->output << "<modifiers>";
    this->addModifiers(*comment.getModifiers());
    this->output << "</modifiers>";
    this->addSummary(
        tags.summary != "" ? tags.summary : truncate(description, 250)
    );
    this->addDescription(description);
    if (tags.deprecated_reason != "") {
        this->addDeprecated(tags.deprecated_reason);
    }
    this->output << "<examples>";
    for(auto& example : tags.examples) {
        this->addExample(example->title, example->code);
    }
    this->output << "</examples>";
    this->output << "<see>";
    for (auto& see : tags.see_also) {
        this->addSeeAlso(see->title, see->path);
    }
    this->output << "</see>";
    this->output << "</class>";
}

void OutputBuilder::buildFunctions(const MethodCommentData& comment) {
    const DocCommentTags& tags = comment.tags();

    this->output << "<method>";

    this->addTitle(comment.getName());
    this->output << "<menu file=\"Developers\" />";
    const std::string description = comment.getBodyText();
    const std::string summary = tags.summary;
    this->addSummary(summary != "" ? summary : truncate(description, 250));

    this->output << "<overload>";
    /* #region: Individual <overload> Generation */
    {{
        this->addDescription(description);
        if (tags.deprecated_reason != "") {
            this->addDeprecated(tags.deprecated_reason);
        }

        this->output << "<return type=\"";
        this->output << utils::escapeXML(comment.getReturnType());
        this->output << "\">";
        this->output << cdata(tags.return_info);
        this->output << "</return>";

        this->output << "<params>";
        this->addParameters(comment, tags);
        this->output << "</params>";

        this->output << "<examples>";
        for(auto& example : tags.examples) {
            this->addExample(example->title, example->code);
        }
        this->output << "</examples>";

        this->output << "<see>";
        for (auto& see : tags.see_also) {
            this->addSeeAlso(see->title, see->path);
        }
        this->output << "</see>";
    }}
    this->output << "</overload>";

    this->output << "</method>";
}

void OutputBuilder::buildFunctions(const OverloadedMethodCommentData& comment) {
    this->output << "<method>";
    this->addTitle(comment.getName());
    this->output << "<menu file=\"Developers\" />";
    this->addSummary(comment.getSummary());

    auto& overloads = comment.getOverloads();
    auto it = overloads.begin(), end = overloads.end();
    for (; it != end; ++it) {
        const MethodCommentData& overload  = **it;
        const DocCommentTags& overloadTags = overload.tags();

        this->output << "<overload>";

        this->addDescription(overload.getBodyText());
        if (overloadTags.deprecated_reason != "") {
            this->addDeprecated(overloadTags.deprecated_reason);
        }

        this->output << "<return type=\"";
        this->output << utils::escapeXML(overload.getReturnType());
        this->output << "\">";
        this->output << cdata(overloadTags.return_info);
        this->output << "</return>";

        this->output << "<params>";
        this->addParameters(overload, overloadTags);
        this->output << "</params>";

        this->output << "<examples>";
        for(auto& example : overloadTags.examples) {
            this->addExample(example->title, example->code);
        }
        this->output << "</examples>";
        this->output << "<see>";
        for (auto& see : overloadTags.see_also) {
            this->addSeeAlso(see->title, see->path);
        }
        this->output << "</see>";
        this->output << "</overload>";
    }
    this->output << "</method>";
}


void OutputBuilder::buildField(const FieldCommentData& comment) {
    const DocCommentTags& tags = comment.tags();
    const std::string description = comment.getBodyText();

    this->output << "<field>";
    this->addTitle(comment.getName());
    this->output << "<menu file=\"Developers\" />";
    this->output << "<modifiers>";
    this->addModifiers(*comment.getModifiers());
    this->output << "</modifiers>";
    this->addSummary(
        tags.summary != "" ? tags.summary : truncate(description, 250)
    );
    this->addDescription(description);
    if (tags.deprecated_reason != "") {
        this->addDeprecated(tags.deprecated_reason);
    }
    this->output << "<examples>";
    for(auto& example : tags.examples) {
        this->addExample(example->title, example->code);
    }
    this->output << "</examples>";
    this->output << "<see>";
    for (auto& see : tags.see_also) {
        this->addSeeAlso(see->title, see->path);
    }
    this->output << "</see>";
    this->output << "</field>";
}

std::string jspp::docgen::OutputBuilder::cdata(const std::string& s) const {
    return "<![CDATA[" + s + "]]>";
}

void jspp::docgen::OutputBuilder::addTitle(const std::string& title) {
    this->output << "<title>";
    this->output << cdata(title);
    this->output << "</title>";
}

void jspp::docgen::OutputBuilder::addSummary(const std::string& text) {
    this->output << "<summary>";
    this->output << cdata(text);
    this->output << "</summary>";
}

void jspp::docgen::OutputBuilder::addDescription(const std::string& text) {
    this->output << "<description>";
    this->output << cdata(text);
    this->output << "</description>";
}

void jspp::docgen::OutputBuilder::addExample(const std::string& title, const std::string& code) {
    this->output << "<example name=\"" << utils::escapeXML(title) << "\">";
    this->output << cdata(code);
    this->output << "</example>";
}

void jspp::docgen::OutputBuilder::addDeprecated(const std::string& reason) {
    this->output << "<deprecated>";
    this->output << cdata(reason);
    this->output << "</deprecated>";
}

void jspp::docgen::OutputBuilder::addSeeAlso(const std::string& title, const std::string& page) {
    this->output << "<ref to=\"" << page << "\">";
    this->output << cdata(title);
    this->output << "</ref>";
}

void jspp::docgen::OutputBuilder::addModifiers(const jspp::docgen::Modifiers& modifiers) {
    if (modifiers.isPublic) {
        this->output << "<modifier name=\"public\" />";
    }
    if (modifiers.isProtected) {
        this->output << "<modifier name=\"protected\" />";
    }
    if (modifiers.isPrivate) {
        this->output << "<modifier name=\"private\" />";
    }
    if (modifiers.isStatic) {
        this->output << "<modifier name=\"static\" />";
    }
    if (modifiers.isFinal) {
        this->output << "<modifier name=\"final\" />";
    }
    if (modifiers.isInline) {
        this->output << "<modifier name=\"inline\" />";
    }
    if (modifiers.isProperty) {
        this->output << "<modifier name=\"property\" />";
    }
    if (modifiers.isAbstract) {
        this->output << "<modifier name=\"abstract\" />";
    }
    if (modifiers.isVirtual) {
        this->output << "<modifier name=\"virtual\" />";
    }
    if (modifiers.isOverride) {
        this->output << "<modifier name=\"override\" />";
    }
}

void jspp::docgen::OutputBuilder::addParameters(const MethodCommentData& commentData,
                                                const DocCommentTags& tags) {
    auto it  = tags.params.cbegin();
    auto end = tags.params.cend();
    for (; it != end; ++it) {
        auto& param = *it;
        ptrdiff_t index = it - tags.params.cbegin();

        this->output << "<param name=\"";
        this->output << param->name;
        this->output << "\" ";
        this->output << "type=\"";
        this->output << utils::escapeXML(commentData.getParameterType(index));
        this->output << "\">";
        this->output << cdata(param->description);
        this->output << "</param>";
    }
}

static std::string truncate(const std::string& s, size_t count) {
    std::vector<std::string> lines = utils::splitLines(s);
    if (lines.size() == 0) {
        return "";
    }

    std::string firstLine = lines[0];
    if (firstLine.size() <= count) {
        return firstLine;
    }
    else {
        firstLine.resize(count);
        firstLine += "...";
        return firstLine;
    }
}