#ifndef ABICORN_CHECKER_H
#define ABICORN_CHECKER_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Core/Diagnostic.h"

#include "AbicornContext.h"
#include "LibData.hpp"

namespace abicorn {

using namespace clang::ast_matchers;

class Checker : public MatchFinder::MatchCallback {
public:
  enum Options {
    CheckMissingOnly = 1,
    CompareExistingOnly = 2,
    CheckAndCompare = 3
  };

private:
  std::string CheckerName;
  AbicornContext &Context;

protected:
  LibDataRef Data;
  bool CheckMissingEnabled;
  bool CompareExistingEnabled;

public:
  Checker(StringRef CheckerName, AbicornContext &Context, LibDataRef Data,
          Options Mode = CheckAndCompare)
      : CheckerName(CheckerName), Context(Context), Data(std::move(Data)) {
    assert(!CheckerName.empty() && "Empty checker name");
    assert(this->Data && "Data to compare against shouldn't be null");

    CheckMissingEnabled = Mode & CheckMissingOnly;
    CompareExistingEnabled = Mode & CompareExistingOnly;
  };

  [[nodiscard]] AbicornContext &getContext() const { return Context; }

  [[nodiscard]] std::string getName() const { return CheckerName; }

  DiagnosticBuilder diag(SourceLocation Loc, StringRef Message,
                         ASTContext *ASTCtx,
                         DiagnosticIDs::Level Level = DiagnosticIDs::Warning) {
    return Context.diag(CheckerName, Loc, Message, ASTCtx, Level);
  }

  virtual void registerMatchers(MatchFinder &Finder) = 0;

  virtual void check(const MatchFinder::MatchResult &Result) = 0;

  void run(const MatchFinder::MatchResult &Result) override { check(Result); };
};
} // namespace abicorn

#endif // ABICORN_CHECKER_H
