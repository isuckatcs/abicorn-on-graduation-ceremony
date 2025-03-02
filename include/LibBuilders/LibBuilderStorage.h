#ifndef ABICORN_BUILDER_STORAGE_H
#define ABICORN_BUILDER_STORAGE_H

#include <memory>
#include <string>
#include <vector>

#include "AbicornContext.h"
#include "FunctionBuilder.h"
#include "LibBuilder.h"
#include "LibData.hpp"
#include "RecordBuilder.h"
#include "VariableBuilder.h"

namespace abicorn {
class LibBuilderStorage {
public:
  using LibBuilderContainerTy = std::vector<std::unique_ptr<LibBuilder>>;

private:
  AbicornContext &Context;
  const LibDataRef &Data;
  LibBuilderContainerTy Builders;

public:
  LibBuilderStorage(AbicornContext &Context, const LibDataRef &Data)
      : Context(Context), Data(Data) {
    registerBuilder<FunctionBuilder>();
    registerBuilder<RecordBuilder>();
    registerBuilder<VariableBuilder>();
  }

  const LibBuilderContainerTy &builders() { return Builders; }

  void registerBuilders(MatchFinder &MF) {
    for (auto &B : Builders) {
      B->registerMatchers(MF);
    }
  }

private:
  template <typename T>
  void registerBuilder() {
    Builders.emplace_back(std::make_unique<T>(Context, Data));
  }
};
} // namespace abicorn

#endif // ABICORN_BUILDER_STORAGE_H
