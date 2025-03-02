#ifndef ABICORN_FUNCTION_BUILDER_H
#define ABICORN_FUNCTION_BUILDER_H

#include <utility>

#include "LibBuilder.h"

namespace abicorn {
class FunctionBuilder : public LibBuilder {
public:
  explicit FunctionBuilder(AbicornContext &Context, LibDataRef Data)
      : LibBuilder(Context, std::move(Data)){};

  void registerMatchers(MatchFinder &Finder) override;
  void handleMatch(const MatchFinder::MatchResult &Result) override;
};
}; // namespace abicorn

#endif // ABICORN_FUNCTION_BUILDER_H
