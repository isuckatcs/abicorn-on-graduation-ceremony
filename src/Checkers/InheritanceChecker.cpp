#include <sstream>

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTStructuralEquivalence.h"
#include "clang/AST/DeclCXX.h"

#include "Checkers/InheritanceChecker.h"
#include "Matchers/Matchers.h"

namespace abicorn {
using namespace abicorn::matchers;

namespace {

struct BaseClassInfo {
  const CXXBaseSpecifier Base;
  int Index = -1;
  bool Found = false;
};

BaseClassInfo getBaseClassInfo(const CXXRecordDecl *Class,
                               const CXXBaseSpecifier &Base) {
  ODRHash BaseHash;
  BaseHash.AddQualType(Base.getType());
  size_t BaseHashVal = BaseHash.CalculateHash();

  int Idx = 0;
  for (auto &&B : Class->bases()) {
    ODRHash CachedHash;
    CachedHash.AddQualType(B.getType());

    if (CachedHash.CalculateHash() == BaseHashVal) {
      return {B, Idx, true};
    }

    ++Idx;
  }

  return {};
}
} // namespace

void InheritanceChecker::reportChangeInBaseClassOrder(
    const clang::CXXBaseSpecifier &Cur, ASTContext &CurASTCtx,
    const clang::CXXBaseSpecifier &Cached, ASTContext &CachedASTCtx, int CurIdx,
    int BaseIdx) {
  std::string Name = getBaseClassStr(Cur, CurASTCtx);

  diag(Cached.getBaseTypeLoc(), "change in base class order", &CachedASTCtx);

  diag(Cur.getBaseTypeLoc(), "'%0' is at index %1 in the %2 library ",
       &CurASTCtx, DiagnosticIDs::Note)
      << Name << CurIdx << getContext().getTraversedLibraryStr();

  diag(Cached.getBaseTypeLoc(), "'%0' is at index %1 in the %2 library ",
       &CachedASTCtx, DiagnosticIDs::Note)
      << Name << BaseIdx << getContext().getCachedLibraryStr();
}

void InheritanceChecker::reportChangeInBaseClassVirtuality(
    const clang::CXXBaseSpecifier &Cur, ASTContext &CurASTCtx,
    const clang::CXXBaseSpecifier &Cached, ASTContext &CachedASTCtx,
    bool CurIsVirtual) {

  std::string Name = getBaseClassStr(Cur, CurASTCtx);

  diag(Cached.getBaseTypeLoc(), "change in base class virtuality",
       &CachedASTCtx);

  diag(Cur.getBaseTypeLoc(), "'%0' is inherited %1virtually in the %2 library",
       &CurASTCtx, DiagnosticIDs::Note)
      << Name << (CurIsVirtual ? "" : "non-")
      << getContext().getTraversedLibraryStr();

  diag(Cached.getBaseTypeLoc(),
       "'%0' is inherited %1virtually in the %2 library", &CachedASTCtx,
       DiagnosticIDs::Note)
      << Name << (CurIsVirtual ? "non-" : "")
      << getContext().getCachedLibraryStr();
}

void InheritanceChecker::reportAddedBaseClass(
    const CXXRecordDecl *Cur, const CXXRecordDecl *Cached,
    const clang::CXXBaseSpecifier &Base) {

  std::string BaseName = getBaseClassStr(Base, Cached->getASTContext());
  StringRef ClassName = Cur->getName();

  diag(Cached->getLocation(), "change in base class hierarchy",
       &Cached->getASTContext());

  diag(Cur->getLocation(), "'%0' is not derived from '%1' in the %2 library",
       &Cur->getASTContext(), DiagnosticIDs::Note)
      << ClassName << BaseName << getContext().getTraversedLibraryStr();

  diag(Base.getBaseTypeLoc(), "'%0' is derived from '%1' in the %2 library",
       &Cached->getASTContext(), DiagnosticIDs::Note)
      << ClassName << BaseName << getContext().getCachedLibraryStr();
}

void InheritanceChecker::reportRemovedBaseClass(
    const CXXRecordDecl *Cur, const CXXRecordDecl *Cached,
    const clang::CXXBaseSpecifier &Base) {

  std::string BaseName = getBaseClassStr(Base, Cur->getASTContext());
  StringRef ClassName = Cur->getName();

  diag(Cached->getLocation(), "change in base class hierarchy",
       &Cached->getASTContext());

  diag(Base.getBaseTypeLoc(), "'%0' is derived from '%1' in the %2 library",
       &Cur->getASTContext(), DiagnosticIDs::Note)
      << ClassName << BaseName << getContext().getTraversedLibraryStr();

  diag(Cached->getLocation(), "'%0' is not derived from '%1' in the %2 library",
       &Cached->getASTContext(), DiagnosticIDs::Note)
      << ClassName << BaseName << getContext().getCachedLibraryStr();
}

void InheritanceChecker::reportChangeInParent(
    const clang::CXXBaseSpecifier &Cur, ASTContext &CurASTCtx) {

  std::string Name = getBaseClassStr(Cur, CurASTCtx);

  diag(Cur.getBaseTypeLoc(),
       "inheriting from class with modified base class hierarchy", &CurASTCtx);

  diag(Cur.getBaseTypeLoc(),
       "the base class hierarchy of '%0' has changed in the %1 library",
       &CurASTCtx, DiagnosticIDs::Note)
      << Name << getContext().getCachedLibraryStr();
}

void InheritanceChecker::reportChangeInBaseAccessSpecifier(
    const clang::CXXBaseSpecifier &Cur, ASTContext &CurASTCtx,
    const clang::CXXBaseSpecifier &Cached, ASTContext &CachedASTCtx) {

  std::string Name = getBaseClassStr(Cur, CurASTCtx);

  diag(Cached.getBaseTypeLoc(), "change in inheritance access specifier",
       &CachedASTCtx);

  const char *FormatStr = "'%0' is inherited as '%1' in the '%2' library";
  diag(Cur.getBaseTypeLoc(), FormatStr, &CurASTCtx, clang::DiagnosticIDs::Note)
      << Name << getContext().getAccessStr(Cur.getAccessSpecifier())
      << getContext().getTraversedLibraryStr();

  diag(Cached.getBaseTypeLoc(), FormatStr, &CachedASTCtx,
       clang::DiagnosticIDs::Note)
      << Name << getContext().getAccessStr(Cached.getAccessSpecifier())
      << getContext().getCachedLibraryStr();
}

void InheritanceChecker::registerMatchers(MatchFinder &Finder) {
  Finder.addMatcher(GeneralRecordMatcher::Matcher, this);
}

void InheritanceChecker::check(const MatchFinder::MatchResult &Result) {
  const auto *RD =
      Result.Nodes.getNodeAs<clang::CXXRecordDecl>(GeneralRecordMatcher::Id);

  if (!RD || !RD->hasDefinition() || !RD->isThisDeclarationADefinition() ||
      !CompareExistingEnabled)
    return;

  const auto &CachedRecords = Data->getRecords();

  auto CachedIt = CachedRecords.find(RD);
  if (CachedIt == CachedRecords.end())
    return;

  const auto *CachedDecl = *CachedIt;

  int Idx = 0;
  for (auto &&CurBase : RD->bases()) {
    const Type *BaseType = CurBase.getType()->getUnqualifiedDesugaredType();

    const auto &BaseInfo = getBaseClassInfo(CachedDecl, CurBase);
    if (!BaseInfo.Found) {
      registerAsChanged(CachedDecl);

      reportRemovedBaseClass(RD, CachedDecl, CurBase);
      continue;
    }

    if (BaseInfo.Index != Idx) {
      registerAsChanged(CachedDecl);

      reportChangeInBaseClassOrder(CurBase, RD->getASTContext(), BaseInfo.Base,
                                   CachedDecl->getASTContext(), Idx,
                                   BaseInfo.Index);
    }

    if (BaseInfo.Base.isVirtual() != CurBase.isVirtual()) {
      registerAsChanged(CachedDecl);

      reportChangeInBaseClassVirtuality(
          CurBase, RD->getASTContext(), BaseInfo.Base,
          CachedDecl->getASTContext(), CurBase.isVirtual());
    }

    if (BaseInfo.Base.getAccessSpecifier() != CurBase.getAccessSpecifier()) {
      registerAsChanged(CachedDecl);

      reportChangeInBaseAccessSpecifier(CurBase, RD->getASTContext(),
                                        BaseInfo.Base,
                                        CachedDecl->getASTContext());
    }

    ++Idx;
  }

  for (auto &&CurBase : CachedDecl->bases()) {
    const Type *BaseType = CurBase.getType()->getUnqualifiedDesugaredType();

    if (isRegisteredAsChanged(CurBase.getType()->getAsTagDecl())) {
      registerAsChanged(CachedDecl);

      reportChangeInParent(CurBase, CachedDecl->getASTContext());
    }

    const auto &BaseInfo = getBaseClassInfo(RD, CurBase);
    if (!BaseInfo.Found) {
      registerAsChanged(CachedDecl);

      reportAddedBaseClass(RD, CachedDecl, CurBase);
    }
  }
}
} // namespace abicorn