#ifndef ABICORN_LIB_DATA_H
#define ABICORN_LIB_DATA_H

#include <memory>
#include <unordered_set>

#include "clang/AST/Decl.h"

#include "Utils/Hash.h"

namespace abicorn {
using namespace llvm;
using namespace clang;

template <typename T>
struct HashEquals {
  bool operator()(const T *LHS, const T *RHS) const {
    AbicornHash H1, H2;

    return H1(LHS) == H2(RHS);
  }
};

class LibData : public std::enable_shared_from_this<LibData> {
public:
  using OverloadsContainerTy =
      std::unordered_set<const FunctionDecl *, AbicornHash,
                         HashEquals<FunctionDecl>>;

  // Map names to overloads.
  using FunctionContainerTy =
      std::unordered_map<std::string, OverloadsContainerTy>;

  using RecordContainerTy =
      std::unordered_set<const CXXRecordDecl *, AbicornHash,
                         HashEquals<CXXRecordDecl>>;

  using VariableContainerTy =
      std::unordered_set<const VarDecl *, AbicornHash, HashEquals<VarDecl>>;

private:
  RecordContainerTy Records;
  VariableContainerTy ReachableGlobalVariables;
  FunctionContainerTy ReachableFunctions;

  LibData() = default;
  LibData(const LibData &) = default;
  LibData &operator=(LibData &Other) = default;

public:
  [[nodiscard]] static std::shared_ptr<LibData> create() {
    return std::shared_ptr<LibData>(new LibData);
  }

  void addRecord(const CXXRecordDecl *RD) { Records.emplace(RD); }

  const RecordContainerTy &getRecords() const { return Records; }

  void addReachableFunction(const FunctionDecl *FD) {
    assert(FD && "nullptr passed as overload");
    assert(FD->hasExternalFormalLinkage() &&
           "internal function cannot be stored as reachable");
    ReachableFunctions[FD->getDeclName().getAsString()].emplace(FD);
  }

  const FunctionContainerTy &getReachableFunctions() const {
    return ReachableFunctions;
  }

  void addReachableGlobalVariable(const VarDecl *VD) {
    assert(VD && "nullptr passed as variable");
    assert(VD->hasExternalFormalLinkage() &&
           "internal variable cannot be stored as external");
    ReachableGlobalVariables.emplace(VD);
  }

  const VariableContainerTy &getReachableGlobalVariables() const {
    return ReachableGlobalVariables;
  }
};

using LibDataRef = std::shared_ptr<LibData>;
} // namespace abicorn

#endif // ABICORN_LIB_DATA_H
