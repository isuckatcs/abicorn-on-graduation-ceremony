#ifndef ABICORN_CLASS_CHECKER_H
#define ABICORN_CLASS_CHECKER_H

#include "Checker.h"
#include "LibData.hpp"

namespace abicorn {
class ClassChecker : public Checker {
public:
  ClassChecker(AbicornContext &Context, const LibDataRef &Data,
               Options Mode = CheckAndCompare)
      : Checker("class-checker", Context, Data, Mode) {}

  void registerMatchers(MatchFinder &Finder) override;

  void check(const MatchFinder::MatchResult &Result) override;

private:
  void reportMissingClass(const CXXRecordDecl *RD);
  bool hasAnonymousFields(const CXXRecordDecl *RD);

  void checkFinalityMismatch(const CXXRecordDecl *Traversed,
                             const CXXRecordDecl *Cached);
  void checkSizeMismatch(const CXXRecordDecl *Traversed,
                         const CXXRecordDecl *Cached);
};
} // namespace abicorn

#endif // ABICORN_CLASS_CHECKER_H
