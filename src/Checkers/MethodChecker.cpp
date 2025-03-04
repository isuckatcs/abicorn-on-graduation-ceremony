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
      << Name << PrepareRQStr(Cur->getRefQualifier())
      << getContext().getTraversedLibraryStr();

  diag(Cached->getLocation(), Format, &Cached->getASTContext(),
       DiagnosticIDs::Note)
      << Name << PrepareRQStr(Cached->getRefQualifier())
      << getContext().getCachedLibraryStr();
}

void MethodChecker::reportAccessModifierMissmatch(const CXXMethodDecl *Cur,
                                                  const CXXMethodDecl *Cached) {
  const std::string Name = Cur->getNameAsString();

  diag(Cached->getLocation(), "change in the access modifier of method '%0'",
       &Cached->getASTContext())
      << Name;

  diag(Cur->getLocation(), "method '%0' is declared '%1' in the %2 library",
       &Cur->getASTContext(), DiagnosticIDs::Note)
      << Name << getContext().getAccessStr(Cur->getAccess())
      << getContext().getTraversedLibraryStr();

  diag(Cached->getLocation(), "method '%0' is declared '%1' in the %2 library",
       &Cached->getASTContext(), DiagnosticIDs::Note)
      << Name << getContext().getAccessStr(Cached->getAccess())
      << getContext().getCachedLibraryStr();
}

void MethodChecker::reportMissingMethod(const CXXMethodDecl *Decl) {
  diag(Decl->getLocation(), "method '%0' not found in the %1 library",
       &Decl->getASTContext())
      << Decl->getNameAsString() << getContext().getCachedLibraryStr();
}

void MethodChecker::reportMissingOverload(const CXXMethodDecl *Decl) {
  diag(Decl->getLocation(), "overload of '%0' not found in the %1 library",
       &Decl->getASTContext())
      << Decl->getNameAsString() << getContext().getCachedLibraryStr();
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

  diag(Cur->getLocation(), "'%0' %1 '%2' in the %3 library",
       &Cur->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (CurIsQualified ? "is" : "is not") << Qualifier
      << getContext().getTraversedLibraryStr();

  diag(Cached->getLocation(), "'%0' %1 '%2' in the %3 library",
       &Cached->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (CurIsQualified ? "is not" : "is") << Qualifier
      << getContext().getCachedLibraryStr();
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
  const auto *RD =
      Result.Nodes.getNodeAs<clang::CXXRecordDecl>(GeneralRecordMatcher::Id);

  if (!RD || !RD->hasDefinition() || !RD->isThisDeclarationADefinition() ||
      RD->getDeclName().isEmpty())
    return;

  auto OtherClasses = Data->getRecords();

  auto OtherIt = OtherClasses.find(RD);
  if (OtherIt == OtherClasses.end())
    return;

  const auto *OtherDecl = *OtherIt;

  const auto &CurMethods = RD->methods();
  const auto &CachedMethods = OtherDecl->methods();
  AbicornHash Hash;

  // Filter out the methods from the cached library, that are also in the
  // traversed one.
  std::set<const CXXMethodDecl *> PerfectMatchInCurrent;
  for (const auto *CachedMethod : CachedMethods) {
    for (const auto *CurMethod : CurMethods) {

      if (Hash(CurMethod) == Hash(CachedMethod) &&
          CurMethod->getMethodQualifiers() ==
              CachedMethod->getMethodQualifiers() &&
          CurMethod->getRefQualifier() == CachedMethod->getRefQualifier()) {
        PerfectMatchInCurrent.emplace(CachedMethod);
        break;
      }
    }
  }

  for (const auto *CurMethod : CurMethods) {
    MatchType MatchType = Unknown;
    const clang::CXXMethodDecl *BestMatch = nullptr;
    std::vector<const clang::CXXMethodDecl *> Overloads;

    for (const auto *CachedMethod : CachedMethods) {
      std::string CurDeclName = CurMethod->getDeclName().getAsString();
      std::string CachedDeclName = CachedMethod->getDeclName().getAsString();

      auto CurHash = Hash(CurMethod);
      auto CachedHash = Hash(CachedMethod);

      // Everything matches, we found the method.
      if (MatchType < Perfect && CurHash == CachedHash &&
          CurMethod->getMethodQualifiers() ==
              CachedMethod->getMethodQualifiers() &&
          CurMethod->getRefQualifier() == CachedMethod->getRefQualifier()) {
        MatchType = Perfect;
        BestMatch = CachedMethod;
      }
      // The return type, parameters, template specializations all match.
      else if (MatchType < UnqualifiedOnly && CurHash == CachedHash &&
               !PerfectMatchInCurrent.count(CachedMethod)) {
        MatchType = UnqualifiedOnly;
        BestMatch = CachedMethod;
      }
      // Only the names match.
      else if (MatchType < NameOnly && CurDeclName == CachedDeclName &&
               !PerfectMatchInCurrent.count(CachedMethod)) {
        MatchType = NameOnly;
        BestMatch = CachedMethod;
      }

      if (CurDeclName == CachedDeclName) {
        Overloads.emplace_back(CachedMethod);
      }
    }

    if ((MatchType == Perfect ||
         (MatchType == UnqualifiedOnly && Overloads.size() == 1)) &&
        CompareExistingEnabled) {
      checkMethodChanges(CurMethod, BestMatch);
      continue;
    }

    if (!CurMethod->isVirtual() && !CurMethod->isTrivial() &&
        !CurMethod->isImplicit() &&
        getContext().getTraversedLibrary() == Lib::Old &&
        !isa<CXXDestructorDecl>(CurMethod) && CheckMissingEnabled) {

      if (const auto *Ctor = dyn_cast<CXXConstructorDecl>(CurMethod)) {
        // FIXME: reason better about constructors
        if (Ctor->isDefaultConstructor() || Ctor->isCopyOrMoveConstructor() ||
            Ctor->getNumParams() == 0)
          continue;
      }

      if (Overloads.empty()) {
        reportMissingMethod(CurMethod);
      } else {
        reportMissingOverload(CurMethod);
        for (const auto &O : Overloads) {
          reportMissingNote(O);
        }
      }
    }
  }
}

} // namespace abicorn