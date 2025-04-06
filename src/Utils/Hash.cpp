#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclTemplate.h"

#include "Utils/Hash.h"

namespace abicorn {
std::size_t AbicornHash::operator()(const CXXRecordDecl *RD) const {
  Impl I;
  I.addRecordDecl(RD);

  auto Hash = I.calculateHash();

  return Hash;
}

std::size_t AbicornHash::operator()(const FunctionDecl *FD) const {
  Impl I;
  I.addFunctionDecl(FD);

  auto Hash = I.calculateHash();

  return Hash;
}

std::size_t AbicornHash::operator()(const VarDecl *VD) const {
  Impl I;
  I.addVarDecl(VD);

  auto Hash = I.calculateHash();

  return Hash;
}

void AbicornHash::Impl::addRecordDecl(const CXXRecordDecl *RD) {
  Hash.AddDecl(RD);
  Hash.AddDeclarationName(RD->getDeclName());

  addTemplateDecl(RD->getDescribedTemplate());
  addTemplateSpecialization(RD);

  addDeclContext(RD->getDeclContext());
}

void AbicornHash::Impl::addFunctionDecl(const FunctionDecl *FD) {
  Hash.AddDecl(FD);
  Hash.AddDeclarationName(FD->getDeclName());

  Hash.AddQualType(FD->getReturnType().getCanonicalType());
  for (auto &P : FD->parameters()) {
    Hash.AddDecl(P);
    Hash.AddQualType(P->getType().getCanonicalType());
  }

  addTemplateDecl(FD->getDescribedFunctionTemplate());
  addTemplateSpecialization(FD);

  addDeclContext(FD->getDeclContext());
}

void AbicornHash::Impl::addVarDecl(const VarDecl *VD) {
  Hash.AddDecl(VD);
  Hash.AddDeclarationName(VD->getDeclName());

  addTemplateDecl(VD->getDescribedTemplate());
  addTemplateSpecialization(VD);

  addDeclContext(VD->getDeclContext());
}

std::size_t AbicornHash::Impl::calculateHash() { return Hash.CalculateHash(); }

void AbicornHash::Impl::addDeclContext(const DeclContext *DC) {
  while (DC) {
    if (const auto *ND = dyn_cast<NamespaceDecl>(DC)) {
      Hash.AddDecl(ND);
    } else if (const auto *RD = dyn_cast<CXXRecordDecl>(DC)) {
      Hash.AddDeclarationName(RD->getDeclName());
      addTemplateDecl(RD->getDescribedClassTemplate());
      addTemplateSpecialization(RD);
    }

    DC = DC->getParent();
  }
}

void AbicornHash::Impl::addTemplateDecl(const TemplateDecl *TD) {
  Hash.AddBoolean(TD);
  if (TD) {
    for (auto &&D : TD->getTemplateParameters()->asArray()) {
      size_t K = D->getKind();
      for (int I = 0; I < sizeof(size_t); ++I)
        Hash.AddBoolean((1 << I) & K);
    }
  }
}

void AbicornHash::Impl::addTemplateSpecialization(const Decl *D) {
  assert(D && "expected non-null decl");

  if (const auto *FD = dyn_cast<FunctionDecl>(D)) {
    if (FD->isFunctionTemplateSpecialization()) {
      addTemplateArgs(FD->getTemplateSpecializationArgs());
    }
  } else if (const auto *TS = dyn_cast<ClassTemplateSpecializationDecl>(D)) {
    addTemplateArgs(&TS->getTemplateArgs());
  } else if (const auto *TS = dyn_cast<VarTemplateSpecializationDecl>(D)) {
    addTemplateArgs(&TS->getTemplateArgs());
  }
}

void AbicornHash::Impl::addTemplateArgs(const TemplateArgumentList *AL) {
  for (const TemplateArgument &TA : AL->asArray()) {
    Hash.AddTemplateArgument(TA);
  }
}

std::size_t GeneralFunctionHasher::operator()(const FunctionDecl *FD) const {
  ODRHash Hash;

  Hash.AddQualType(FD->getReturnType().getCanonicalType());
  Hash.AddDeclarationName(FD->getDeclName());
  for (auto &P : FD->parameters()) {
    Hash.AddDecl(P);
    Hash.AddQualType(P->getType().getCanonicalType());
  }

  return Hash.CalculateHash();
}

}; // namespace abicorn