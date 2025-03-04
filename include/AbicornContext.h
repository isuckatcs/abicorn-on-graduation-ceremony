#ifndef ABICORN_ABICORN_CONTEXT_H
#define ABICORN_ABICORN_CONTEXT_H

#include <string>

#include "clang/AST/DeclBase.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/Support/ErrorHandling.h"

namespace abicorn {
using namespace llvm;
using namespace clang;

class AbicornContext {
  DiagnosticsEngine *DiagEng;

public:
  AbicornContext() = default;
  AbicornContext(const AbicornContext &) = delete;
  AbicornContext &operator=(const AbicornContext &) = delete;

  DiagnosticBuilder diag(StringRef Name, SourceLocation Loc, StringRef Message,
                         ASTContext *ASTCtx,
                         DiagnosticIDs::Level Level = DiagnosticIDs::Warning);

  void setDiagEngine(DiagnosticsEngine *DiagEng) { this->DiagEng = DiagEng; }

  [[nodiscard]] DiagnosticsEngine &getDiagEngine() {
    assert(DiagEng && "Missing diagnostics engine");
    return *DiagEng;
  }

  static std::string getAccessStr(const clang::AccessSpecifier &AS);
};
} // namespace abicorn

#endif // ABICORN_ABICORN_CONTEXT_H
