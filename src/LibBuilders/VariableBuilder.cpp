#include "LibBuilders/VariableBuilder.h"
#include "Matchers/Matchers.h"

namespace abicorn {
using namespace clang;
using namespace abicorn::matchers;

void VariableBuilder::registerMatchers(MatchFinder &Finder) {
  Finder.addMatcher(GlobalStorageVariableMatcher::Matcher, this);
}

void VariableBuilder::handleMatch(const MatchFinder::MatchResult &Result) {
  if (const auto *VD =
          Result.Nodes.getNodeAs<VarDecl>(GlobalStorageVariableMatcher::Id)) {

    if (VD->getNameAsString().empty() || !VD->hasExternalFormalLinkage()) {
      return;
    }

    Data->addReachableGlobalVariable(VD);
  }
}

} // namespace abicorn
