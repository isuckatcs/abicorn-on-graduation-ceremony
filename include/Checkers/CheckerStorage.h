#ifndef ABICORN_CHECKER_STORAGE_H
#define ABICORN_CHECKER_STORAGE_H

#include <memory>
#include <string>
#include <vector>

#include "AbicornContext.h"
#include "Checker.h"
#include "ClassChecker.h"
#include "FieldChecker.h"
#include "FunctionChecker.h"
#include "GlobalVariableChecker.h"
#include "InheritanceChecker.h"
#include "LibData.hpp"
#include "MethodChecker.h"
#include "VtableChecker.h"

namespace abicorn {
class CheckerStorage {
public:
  using CheckerContainerTy = std::vector<std::unique_ptr<Checker>>;

private:
  CheckerContainerTy Checkers;
  AbicornContext &Context;
  const LibDataRef &Data;
  Checker::Options Mode;

public:
  CheckerStorage(AbicornContext &Context, const LibDataRef &Data,
                 Checker::Options Mode = Checker::CheckAndCompare)
      : Context(Context), Data(Data), Mode(Mode) {

    registerChecker<FieldChecker>();
    registerChecker<InheritanceChecker>();
    registerChecker<FunctionChecker>();
    registerChecker<GlobalVariableChecker>();
    registerChecker<VtableChecker>();
    registerChecker<MethodChecker>();
    registerChecker<ClassChecker>();
  }

  void registerCheckers(MatchFinder &MF) {
    for (auto &&C : Checkers) {
      C->registerMatchers(MF);
    }
  }

  [[nodiscard]] const CheckerContainerTy &checkers() const { return Checkers; }

private:
  template <typename T>
  void registerChecker() {
    Checkers.emplace_back(std::make_unique<T>(Context, Data, Mode));
  }
};
} // namespace abicorn

#endif // ABICORN_CHECKER_STORAGE_H
