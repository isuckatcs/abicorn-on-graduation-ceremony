#include "LibBuilders/FunctionBuilder.h"
#include "Matchers/Matchers.h"

namespace abicorn {
using namespace clang;
using namespace abicorn::matchers;

void FunctionBuilder::registerMatchers(MatchFinder &Finder) {
  Finder.addMatcher(GeneralFunctionMatcher::Matcher, this);
}

void FunctionBuilder::handleMatch(const MatchFinder::MatchResult &Result) {
  if (const auto *FD =
          Result.Nodes.getNodeAs<FunctionDecl>(GeneralFunctionMatcher::Id)) {

    // A function can have multiple declarations, but only one definition. A
    // declaration and a definition can also have different qualifiers.
    // E.g.:
    //    void foo();
    //    inline void foo() {};
    //
    // We only want to check changes in the definitions.
    if (!FD->isThisDeclarationADefinition()) {
      return;
    }

    if (FD->hasExternalFormalLinkage())
      Data->addReachableFunction(FD);
  }
}

} // namespace abicorn
