#include "clang/AST/ASTStructuralEquivalence.h"

#include "Checkers/GlobalVariableChecker.h"
#include "Matchers/Matchers.h"

namespace abicorn {

using namespace abicorn::matchers;

void GlobalVariableChecker::reportMissingVariable(const VarDecl *VD) {
  diag(VD->getLocation(), "'%0' is not declared in the new library",
       &VD->getASTContext())
      << VD->getNameAsString();
}

void GlobalVariableChecker::reportChangeInType(const VarDecl *Cur,
                                               const VarDecl *Cached) {
  const auto Name = Cur->getDeclName().getAsString();

  diag(Cached->getLocation(), "change in the type of '%0'",
       &Cached->getASTContext())
      << Name;

  diag(Cur->getLocation(), "the type of '%0' is '%1' in the old library",
       &Cur->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << Cur->getType().getAsString();

  diag(Cached->getLocation(), "the type of '%0' is '%1' in the new library",
       &Cached->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << Cached->getType().getAsString();
}

void GlobalVariableChecker::reportMissingTemplateSpecialization(
    const VarTemplateSpecializationDecl *VTD) {

  diag(VTD->getLocation(),
       "specialization of variable '%0' is not declared in the new library",
       &VTD->getASTContext())
      << VTD->getNameAsString();
}

void GlobalVariableChecker::registerMatchers(MatchFinder &Finder) {
  Finder.addMatcher(GlobalStorageVariableMatcher::Matcher, this);
}

void GlobalVariableChecker::check(const MatchFinder::MatchResult &Result) {
  const auto *VD =
      Result.Nodes.getNodeAs<clang::VarDecl>(GlobalStorageVariableMatcher::Id);

  if (!VD || VD->getNameAsString().empty() || !VD->hasExternalFormalLinkage())
    return;

  const auto &ReachableGlobalsInNew = Data->getReachableGlobalVariables();

  if (ReachableGlobalsInNew.count(VD) == 0) {
    if (const auto *VTD = dyn_cast<VarTemplateSpecializationDecl>(VD)) {
      reportMissingTemplateSpecialization(VTD);
    } else {
      reportMissingVariable(VD);
    }

    return;
  }

  const VarDecl *CachedVD = *ReachableGlobalsInNew.find(VD);

  ODRHash HashCur, HashCached;
  HashCur.AddQualType(VD->getType());
  HashCached.AddQualType(CachedVD->getType());

  if (HashCur.CalculateHash() != HashCached.CalculateHash())
    reportChangeInType(VD, CachedVD);
};
} // namespace abicorn