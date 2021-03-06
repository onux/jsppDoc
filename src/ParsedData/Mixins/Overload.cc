#include "Overload.h"
#include "../../Utils/Utils.h"

using namespace jspp::docgen;

OverloadMixin::OverloadMixin(methods_t methods)
                            :
                            summary(this->extractSummary(methods)),
                            overloads(std::move(methods))
{
}

OverloadMixin::OverloadMixin(methods_t methods,
                             overload_tag_t overload_tag)
                            :
                            summary(this->extractSummary(methods, overload_tag)),
                            overloads(std::move(methods))
{
    this->mergeOverloadedDocs(*overload_tag);
}

const std::string OverloadMixin::getSummary() const {
    return this->summary;
}

auto OverloadMixin::getOverloads() const -> const methods_t& {
    return this->overloads;
}

const std::string OverloadMixin::extractSummary(const methods_t& methods) const {
    return this->extractSummary(methods, nullptr);
}

const std::string OverloadMixin::extractSummary(const methods_t& methods,
                                                overload_tag_t overload_tag) const
{
    std::string result;

    if (nullptr != overload_tag) {
        const DocCommentTags& tags = overload_tag->tags();
        result = tags.summary;
        if (result != "") {
            return result;
        }

        const std::string shared_description = overload_tag->getBodyText();
        result = utils::truncate(shared_description, 250);
        if (result != "") {
            return result;
        }
    }

    for (const std::unique_ptr<OverloadableCommentData>& method : methods) {
        const DocCommentTags& tags = method->tags();
        result = tags.summary;
        if (result != "") return result;
    }


    return "";
}

void OverloadMixin::mergeOverloadedDocs(const OverloadTagCommentData& overload_tag) const {
    const DocCommentTags& sharedData = overload_tag.tags();

    for (auto& method : this->overloads) {
        DocCommentTags& methodData = method->writableTags();

        const std::string shared_description = overload_tag.getBodyText();
        if (method->getBodyText() == "") {
            method->setBodyText(shared_description);
        }

        if (methodData.return_info == "") {
            methodData.return_info = sharedData.return_info;
        }

        const bool overwriteDeprecated =
            (   !methodData.isDeprecated
                &&
                methodData.deprecated_reason == ""
            )
            &&
            (   sharedData.isDeprecated
                &&
                sharedData.deprecated_reason != ""
            );

        if (overwriteDeprecated) {
            methodData.isDeprecated = true;
            methodData.deprecated_reason = sharedData.deprecated_reason;
        }

        for (size_t i = sharedData.params.size(); i-- != 0;) {
            std::shared_ptr<Parameter> param = sharedData.params[i];

            methodData.params.push_front(param);
        }

        for (size_t i = sharedData.examples.size(); i-- != 0;) {
            std::shared_ptr<Example> example = sharedData.examples[i];
            
            methodData.examples.push_front(example);
        }

        for (size_t i = sharedData.see_also.size(); i-- != 0;) {
            std::shared_ptr<SeeAlso> see = sharedData.see_also[i];
            
            methodData.see_also.push_front(see);
        }
    }
}