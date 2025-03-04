#include "Checkers/MethodChecker.h"
#include "Matchers/Matchers.h"
#include "Utils/Hash.h"

namespace abicorn {
using namespace abicorn::matchers;

void MethodChecker::registerMatchers(MatchFinder &Finder) {
  Finder.addMatcher(GeneralRecordMatcher::Matcher, this);
}

void MethodChecker::reportRefQualifierMissmatch(const CXXMethodDecl *Cur,
                                                const CXXMethodDecl *Cached) {
  const std::string Name = Cur->getNameAsString();

  auto PrepareRQStr = [](clang::RefQualifierKind RQ) -> std::string {
    switch (RQ) {
    case clang::RQ_LValue:
      return "'&'";
    case clang::RQ_RValue:
      return "'&&'";
    default:
      return "no";
    }
  };

  diag(Cached->getLocation(), "change in the ref-qualifier of method '%0'",
       &Cached->getASTContext())
      << Name;

  std::string Format =
      "method '%0' is marked with %1 ref-qualifier in the %2 library";

  diag(Cur->getLocation(), Format, &Cur->getASTContext(), DiagnosticIDs::Note)
      << Name << PrepareRQStr(Cur->getRefQualifier()) << "old";

  diag(Cached->getLocation(), Format, &Cached->getASTContext(),
       DiagnosticIDs::Note)
      << Name << PrepareRQStr(Cached->getRefQualifier()) << "new";
}

void MethodChecker::reportAccessModifierMissmatch(const CXXMethodDecl *Cur,
                                                  const CXXMethodDecl *Cached) {
  const std::string Name = Cur->getNameAsString();

  diag(Cached->getLocation(), "change in the access modifier of method '%0'",
       &Cached->getASTContext())
      << Name;

  diag(Cur->getLocation(), "method '%0' is declared '%1' in the %2 library",
       &Cur->getASTContext(), DiagnosticIDs::Note)
      << Name << getContext().getAccessStr(Cur->getAccess()) << "old";

  diag(Cached->getLocation(), "method '%0' is declared '%1' in the %2 library",
       &Cached->getASTContext(), DiagnosticIDs::Note)
      << Name << getContext().getAccessStr(Cached->getAccess()) << "new";
}

void MethodChecker::reportMissingMethod(const CXXMethodDecl *Decl) {
  diag(Decl->getLocation(), "method '%0' not found in the new library",
       &Decl->getASTContext())
      << Decl->getNameAsString();
}

void MethodChecker::reportMissingOverload(const CXXMethodDecl *Decl) {
  diag(Decl->getLocation(), "overload of '%0' not found in the new library",
       &Decl->getASTContext())
      << Decl->getNameAsString();
}

void MethodChecker::reportMissingNote(const CXXMethodDecl *Decl) {
  diag(Decl->getLocation(), "found similar method", &Decl->getASTContext(),
       clang::DiagnosticIDs::Note);
}

void MethodChecker::reportChangeInQualifier(const CXXMethodDecl *Cur,
                                            const CXXMethodDecl *Cached,
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

void MethodChecker::checkMethodChanges(const CXXMethodDecl *Cur,
                                       const CXXMethodDecl *Cached) {
  if (Cur->isConst() != Cached->isConst())
    reportChangeInQualifier(Cur, Cached, "const", Cur->isConst());

  if (Cur->isVolatile() != Cached->isVolatile())
    reportChangeInQualifier(Cur, Cached, "volatile", Cur->isVolatile());

  if (Cur->isInlined() != Cached->isInlined())
    reportChangeInQualifier(Cur, Cached, "inline", Cur->isInlined());

  if (Cur->isStatic() != Cached->isStatic())
    reportChangeInQualifier(Cur, Cached, "static", Cur->isStatic());

  if (Cur->getRefQualifier() != Cached->getRefQualifier())
    reportRefQualifierMissmatch(Cur, Cached);

  if (Cur->getAccess() != Cached->getAccess())
    reportAccessModifierMissmatch(Cur, Cached);
}

void MethodChecker::check(const MatchFinder::MatchResult &Result) {
  const auto *OldRecord =
      Result.Nodes.getNodeAs<clang::CXXRecordDecl>(GeneralRecordMatcher::Id);

  if (!OldRecord || !OldRecord->hasDefinition() ||
      !OldRecord->isThisDeclarationADefinition() ||
      OldRecord->getDeclName().isEmpty())
    return;

  auto NewRecords = Data->getRecords();

  auto NewRecordIt = NewRecords.find(OldRecord);
  if (NewRecordIt == NewRecords.end())
    return;

  const auto *NewRecord = *NewRecordIt;

  const auto &OldMethods = OldRecord->methods();
  const auto &NewMethods = NewRecord->methods();

  AbicornHash Hash;
  std::set<const CXXMethodDecl *> PerfectMatches;

  for (const auto *OldMethod : OldMethods) {
    // OldMethod->dump();

    MatchType BestMatchType = Unknown;
    const clang::CXXMethodDecl *BestMatch = nullptr;
    std::vector<const clang::CXXMethodDecl *> Overloads;

    for (const auto *NewMethod : NewMethods) {
      // NewMethod->dump();

      std::string OldDeclName = OldMethod->getDeclName().getAsString();
      std::string NewDeclName = NewMethod->getDeclName().getAsString();

      if (OldDeclName != NewDeclName)
        continue;

      Overloads.emplace_back(NewMethod);

      // This method is already found in the old library, so we ignore it.
      if (PerfectMatches.count(NewMethod))
        continue;

      // Only the names match so far.
      MatchType MatchType = NameOnly;

      auto OldHash = Hash(OldMethod);
      auto NewHash = Hash(NewMethod);

      if (OldHash == NewHash) {
        // The return type, parameters, template specializations all match too.
        MatchType = UnqualifiedOnly;

        if (OldMethod->getMethodQualifiers() ==
                NewMethod->getMethodQualifiers() &&
            OldMethod->getRefQualifier() == NewMethod->getRefQualifier()) {
          // Everything matches, we found the exact same method.
          MatchType = Perfect;
          PerfectMatches.emplace(NewMethod);
        }
      }

      if (MatchType < BestMatchType)
        continue;

      BestMatchType = MatchType;
      BestMatch = NewMethod;
    }

    if ((BestMatchType == Perfect ||
         (BestMatchType == UnqualifiedOnly && Overloads.size() == 1))) {
      checkMethodChanges(OldMethod, BestMatch);
      continue;
    }

    if (OldMethod->isVirtual() || OldMethod->isTrivial() ||
        OldMethod->isImplicit() || isa<CXXDestructorDecl>(OldMethod))
      continue;

    if (const auto *Ctor = dyn_cast<CXXConstructorDecl>(OldMethod)) {
      // FIXME: reason better about constructors
      if (Ctor->isDefaultConstructor() || Ctor->isCopyOrMoveConstructor() ||
          Ctor->getNumParams() == 0)
        continue;
    }

    if (Overloads.empty()) {
      reportMissingMethod(OldMethod);
      continue;
    }

    reportMissingOverload(OldMethod);
    for (const auto &O : Overloads) {
      reportMissingNote(O);
    }
  }

  const CXXDestructorDecl *OldDtor = OldRecord->getDestructor();
  const CXXDestructorDecl *NewDtor = NewRecord->getDestructor();

  if (!OldDtor && NewDtor && !NewDtor->isVirtual()) {
    if (NewDtor->isDeleted()) {
      diag(NewDtor->getLocation(),
           "the implicit destructor of '%0' is deleted in the new library",
           &NewDtor->getASTContext())
          << NewRecord->getName();
    }

    if (NewDtor->getAccess() != clang::AS_public) {
      diag(NewDtor->getLocation(),
           "the implicit destructor of '%0' is replaced with a non-public "
           "destructor in the new library",
           &NewDtor->getASTContext())
          << NewRecord->getName();
    }
  }
}

} // namespace abicorn