#include "clang/AST/ASTStructuralEquivalence.h"

#include "Checkers/FunctionChecker.h"
#include "Matchers/Matchers.h"

namespace abicorn {
using namespace abicorn::matchers;

void FunctionChecker::reportMissingFunction(const std::string &Name,
                                            const FunctionDecl *Decl) {
  diag(Decl->getLocation(), "'%0' is not defined in the new library",
       &Decl->getASTContext())
      << Name;
}

void FunctionChecker::reportMissingOverload(const std::string &Name,
                                            const FunctionDecl *Decl) {
  diag(Decl->getLocation(), "overload of '%0' not found in the new library",
       &Decl->getASTContext())
      << Name;
}

void FunctionChecker::reportMissingNote(const FunctionDecl *Decl) {
  diag(Decl->getLocation(), "found similar function", &Decl->getASTContext(),
       clang::DiagnosticIDs::Note);
}

void FunctionChecker::reportChangeInQualifier(const FunctionDecl *Cur,
                                              const FunctionDecl *Cached,
                                              const std::string &Qualifier,
                                              bool CurIsQualified) {
  const std::string Name = Cur->getNameAsString();

  diag(Cached->getLocation(), "change in the '%0' qualifier of '%1'",
       &Cached->getASTContext())
      << Qualifier << Name;

  diag(Cur->getLocation(), "'%0' %1 '%2' in the old library",
       &Cur->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (CurIsQualified ? "is" : "is not") << Qualifier;

  diag(Cached->getLocation(), "'%0' %1 '%2' in the new library",
       &Cached->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (CurIsQualified ? "is not" : "is") << Qualifier;
}

void FunctionChecker::reportMissingTemplate(const FunctionDecl *Decl) {
  assert(Decl->getTemplatedKind() == clang::FunctionDecl::TK_FunctionTemplate &&
         "reporting missing template for a non-template function");

  diag(Decl->getLocation(),
       "function template declaration of '%0' not found in the new library",
       &Decl->getASTContext())
      << Decl->getNameAsString();
}

void FunctionChecker::reportMissingSpecialization(const FunctionDecl *Decl) {
  assert(Decl->getTemplatedKind() ==
             clang::FunctionDecl::TK_FunctionTemplateSpecialization &&
         "reporting missing specialization for a non-template function");

  diag(Decl->getLocation(),
       "function template specialization of '%0' not found in the new library",
       &Decl->getASTContext())
      << Decl->getNameAsString();
}

void FunctionChecker::registerMatchers(MatchFinder &Finder) {
  Finder.addMatcher(GeneralFunctionMatcher::Matcher, this);
}

void FunctionChecker::checkFunctionChanges(const FunctionDecl *Cur,
                                           const FunctionDecl *Cached) {
  if (Cur->isInlined() != Cached->isInlined())
    reportChangeInQualifier(Cur, Cached, "inline", Cur->isInlined());
}

void FunctionChecker::check(const MatchFinder::MatchResult &Result) {
  const auto *FD =
      Result.Nodes.getNodeAs<FunctionDecl>(GeneralFunctionMatcher::Id);

  if (!FD || !FD->isThisDeclarationADefinition() ||
      !FD->hasExternalFormalLinkage())
    return;

  std::string Name = FD->getNameAsString();

  const auto &ReachableInNew = Data->getReachableFunctions();

  if (ReachableInNew.count(Name) == 0) {
    reportMissingFunction(Name, FD);
    return;
  }

  const auto &OverloadsInNew = ReachableInNew.at(Name);
  assert(!OverloadsInNew.empty() && "function entry without any declaration");

  // Check the overloads.
  if (OverloadsInNew.count(FD) == 0) {
    switch (FD->getTemplatedKind()) {
    case clang::FunctionDecl::TK_FunctionTemplate:
      reportMissingTemplate(FD);
      break;
    case clang::FunctionDecl::TK_FunctionTemplateSpecialization:
      reportMissingSpecialization(FD);
      break;
    default:
      reportMissingOverload(Name, FD);
    }

    if (!OverloadsInNew.empty()) {
      auto Cmp = [](const Decl *LHS, const Decl *RHS) {
        return LHS->getLocation() < RHS->getLocation();
      };

      std::set<const FunctionDecl *, decltype(Cmp)> S{Cmp};

      for (auto &&CachedDecl : OverloadsInNew) {
        S.emplace(CachedDecl);
      }

      for (auto &&CachedDecl : S) {
        if (CachedDecl->getTemplatedKind() == FD->getTemplatedKind()) {
          reportMissingNote(CachedDecl);
        }
      }
    }

    return;
  }

  checkFunctionChanges(FD, *OverloadsInNew.find(FD));
}

} // namespace abicorn