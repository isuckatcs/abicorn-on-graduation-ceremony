#include "clang/AST/ASTStructuralEquivalence.h"
#include "clang/AST/DeclCXX.h"

#include "Checkers/VtableChecker.h"
#include "Matchers/Matchers.h"

namespace abicorn {
using namespace abicorn::matchers;

namespace {
size_t getVirtualMethodIdx(const CXXMethodDecl *MD) {
  size_t Idx = 0;
  for (const auto *Method : MD->getParent()->methods()) {
    if (Method == MD)
      break;

    if (Method->isVirtual())
      ++Idx;
  }

  return Idx;
}
} // namespace

void VtableChecker::registerMatchers(MatchFinder &Finder) {
  Finder.addMatcher(GeneralRecordMatcher::Matcher, this);
}

void VtableChecker::reportMissingMethod(const CXXMethodDecl *Cur) {
  diag(Cur->getLocation(), "virtual method '%0' not found in the %1 library",
       &Cur->getASTContext())
      << Cur->getNameAsString() << (CheckingNewLib ? "new" : "old");
}

void VtableChecker::reportVirtualityChange(const CXXMethodDecl *Cached,
                                           const CXXMethodDecl *Cur) {
  std::string Name = Cur->getNameAsString();

  diag(Cached->getLocation(), "change in the virtuality of '%0'",
       &Cached->getASTContext())
      << Name;

  diag(Cur->getLocation(),
       "function '%0' is declared as '%1' in the %2 library",
       &Cur->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (Cur->isVirtual() ? "virtual" : "non-virtual")
      << (CheckingNewLib ? "old" : "new");

  diag(Cached->getLocation(),
       "function '%0' is declared as '%1' in the %2 library",
       &Cached->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (Cached->isVirtual() ? "virtual" : "non-virtual")
      << (CheckingNewLib ? "new" : "old");
}

void VtableChecker::reportIndexChange(const CXXMethodDecl *Cached,
                                      size_t CachedIdx,
                                      const CXXMethodDecl *Cur, size_t CurIdx) {
  std::string Name = Cur->getNameAsString();

  diag(Cached->getLocation(), "change in virtual function index",
       &Cached->getASTContext());

  diag(Cur->getLocation(),
       "virtual function '%0' is at index %2 in the %1 library",
       &Cur->getASTContext(), DiagnosticIDs::Note)
      << Name << (CheckingNewLib ? "old" : "new") << CurIdx;

  diag(Cached->getLocation(),
       "virtual function '%0' is at index %2 in the %1 library",
       &Cached->getASTContext(), DiagnosticIDs::Note)
      << Name << (CheckingNewLib ? "new" : "old") << CachedIdx;
}

void VtableChecker::reportFinalityChange(const CXXMethodDecl *Cached,
                                         const CXXMethodDecl *Cur) {
  std::string Name = Cur->getNameAsString();

  diag(Cached->getLocation(), "change in virtual function finality",
       &Cached->getASTContext());

  diag(Cur->getLocation(), "virtual function '%0' is %2 in the %1 library",
       &Cur->getASTContext(), DiagnosticIDs::Note)
      << Name << (CheckingNewLib ? "old" : "new")
      << (Cur->hasAttr<FinalAttr>() ? "final" : "non-final");

  diag(Cached->getLocation(), "virtual function '%0' is %2 in the %1 library",
       &Cached->getASTContext(), DiagnosticIDs::Note)
      << Name << (CheckingNewLib ? "new" : "old")
      << (Cached->hasAttr<FinalAttr>() ? "final" : "non-final");
}

void VtableChecker::checkAllVirtualMethods(const CXXRecordDecl *OldRecordDecl,
                                           const CXXRecordDecl *NewRecordDecl,
                                           bool CheckMissingOnly) {
  for (auto OldMethodIt = OldRecordDecl->method_begin();
       OldMethodIt != OldRecordDecl->method_end(); ++OldMethodIt) {
    const CXXMethodDecl *OldMethodDecl = *OldMethodIt;

    auto NewMethodIt = std::find_if(
        NewRecordDecl->method_begin(), NewRecordDecl->method_end(),
        [&OldMethodDecl](const CXXMethodDecl *MD) {
          return AbicornHash()(OldMethodDecl) == AbicornHash()(MD) &&
                 OldMethodDecl->isConst() == MD->isConst() &&
                 OldMethodDecl->getRefQualifier() == MD->getRefQualifier();
        });

    if (NewMethodIt == NewRecordDecl->method_end()) {
      if (OldMethodDecl->isVirtual())
        reportMissingMethod(OldMethodDecl);
      continue;
    }

    if (CheckMissingOnly)
      continue;

    if (OldMethodDecl->isVirtual() != NewMethodIt->isVirtual()) {
      reportVirtualityChange(*NewMethodIt, OldMethodDecl);
      continue;
    }

    if (OldMethodDecl->isVirtual() && NewMethodIt->isVirtual()) {
      size_t NewMethodIdx = getVirtualMethodIdx(OldMethodDecl);
      size_t OldMethodIdx = getVirtualMethodIdx(*NewMethodIt);

      if (NewMethodIdx != OldMethodIdx) {
        reportIndexChange(*NewMethodIt, OldMethodIdx, OldMethodDecl,
                          NewMethodIdx);
      }

      if (OldMethodDecl->hasAttr<FinalAttr>() !=
          NewMethodIt->hasAttr<FinalAttr>()) {
        reportFinalityChange(*NewMethodIt, OldMethodDecl);
      }
    }
  }
}

void VtableChecker::check(const MatchFinder::MatchResult &Result) {
  const auto *RD =
      Result.Nodes.getNodeAs<clang::CXXRecordDecl>(GeneralRecordMatcher::Id);

  if (!RD || !RD->hasDefinition() || !RD->isThisDeclarationADefinition())
    return;

  auto NewLibRecords = Data->getRecords();
  auto NewRecord = NewLibRecords.find(RD);
  if (NewRecord == NewLibRecords.end())
    return;

  CheckingNewLib = true;
  checkAllVirtualMethods(RD, *NewRecord, false);

  CheckingNewLib = false;
  checkAllVirtualMethods(*NewRecord, RD, true);
};
} // namespace abicorn