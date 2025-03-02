#ifndef ABICORN_LIB_BUILDER_H
#define ABICORN_LIB_BUILDER_H

#include <cassert>
#include <memory>
#include <utility>

#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "AbicornContext.h"
#include "LibData.hpp"

namespace abicorn {

using namespace clang::ast_matchers;

class LibBuilder : public MatchFinder::MatchCallback {
protected:
  AbicornContext &Context;
  LibDataRef Data;

public:
  LibBuilder(AbicornContext &Context, LibDataRef Data)
      : Context(Context), Data(std::move(Data)){};

  virtual void registerMatchers(MatchFinder &Finder) = 0;

  virtual void handleMatch(const MatchFinder::MatchResult &Result) = 0;

  void run(const MatchFinder::MatchResult &Result) override {
    handleMatch(Result);
  };
};
} // namespace abicorn

#endif // ABICORN_LIB_BUILDER_H
