#ifndef ABICORN_METHOD_CHECKER_H
#define ABICORN_METHOD_CHECKER_H

#include <string>
#include <vector>

#include "Checker.h"
#include "LibData.hpp"

namespace abicorn {
class MethodChecker : public Checker {
  enum MatchType { Unknown = 0, NameOnly, UnqualifiedOnly, Perfect };

public:
  MethodChecker(AbicornContext &Context, const LibDataRef &Data,
                Options Mode = CheckAndCompare)
      : Checker("method-checker", Context, Data, Mode) {}

  void registerMatchers(MatchFinder &Finder) override;

  void check(const MatchFinder::MatchResult &Result) override;

private:
  void checkMethodChanges(const CXXMethodDecl *Old, const CXXMethodDecl *New);

  void reportMissingMethod(const CXXMethodDecl *Decl);

  void reportMissingOverload(const CXXMethodDecl *Decl);

  void reportMissingNote(const CXXMethodDecl *Decl);

  void reportChangeInQualifier(const CXXMethodDecl *New,
                               const CXXMethodDecl *Old,
                               const std::string &Qualifier,
                               bool NewIsQualified);

  void reportRefQualifierMissmatch(const CXXMethodDecl *Cur,
                                   const CXXMethodDecl *Cached);

  void reportAccessModifierMissmatch(const CXXMethodDecl *From,
                                     const CXXMethodDecl *To);
};
} // namespace abicorn

#endif // ABICORN_METHOD_CHECKER_H
