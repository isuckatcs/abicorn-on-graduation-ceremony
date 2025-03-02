#ifndef ABICORN_ABICORN_CONTEXT_H
#define ABICORN_ABICORN_CONTEXT_H

#include <string>

#include "clang/AST/DeclBase.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/Support/ErrorHandling.h"

namespace abicorn {
using namespace llvm;
using namespace clang;

struct Lib {
  enum Version { Old, New, Unknown };

  static std::string getVersionAsString(const Version &V) {
    switch (V) {

    case Old:
      return "old";
    case New:
      return "new";
    case Unknown:
      return "unknown";
    default:
      llvm_unreachable("Unexpected library version");
    }
  };
};

class AbicornContext {
  DiagnosticsEngine *DiagEng;

  Lib::Version TraversedLibrary = Lib::Unknown;
  Lib::Version CachedLibrary = Lib::Unknown;

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

  void setTraversedLibrary(const Lib::Version &L) { TraversedLibrary = L; }

  [[nodiscard]] Lib::Version getTraversedLibrary() const {
    return TraversedLibrary;
  }

  [[nodiscard]] std::string getTraversedLibraryStr() const {
    return Lib::getVersionAsString(TraversedLibrary);
  }

  void setCachedLibrary(const Lib::Version &L) { CachedLibrary = L; }

  [[nodiscard]] Lib::Version getCachedLibrary() const { return CachedLibrary; }

  [[nodiscard]] std::string getCachedLibraryStr() const {
    return Lib::getVersionAsString(CachedLibrary);
  }

  static std::string getAccessStr(const clang::AccessSpecifier &AS);
};
} // namespace abicorn

#endif // ABICORN_ABICORN_CONTEXT_H
