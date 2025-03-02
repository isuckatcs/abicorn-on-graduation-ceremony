#ifndef ABICORN_INHERITANCE_CHECKER_H
#define ABICORN_INHERITANCE_CHECKER_H

#include <string>
#include <vector>

#include "Checker.h"
#include "LibData.hpp"
#include "clang/Rewrite/Core/Rewriter.h"

namespace abicorn {
class InheritanceChecker : public Checker {
  std::set<const Decl *> ChangedBases;
  mutable Rewriter RW;

public:
  InheritanceChecker(AbicornContext &Context, const LibDataRef &Data,
                     Options Mode = CheckAndCompare)
      : Checker("inheritance-checker", Context, Data, Mode) {}

  void registerMatchers(MatchFinder &Finder) override;

  void check(const MatchFinder::MatchResult &Result) override;

private:
  void reportChangeInBaseClassOrder(const clang::CXXBaseSpecifier &Cur,
                                    ASTContext &CurASTCtx,
                                    const clang::CXXBaseSpecifier &Cached,
                                    ASTContext &CachedASTCtx, int CurIdx,
                                    int BaseIdx);

  void reportChangeInBaseClassVirtuality(const clang::CXXBaseSpecifier &Cur,
                                         ASTContext &CurASTCtx,
                                         const clang::CXXBaseSpecifier &Cached,
                                         ASTContext &CachedASTCtx,
                                         bool CurIsVirtual);

  void reportAddedBaseClass(const CXXRecordDecl *, const CXXRecordDecl *,
                            const clang::CXXBaseSpecifier &);

  void reportRemovedBaseClass(const CXXRecordDecl *, const CXXRecordDecl *,
                              const clang::CXXBaseSpecifier &);

  void reportChangeInParent(const clang::CXXBaseSpecifier &Cur,
                            ASTContext &CurASTCtx);

  void reportChangeInBaseAccessSpecifier(const clang::CXXBaseSpecifier &Cur,
                                         ASTContext &CurASTCtx,
                                         const clang::CXXBaseSpecifier &Cached,
                                         ASTContext &CachedASTCtx);

  void registerAsChanged(const CXXRecordDecl *RD) {
    assert(RD && "cannot cache nullptr");

    ChangedBases.emplace(RD);

    if (const auto *TD = dyn_cast_or_null<ClassTemplateDecl>(
            RD->getDescribedClassTemplate())) {
      for (const clang::ClassTemplateSpecializationDecl *Spec :
           TD->specializations()) {
        ChangedBases.emplace(Spec);
      }
    }
  }

  bool isRegisteredAsChanged(const Decl *RD) {
    if (!RD) {
      return false;
    }

    if (ChangedBases.count(RD)) {
      return true;
    }

    if (const auto *TI = dyn_cast<ClassTemplateSpecializationDecl>(RD)) {
      llvm::PointerUnion<ClassTemplateDecl *,
                         ClassTemplatePartialSpecializationDecl *>
          InstFrom = TI->getInstantiatedFrom();

      if (InstFrom.is<ClassTemplateDecl *>()) {
        return ChangedBases.count(InstFrom.get<ClassTemplateDecl *>());
      }

      return ChangedBases.count(
          InstFrom.get<ClassTemplatePartialSpecializationDecl *>());
    }

    return false;
  }

  std::string getBaseClassStr(const clang::CXXBaseSpecifier &Base,
                              ASTContext &ASTCtx) const {
    RW.setSourceMgr(ASTCtx.getSourceManager(), ASTCtx.getLangOpts());

    SourceRange SR = SourceRange(Base.getBaseTypeLoc(), Base.getEndLoc());

    if (SR.getBegin().isMacroID()) {
      SR = ASTCtx.getSourceManager()
               .getImmediateExpansionRange(SR.getBegin())
               .getAsRange();
    }

    return RW.getRewrittenText(SR);
  }
};
} // namespace abicorn

#endif // ABICORN_INHERITANCE_CHECKER_H
