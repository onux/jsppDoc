#include "OutputBuilder.h"
#include "Data/DocCommentTags.h"
#include "Utils/Utils.h"
#include <cstddef>
#include <sundown.h>

using namespace jspp::docgen;

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
        tags.summary != "" ? tags.summary : utils::truncate(description, 250)
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
    this->addTitle(utils::escapeXML(comment.getTitle()));
    this->output << "<menu file=\"Developers\" />";
    this->output << "<modifiers>";
    this->addModifiers(*comment.getModifiers());
    this->output << "</modifiers>";
    this->addSummary(
        tags.summary != "" ? tags.summary : utils::truncate(description, 250)
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

void OutputBuilder::buildInterface(const InterfaceCommentData& comment) {
    const DocCommentTags& tags = comment.tags();
    const std::string description = comment.getBodyText();

    this->output << "<interface>";
    this->addTitle(utils::escapeXML(comment.getTitle()));
    this->output << "<menu file=\"Developers\" />";
    this->output << "<modifiers>";
    this->addModifiers(*comment.getModifiers());
    this->output << "</modifiers>";
    this->addSummary(
        tags.summary != "" ? tags.summary : utils::truncate(description, 250)
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
    this->output << "</interface>";
}

void OutputBuilder::buildFunctions(const MethodCommentData& comment) {
    const DocCommentTags& tags = comment.tags();

    this->output << "<method>";

    this->addTitle(comment.getName());
    this->output << "<menu file=\"Developers\" />";
    const std::string description = comment.getBodyText();
    const std::string summary = tags.summary;
    this->addSummary(summary != "" ? summary : utils::truncate(description, 250));

    this->output << "<overload>";
    /* #region: Individual <overload> Generation */
    {{
        this->addDescription(description);
        if (tags.deprecated_reason != "") {
            this->addDeprecated(tags.deprecated_reason);
        }

        this->output << "<modifiers>";
        this->addModifiers(*comment.getModifiers());
        this->output << "</modifiers>";

        this->output << "<return type=\"";
        this->output << utils::escapeXML(comment.getReturnType());
        this->output << "\">";
        this->output << cdata(markdown(tags.return_info));
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
        auto downcast = dynamic_cast<MethodCommentData*>(it->get());
        if (nullptr == downcast) continue;

        const MethodCommentData& overload  = *downcast;
        const DocCommentTags& overloadTags = overload.tags();

        this->output << "<overload>";

        this->addDescription(overload.getBodyText());
        if (overloadTags.deprecated_reason != "") {
            this->addDeprecated(overloadTags.deprecated_reason);
        }
        
        this->output << "<modifiers>";
        this->addModifiers(*overload.getModifiers());
        this->output << "</modifiers>";

        this->output << "<return type=\"";
        this->output << utils::escapeXML(overload.getReturnType());
        this->output << "\">";
        this->output << cdata(markdown(overloadTags.return_info));
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

void OutputBuilder::buildFunctions(const ConstructorCommentData& comment) {
    const DocCommentTags& tags = comment.tags();

    this->output << "<constructor>";

    const std::string className = comment.getClassName();
    this->addClass(className);
    this->addTitle(className + " (Constructor)");
    this->output << "<menu file=\"Developers\" />";
    const std::string description = comment.getBodyText();
    const std::string summary = tags.summary;
    this->addSummary(summary != "" ? summary : utils::truncate(description, 250));

    this->output << "<overload>";
    /* #region: Individual <overload> Generation */
    {{
        this->addDescription(description);
        if (tags.deprecated_reason != "") {
            this->addDeprecated(tags.deprecated_reason);
        }

        this->output << "<modifiers>";
        this->addModifiers(*comment.getModifiers());
        this->output << "</modifiers>";

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

    this->output << "</constructor>";
}

void OutputBuilder::buildFunctions(const OverloadedConstructorCommentData& comment) {
    this->output << "<constructor>";

    const std::string className = comment.getClassName();
    this->addClass(className);
    this->addTitle(className + " (Constructor)");
    this->output << "<menu file=\"Developers\" />";
    this->addSummary(comment.getSummary());

    auto& overloads = comment.getOverloads();
    auto it = overloads.begin(), end = overloads.end();
    for (; it != end; ++it) {
        auto downcast = dynamic_cast<ConstructorCommentData*>(it->get());
        if (nullptr == downcast) continue;

        const ConstructorCommentData& overload  = *downcast;
        const DocCommentTags& overloadTags = overload.tags();

        this->output << "<overload>";

        this->addDescription(overload.getBodyText());
        if (overloadTags.deprecated_reason != "") {
            this->addDeprecated(overloadTags.deprecated_reason);
        }
        
        this->output << "<modifiers>";
        this->addModifiers(*overload.getModifiers());
        this->output << "</modifiers>";

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
    this->output << "</constructor>";
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
        tags.summary != "" ? tags.summary : utils::truncate(description, 250)
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

void OutputBuilder::buildEnumeration(const EnumCommentData& comment) {
    const DocCommentTags& tags = comment.tags();
    const std::string description = comment.getBodyText();

    this->output << "<enum type=\"" << comment.getDatatype() << "\">";
    this->addTitle(comment.getName());
    this->output << "<menu file=\"Developers\" />";
    this->output << "<modifiers>";
    this->addModifiers(*comment.getModifiers());
    this->output << "</modifiers>";
    this->addSummary(
        tags.summary != "" ? tags.summary : utils::truncate(description, 250)
    );
    this->addDescription(description);
    if (tags.deprecated_reason != "") {
        this->addDeprecated(tags.deprecated_reason);
    }
    this->output << "<members>";
    for (const std::unique_ptr<EnumMemberCommentData>& member : comment.getMembers()) {
        const std::string identifier = member->getName();
        const std::string description = member->getBodyText();

        this->output << "<member name=\"" << identifier << "\">";
        this->output << description;
        this->output << "</member>";
    }
    this->output << "</members>";
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
    this->output << "</enum>";
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
    if (text == "") this->output << "...";
    else this->output << cdata(markdown(text));
    this->output << "</summary>";
}

void jspp::docgen::OutputBuilder::addDescription(const std::string& text) {
    this->output << "<description>";
    this->output << cdata(markdown(text));
    this->output << "</description>";
}

void jspp::docgen::OutputBuilder::addExample(const std::string& title, const std::string& code) {
    this->output << "<example name=\"" << utils::escapeXML(title) << "\">";
    this->output << cdata(code);
    this->output << "</example>";
}

void jspp::docgen::OutputBuilder::addDeprecated(const std::string& reason) {
    this->output << "<deprecated>";
    this->output << cdata(markdown(reason));
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

void jspp::docgen::OutputBuilder::addParameters(const OverloadableCommentData& commentData,
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
        this->output << cdata(markdown(param->description));
        this->output << "</param>";
    }
}

void jspp::docgen::OutputBuilder::addClass(const std::string& text) {
    this->output << "<class>";
    this->output << cdata(text);
    this->output << "</class>";
}

std::string OutputBuilder::markdown(const std::string& text) const {
    return utils::trimWhitespace(sundown::parse(utils::trimWhitespace(text)));
}