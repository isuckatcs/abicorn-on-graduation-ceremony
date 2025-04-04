#ifndef ABICORN_HASH_H
#define ABICORN_HASH_H

#include "clang/AST/Decl.h"
#include "clang/AST/ODRHash.h"

namespace abicorn {
using namespace clang;

struct AbicornHash {
  std::size_t operator()(const CXXRecordDecl *RD) const;
  std::size_t operator()(const FunctionDecl *FD) const;
  std::size_t operator()(const VarDecl *VD) const;

private:
  struct Impl {
    void addRecordDecl(const CXXRecordDecl *RD);
    void addFunctionDecl(const FunctionDecl *FD);
    void addVarDecl(const VarDecl *VD);

    std::size_t calculateHash();

  private:
    void addDeclContext(const DeclContext *DC);

    void addTemplateDecl(const TemplateDecl *TD);
    void addTemplateSpecialization(const Decl *D);
    void addTemplateArgs(const TemplateArgumentList *AL);

    ODRHash Hash;
  };

  mutable std::map<const void *, std::size_t> Cache;
};

// A utility that creates a hash based on the return type, name and parameters
// only.
struct GeneralFunctionHasher {
  std::size_t operator()(const FunctionDecl *FD) const;
};

} // namespace abicorn

#endif // ABICORN_HASH_H
