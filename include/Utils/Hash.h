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
    void setPP(const Decl *D);
    void addDeclContext(const DeclContext *DC);

    void addTemplateDecl(const TemplateDecl *TD);
    void addTemplateSpecialization(const Decl *D);
    void addTemplateArgs(const TemplateArgumentList *AL);
    void addQualType(QualType T);

    ODRHash Hash;
    // Note: this is needed, because ODRHash can generate the same hash for
    // different types, for some reason.
    llvm::FoldingSetNodeID HelperHash;
    std::unique_ptr<PrintingPolicy> PP;
  };

  mutable std::map<const void *, std::size_t> Cache;
};

} // namespace abicorn

#endif // ABICORN_HASH_H
