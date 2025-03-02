#include <optional>

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTStructuralEquivalence.h"

#include "Checkers/FieldChecker.h"
#include "Matchers/Matchers.h"

namespace abicorn {
using namespace abicorn::matchers;

void FieldChecker::registerMatchers(MatchFinder &Finder) {
  Finder.addMatcher(GeneralRecordMatcher::Matcher, this);
}

void FieldChecker::reportMissingField(const DeclaratorDecl *FD) {
  diag(FD->getLocation(), "field '%0' is missing from the %1 library",
       &FD->getASTContext())
      << FD->getName() << (CheckingNewLib ? "new" : "old");
}

void FieldChecker::reportTypeMissmatch(const StringRef Name,
                                       const DeclaratorDecl *Cur,
                                       const DeclaratorDecl *Cached) {
  diag(Cached->getLocation(), "change in the type of '%0'",
       &Cached->getASTContext())
      << Name;

  diag(Cur->getLocation(), "the type of '%0' is '%2' in the %1 library",
       &Cur->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (CheckingNewLib ? "old" : "new")
      << Cur->getType().getAsString();

  diag(Cached->getLocation(), "the type of '%0' is '%2' in the %1 library",
       &Cached->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (CheckingNewLib ? "new" : "old")
      << Cached->getType().getAsString();
}

void FieldChecker::reportIndexMissmatch(const StringRef Name,
                                        const FieldDecl *Cur, size_t CurIdx,
                                        const FieldDecl *Cached,
                                        size_t CachedIdx) {
  diag(Cached->getLocation(), "change in the field index of '%0'",
       &Cached->getASTContext())
      << Name;

  diag(Cur->getLocation(), "field '%0' is at index %1 in the %2 library",
       &Cur->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << CurIdx << (CheckingNewLib ? "old" : "new");

  diag(Cached->getLocation(), "field '%0' is at index %1 in the %2 library",
       &Cached->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << CachedIdx << (CheckingNewLib ? "new" : "old");
}

void FieldChecker::reportStorageChange(const StringRef Name,
                                       const DeclaratorDecl *Cur,
                                       const DeclaratorDecl *Cached,
                                       bool TraversedIsStatic) {
  diag(Cached->getLocation(), "change in the storage of '%0'",
       &Cached->getASTContext())
      << Name;

  diag(Cur->getLocation(), "'%0' has %1 storage in the %2 library",
       &Cur->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (TraversedIsStatic ? "static" : "non-static")
      << (CheckingNewLib ? "old" : "new");

  diag(Cached->getLocation(), "'%0' has %1 storage in the %2 library",
       &Cached->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (TraversedIsStatic ? "non-static" : "static")
      << (CheckingNewLib ? "new" : "old");
}

void FieldChecker::reportAccessSpecifierMissmatch(
    const StringRef Name, const DeclaratorDecl *Cur, AccessSpecifier CurAccess,
    const DeclaratorDecl *Cached, AccessSpecifier CachedAccess) {
  diag(Cached->getLocation(), "change in the access modifier of '%0'",
       &Cached->getASTContext())
      << Name;

  diag(Cur->getLocation(), "'%0' is declared '%1' in the '%2' library",
       &Cur->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << getContext().getAccessStr(CurAccess)
      << (CheckingNewLib ? "old" : "new");

  diag(Cached->getLocation(), "'%0' is declared '%1' in the '%2' library",
       &Cached->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << getContext().getAccessStr(CachedAccess)
      << (CheckingNewLib ? "new" : "old");
}

std::vector<FieldChecker::DeclInfo>
FieldChecker::collectDecls(const RecordDecl *RD) const {
  std::vector<DeclInfo> Out;

  std::deque<DeclInfo> Q;
  for (const auto *Decl : RD->decls())
    Q.push_back({Decl, Decl->getAccess(), std::nullopt});

  while (!Q.empty()) {
    const auto DI = Q.front();
    Q.pop_front();

    Out.emplace_back(DI);

    if (const auto *FD = dyn_cast<FieldDecl>(DI.D);
        FD && FD->isAnonymousStructOrUnion()) {
      const RecordDecl *RD = FD->getType()->getAs<RecordType>()->getDecl();
      for (const auto *Decl : RD->decls()) {
        Decl->isInAnonymousNamespace();
        Q.push_back({Decl, DI.Access,
                     (RD->isUnion() ? std::optional<size_t>(FD->getFieldIndex())
                                    : std::nullopt)});
      }
    }
  }

  return Out;
}

void FieldChecker::checkTypeEquality(const StringRef Name,
                                     const DeclaratorDecl *Cur,
                                     const DeclaratorDecl *Cached) {
  ODRHash FromHash, ToHash;
  FromHash.AddQualType(Cur->getType());
  ToHash.AddQualType(Cached->getType());

  if (FromHash.CalculateHash() != ToHash.CalculateHash()) {
    reportTypeMissmatch(Name, Cur, Cached);
  }
}

void FieldChecker::checkIndexEquality(const StringRef Name,
                                      const FieldDecl *Cur,
                                      std::optional<size_t> CurIdx,
                                      const FieldDecl *Cached,
                                      std::optional<size_t> CachedIdx) {
  if (!Cur || !Cached)
    return;

  size_t ActualCurFieldIdx = CurIdx ? *CurIdx : Cur->getFieldIndex();
  size_t ActualCachedFieldIdx =
      CachedIdx ? *CachedIdx : Cached->getFieldIndex();

  if (ActualCurFieldIdx == ActualCachedFieldIdx)
    return;

  reportIndexMissmatch(Name, Cur, ActualCurFieldIdx, Cached,
                       ActualCachedFieldIdx);
}

void FieldChecker::checkAccessSpecifierEquality(const StringRef Name,
                                                const DeclaratorDecl *Cur,
                                                AccessSpecifier CurAccess,
                                                const DeclaratorDecl *Cached,
                                                AccessSpecifier CachedAccess) {
  if (CurAccess != CachedAccess) {
    reportAccessSpecifierMissmatch(Name, Cur, CurAccess, Cached, CachedAccess);
  }
}

void FieldChecker::checkStorageEquality(const DeclaratorDecl *Cur,
                                        const DeclaratorDecl *Cached) {
  const auto *CurVD = dyn_cast<VarDecl>(Cur);
  const auto *CachedVD = dyn_cast<VarDecl>(Cached);

  assert((!CurVD || CurVD->isStaticDataMember()) &&
         "Non-static datamember VarDecl encountered.");
  assert((!CachedVD || CachedVD->isStaticDataMember()) &&
         "Non-static datamember VarDecl encountered.");

  if (!CurVD && CachedVD || CurVD && !CachedVD) {
    reportStorageChange(Cur->getName(), Cur, Cached, CurVD);
  }
}

void FieldChecker::checkAllDecls(const std::vector<DeclInfo> &OldDecls,
                                 const std::vector<DeclInfo> &NewDecls,
                                 bool IsUnion, bool CheckMissingOnly) {
  for (const auto &[OldFieldDecl, CurDeclAccess, EncapsulatingUnionIndex] :
       OldDecls) {

    // If the field is static, it's going to be a VarDecl.
    if (!isa<VarDecl>(OldFieldDecl) && !isa<FieldDecl>(OldFieldDecl))
      continue;

    const auto *OldField = dyn_cast<DeclaratorDecl>(OldFieldDecl);
    if (!OldField || OldField->getName().empty())
      continue;

    auto NewFieldDeclIt = std::find_if(
        NewDecls.begin(), NewDecls.end(), [&OldField](DeclInfo DI) {
          if (const auto *DD = dyn_cast<DeclaratorDecl>(DI.D)) {
            return OldField->getNameAsString() == DD->getNameAsString();
          }

          return false;
        });

    if (NewFieldDeclIt == NewDecls.end()) {
      // [11.4.8.2.1] A static data member is not part of the subobjects of a
      // class.
      if (!isa<VarDecl>(OldField) || CheckingNewLib) {
        reportMissingField(OldField);
      }
      continue;
    }

    if (CheckMissingOnly)
      continue;

    const auto *NewField = dyn_cast<DeclaratorDecl>(NewFieldDeclIt->D);

    checkTypeEquality(OldField->getName(), OldField, NewField);
    checkAccessSpecifierEquality(OldField->getName(), OldField, CurDeclAccess,
                                 NewField, NewFieldDeclIt->Access);
    checkStorageEquality(OldField, NewField);

    if (IsUnion)
      continue;

    checkIndexEquality(OldField->getName(), dyn_cast<FieldDecl>(OldField),
                       EncapsulatingUnionIndex, dyn_cast<FieldDecl>(NewField),
                       NewFieldDeclIt->EncapsulatingUnionIndex);
  }
}

void FieldChecker::check(const MatchFinder::MatchResult &Result) {
  const auto *RD =
      Result.Nodes.getNodeAs<clang::CXXRecordDecl>(GeneralRecordMatcher::Id);

  if (!RD || !RD->isThisDeclarationADefinition())
    return;

  auto RecordsInNewLibrary = Data->getRecords();
  if (RecordsInNewLibrary.count(RD) == 0)
    return;

  const auto *ClassInNewLib = *RecordsInNewLibrary.find(RD);

  // The field iterator skips the static fields, so we iterate over all of the
  // declarations and filter out static and non-static fields.
  auto OldDecls = collectDecls(RD);
  auto NewDecls = collectDecls(ClassInNewLib);

  CheckingNewLib = true;
  checkAllDecls(OldDecls, NewDecls, RD->isUnion(), false);

  CheckingNewLib = false;
  checkAllDecls(NewDecls, OldDecls, RD->isUnion(), true);
}
} // namespace abicorn