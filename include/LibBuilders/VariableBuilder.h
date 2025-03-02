#ifndef ABICORN_VARIABLE_BUILDER_H
#define ABICORN_VARIABLE_BUILDER_H

#include <utility>

#include "LibBuilder.h"

namespace abicorn {
class VariableBuilder : public LibBuilder {
public:
  explicit VariableBuilder(AbicornContext &Context, LibDataRef Data)
      : LibBuilder(Context, std::move(Data)){};

  void registerMatchers(MatchFinder &Finder) override;
  void handleMatch(const MatchFinder::MatchResult &Result) override;
};
}; // namespace abicorn

#endif // ABICORN_VARIABLE_BUILDER_H
