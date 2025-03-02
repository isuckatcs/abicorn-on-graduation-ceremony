#ifndef ABICORN_FIELD_CHECKER_H
#define ABICORN_FIELD_CHECKER_H

#include <optional>
#include <vector>

#include "Checker.h"
#include "LibData.hpp"

namespace abicorn {
class FieldChecker : public Checker {
public:
  FieldChecker(AbicornContext &Context, const LibDataRef &Data,
               Options Mode = CheckAndCompare)
      : Checker("field-checker", Context, Data, Mode) {}

  void registerMatchers(MatchFinder &Finder) override;

  void check(const MatchFinder::MatchResult &Result) override;

private:
  struct DeclInfo {
    const Decl *D;
    AccessSpecifier Access;
    std::optional<size_t> EncapsulatingUnionIndex;
  };

  bool CheckingNewLib = true;

  std::vector<DeclInfo> collectDecls(const RecordDecl *) const;

  void checkAllDecls(const std::vector<DeclInfo> &,
                     const std::vector<DeclInfo> &, bool, bool);

  void checkTypeEquality(const StringRef, const DeclaratorDecl *,
                         const DeclaratorDecl *);
  void checkIndexEquality(const StringRef, const FieldDecl *,
                          std::optional<size_t>, const FieldDecl *,
                          std::optional<size_t>);
  void checkAccessSpecifierEquality(const StringRef, const DeclaratorDecl *,
                                    AccessSpecifier, const DeclaratorDecl *,
                                    AccessSpecifier);
  void checkStorageEquality(const DeclaratorDecl *, const DeclaratorDecl *);

  void reportMissingField(const DeclaratorDecl *);
  void reportTypeMissmatch(const StringRef, const DeclaratorDecl *,
                           const DeclaratorDecl *);
  void reportIndexMissmatch(const StringRef, const FieldDecl *, size_t,
                            const FieldDecl *, size_t);
  void reportAccessSpecifierMissmatch(const StringRef, const DeclaratorDecl *,
                                      AccessSpecifier, const DeclaratorDecl *,
                                      AccessSpecifier);
  void reportStorageChange(const StringRef, const DeclaratorDecl *,
                           const DeclaratorDecl *, bool);
};
} // namespace abicorn

#endif // ABICORN_FIELD_CHECKER_H
