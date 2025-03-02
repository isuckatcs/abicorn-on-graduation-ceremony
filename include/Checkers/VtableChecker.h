#ifndef ABICORN_VTABLE_CHECKER_H
#define ABICORN_VTABLE_CHECKER_H

#include <string>
#include <vector>

#include "Checker.h"
#include "LibData.hpp"

namespace abicorn {
class VtableChecker : public Checker {
public:
  VtableChecker(AbicornContext &Context, const LibDataRef &Data,
                Options Mode = CheckAndCompare)
      : Checker("vtable-checker", Context, Data, Mode) {}

  void registerMatchers(MatchFinder &Finder) override;

  void check(const MatchFinder::MatchResult &Result) override;

private:
  bool CheckingNewLib = true;

  void checkAllVirtualMethods(const CXXRecordDecl *, const CXXRecordDecl *,
                              bool);

  void reportMissingMethod(const CXXMethodDecl *);

  void reportVirtualityChange(const CXXMethodDecl *, const CXXMethodDecl *);

  void reportFinalityChange(const CXXMethodDecl *, const CXXMethodDecl *);

  void reportIndexChange(const CXXMethodDecl *, size_t, const CXXMethodDecl *,
                         size_t);
};
} // namespace abicorn

#endif // ABICORN_VTABLE_CHECKER_H
