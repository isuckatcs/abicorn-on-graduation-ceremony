#ifndef ABICORN_GLOBAL_VARIABLE_CHECKER_H
#define ABICORN_GLOBAL_VARIABLE_CHECKER_H

#include "Checker.h"
#include "LibData.hpp"

namespace abicorn {
class GlobalVariableChecker : public Checker {
public:
  GlobalVariableChecker(AbicornContext &Context, const LibDataRef &Data,
                        Options Mode = CheckAndCompare)
      : Checker("global-variable-checker", Context, Data, Mode) {}

  void registerMatchers(MatchFinder &Finder) override;

  void check(const MatchFinder::MatchResult &Result) override;

private:
  void reportMissingVariable(const VarDecl *VD);

  void reportChangeInType(const VarDecl *Cur, const VarDecl *Cached);

  void
  reportMissingTemplateSpecialization(const VarTemplateSpecializationDecl *VTD);
};
} // namespace abicorn

#endif // ABICORN_GLOBAL_VARIABLE_CHECKER_H
