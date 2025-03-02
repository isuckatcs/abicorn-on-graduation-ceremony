#ifndef ABICORN_FUNCTION_CHECKER_H
#define ABICORN_FUNCTION_CHECKER_H

#include <string>

#include "Checker.h"
#include "LibData.hpp"

namespace abicorn {
class FunctionChecker : public Checker {
public:
  FunctionChecker(AbicornContext &Context, const LibDataRef &Data,
                  Options Mode = CheckAndCompare)
      : Checker("function-checker", Context, Data, Mode) {}

  void registerMatchers(MatchFinder &Finder) override;

  void check(const MatchFinder::MatchResult &Result) override;

private:
  void checkFunctionChanges(const FunctionDecl *Cur,
                            const FunctionDecl *Cached);

  void checkMissingFunctions(const LibDataRef &From, const LibDataRef &In,
                             const std::string &FromName);

  void reportMissingFunction(const std::string &Name, const FunctionDecl *Decl);

  void reportMissingOverload(const std::string &Name, const FunctionDecl *Decl);

  void reportMissingTemplate(const FunctionDecl *Decl);

  void reportMissingSpecialization(const FunctionDecl *Decl);

  void reportMissingNote(const FunctionDecl *Decl);

  void reportChangeInQualifier(const FunctionDecl *Cur,
                               const FunctionDecl *Cached,
                               const std::string &Qualifier,
                               bool CurIsQualified);
};
} // namespace abicorn

#endif // ABICORN_FUNCTION_CHECKER_H
