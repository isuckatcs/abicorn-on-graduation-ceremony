#include "clang/AST/ASTContext.h"

#include "Checkers/ClassChecker.h"
#include "Matchers/Matchers.h"

namespace abicorn {
using namespace abicorn::matchers;

void ClassChecker::registerMatchers(MatchFinder &Finder) {
  Finder.addMatcher(GeneralRecordMatcher::Matcher, this);
};

void ClassChecker::reportMissingClass(const CXXRecordDecl *RD) {

  std::string Name = RD->getDeclName().getAsString();

  if (const auto *TS = dyn_cast<ClassTemplateSpecializationDecl>(RD)) {
    diag(RD->getLocation(),
         "%0specialization of '%1' is not defined in the new library",
         &RD->getASTContext())
        << (isa<ClassTemplatePartialSpecializationDecl>(TS) ? "partial " : "")
        << Name;
  } else {
    diag(RD->getLocation(), "'%0' is not defined in the new library",
         &RD->getASTContext())
        << Name;
  }
}

bool ClassChecker::hasAnonymousFields(const CXXRecordDecl *RD) {
  for (auto &&Field : RD->fields())
    if (Field->getName().empty() && !Field->isAnonymousStructOrUnion())
      return true;

  return false;
}

void ClassChecker::checkFinalityMismatch(const CXXRecordDecl *Traversed,
                                         const CXXRecordDecl *Cached) {
  if (Traversed->isEffectivelyFinal() == Cached->isEffectivelyFinal())
    return;

  std::string Name = Cached->getNameAsString();

  diag(Cached->getLocation(), "change in the finality of record '%0'",
       &Cached->getASTContext())
      << Name;

  diag(Traversed->getLocation(),
       "record '%0' is declared as '%1' in the old library",
       &Traversed->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (Traversed->isEffectivelyFinal() ? "final" : "non-final");

  diag(Cached->getLocation(),
       "record '%0' is declared as '%1' in the new library",
       &Cached->getASTContext(), clang::DiagnosticIDs::Note)
      << Name << (Cached->isEffectivelyFinal() ? "final" : "non-final");
}

void ClassChecker::checkSizeMismatch(const CXXRecordDecl *Traversed,
                                     const CXXRecordDecl *Cached) {

  ASTContext &TraversedContext = Traversed->getASTContext();
  QualType TraversedType = TraversedContext.getRecordType(Traversed);

  ASTContext &CachedContext = Cached->getASTContext();
  QualType CachedType = CachedContext.getRecordType(Cached);

  if (TraversedType->isDependentType() || CachedType->isDependentType())
    return;

  if (!hasAnonymousFields(Traversed) && !hasAnonymousFields(Cached))
    return;

  std::size_t TraversedSize = TraversedContext.getTypeSize(TraversedType);
  std::size_t CachedSize = CachedContext.getTypeSize(CachedType);

  if (TraversedSize == CachedSize)
    return;

  std::string Name = Cached->getNameAsString();

  diag(Cached->getLocation(),
       "change in size of record '%0' with anonymous fields", &CachedContext)
      << Cached->getNameAsString();

  diag(Traversed->getLocation(),
       "record '%0' occupies %1 bits in the old library", &TraversedContext,
       clang::DiagnosticIDs::Note)
      << Name << TraversedSize;

  diag(Cached->getLocation(), "record '%0' occupies %1 bits in the new library",
       &CachedContext, clang::DiagnosticIDs::Note)
      << Name << CachedSize;
}

void ClassChecker::check(const MatchFinder::MatchResult &Result) {
  const auto *RD =
      Result.Nodes.getNodeAs<CXXRecordDecl>(GeneralRecordMatcher::Id);

  if (!RD || !RD->isThisDeclarationADefinition() ||
      RD->getDeclName().isEmpty()) {
    return;
  }

  const auto &RecordsInNewLib = Data->getRecords();
  if (RecordsInNewLib.count(RD) == 0) {
    reportMissingClass(RD);
    return;
  }

  const CXXRecordDecl *NewRecord = *RecordsInNewLib.find(RD);
  assert(NewRecord && "nullptr returned from stored records set.");

  checkFinalityMismatch(RD, NewRecord);
  checkSizeMismatch(RD, NewRecord);
};

} // namespace abicorn
