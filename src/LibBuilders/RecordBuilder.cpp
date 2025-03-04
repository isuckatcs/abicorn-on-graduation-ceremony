#include "LibBuilders/RecordBuilder.h"
#include "Matchers/Matchers.h"

namespace abicorn {
using namespace clang;
using namespace abicorn::matchers;

void RecordBuilder::registerMatchers(MatchFinder &Finder) {
  Finder.addMatcher(GeneralRecordMatcher::Matcher, this);
}

void RecordBuilder::handleMatch(const MatchFinder::MatchResult &Result) {
  if (const auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
          GeneralRecordMatcher::Id)) {
    if (!RD->getName().empty())
      Data->addRecord(RD);
  }
}

} // namespace abicorn
