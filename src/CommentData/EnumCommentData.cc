#include "EnumCommentData.h"

using namespace jspp::docgen;

EnumCommentData::EnumCommentData(const std::string& name,
                                 const std::string& fqn,
                                 const std::string& datatype,
                                 std::vector<std::unique_ptr<EnumMemberCommentData>>&& members,
                                 const std::string& docComment,
                                 const std::bitset<10>& modifiers)
    :
    NameMixin(name, fqn),
    ModifiersMixin(modifiers),
    DocumentationTextMixin(docComment),

    datatype(datatype),
    members(std::move(members))
{
}

std::string jspp::docgen::EnumCommentData::getDatatype() const {
    return this->datatype;
}

auto jspp::docgen::EnumCommentData::getMembers() const -> const members_t& {
    return this->members;
}