#ifndef ABICORN_RECORD_BUILDER_H
#define ABICORN_RECORD_BUILDER_H

#include <utility>

#include "LibBuilder.h"

namespace abicorn {
class RecordBuilder : public LibBuilder {
public:
  explicit RecordBuilder(AbicornContext &Context, LibDataRef Data)
      : LibBuilder(Context, std::move(Data)){};

  void registerMatchers(MatchFinder &Finder) override;
  void handleMatch(const MatchFinder::MatchResult &Result) override;
};
}; // namespace abicorn

#endif // ABICORN_RECORD_BUILDER_H
