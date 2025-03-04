#include "clang/AST/ASTContext.h"
#include "clang/AST/Stmt.h"
#include "llvm/ADT/Twine.h"

#include "AbicornContext.h"

namespace abicorn {
using namespace llvm;

DiagnosticBuilder AbicornContext::diag(StringRef Name, SourceLocation Loc,
                                       StringRef Message, ASTContext *ASTCtx,
                                       DiagnosticIDs::Level Level) {
  assert(DiagEng && "Missing diagnostics engine");
  assert(Loc.isValid() && "Invalid source location");
  assert(ASTCtx && "AST context is nullptr");

  // To get the diagnostics properly rendered we make the diagnostics renderer
  // believe, we switched source files.
  const auto &SM = ASTCtx->getSourceManager();
  if (&DiagEng->getSourceManager() != &SM) {
    DiagEng->getClient()->EndSourceFile();
    DiagEng->getClient()->BeginSourceFile(ASTCtx->getLangOpts());

    DiagEng->setSourceManager(&ASTCtx->getSourceManager());
  }

  unsigned ID = DiagEng->getDiagnosticIDs()->getCustomDiagID(
      Level, (Message + " [" + Name + "]").str());
  return DiagEng->Report(Loc, ID);
}

std::string AbicornContext::getAccessStr(const clang::AccessSpecifier &AS) {
  switch (AS) {
  case AS_public:
    return "public";
  case AS_protected:
    return "protected";
  case AS_private:
    return "private";
  default:
    return "none";
  };
}

} // namespace abicorn