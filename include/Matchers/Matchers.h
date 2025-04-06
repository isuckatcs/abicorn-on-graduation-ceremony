#ifndef ABICORN_MATCHERS_H
#define ABICORN_MATCHERS_H

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace abicorn::matchers {
using namespace clang::ast_matchers;
AST_MATCHER(clang::VarDecl, isStaticDataMember) {
  return Node.isStaticDataMember();
}

struct GlobalStorageVariableMatcher {
  const inline static char *Id = "GlobalStorageVariableMatcher";

  const inline static auto Matcher =
      traverse(clang::TK_IgnoreUnlessSpelledInSource,
               varDecl(allOf(hasGlobalStorage(),
                             unless(anyOf(hasParent(recordDecl()),
                                          isStaticDataMember())))))
          .bind(Id);
};

struct GeneralFunctionMatcher {
  const inline static char *Id = "GeneralFunctionMatcher";

  const inline static auto Matcher =
      traverse(clang::TK_IgnoreUnlessSpelledInSource,
               functionDecl(unless(cxxMethodDecl())))
          .bind(Id);
};

struct GeneralRecordMatcher {
  const inline static char *Id = "GeneralRecordMatcher";

  const inline static auto Matcher =
      traverse(
          clang::TK_IgnoreUnlessSpelledInSource,
          cxxRecordDecl(allOf(hasDefinition(), hasExternalFormalLinkage())))
          .bind(Id);
};

} // namespace abicorn::matchers

#endif // ABICORN_MATCHERS_H
