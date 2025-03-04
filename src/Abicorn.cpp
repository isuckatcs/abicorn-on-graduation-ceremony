#include <iostream>
#include <memory>
#include <vector>

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "Abicorn.h"
#include "AbicornContext.h"
#include "Checkers/CheckerStorage.h"
#include "LibBuilders/LibBuilderStorage.h"
#include "Utils/Util.h"

namespace abicorn {

using namespace clang;
using namespace clang::tooling;

namespace {
inline DiagnosticBuilder reportASTBuildError(DiagnosticsEngine &DiagEng) {

  unsigned ID = DiagEng.getDiagnosticIDs()->getCustomDiagID(
      DiagnosticIDs::Fatal, "failed to process the ASTs of the %0 library");
  return DiagEng.Report(ID);
}

ClangTool getCompilerInstance(const std::vector<std::string> &SourcePaths,
                              const CompilationDatabase &CompilationDB) {
  ClangTool Instance(CompilationDB, SourcePaths);
  return Instance;
}

bool buildASTs(const std::vector<std::string> &SourcePaths,
               const CompilationDatabase &CompilationDB,
               DiagnosticsEngine &DiagEng,
               std::vector<std::unique_ptr<ASTUnit>> &ASTs) {
  auto Compiler = getCompilerInstance(SourcePaths, CompilationDB);
  auto *DiagConsumer = DiagEng.getClient();

  Compiler.setDiagnosticConsumer(DiagConsumer);
  unsigned Ret = Compiler.buildASTs(ASTs);

  return Ret == 0 && DiagConsumer->getNumErrors() == 0;
}

class Pass {
  const std::vector<std::unique_ptr<ASTUnit>> &ASTs;
  AbicornContext &Context;

  // Initialize these lazily.
  std::unique_ptr<LibBuilderStorage> CacheBuilders = nullptr;
  std::unique_ptr<CheckerStorage> Checkers = nullptr;

public:
  Pass(const std::vector<std::unique_ptr<ASTUnit>> &ASTs,
       AbicornContext &Context)
      : ASTs(ASTs), Context(Context){};

  void addCacheBuilders(const LibDataRef &CacheBuilderData) {
    CacheBuilders =
        std::make_unique<LibBuilderStorage>(Context, CacheBuilderData);
  }

  void addCheckers(const LibDataRef &CheckerData,
                   Checker::Options Mode = Checker::CheckAndCompare) {
    Checkers = std::make_unique<CheckerStorage>(Context, CheckerData, Mode);
  }

  void run() {
    MatchFinder Finder;

    if (CacheBuilders) {
      CacheBuilders->registerBuilders(Finder);
    }

    if (Checkers) {
      Checkers->registerCheckers(Finder);
    }

    auto &DiagEng = Context.getDiagEngine();
    auto *DiagConsumer = DiagEng.getClient();

    for (size_t I = 0; I < ASTs.size(); ++I) {
      const auto &AST = ASTs[I];
      DiagEng.setSourceManager(&AST->getSourceManager());
      DiagConsumer->BeginSourceFile(AST->getLangOpts(),
                                    &AST->getPreprocessor());
      Finder.matchAST(AST->getASTContext());
      DiagConsumer->EndSourceFile();
    }
  }
};
} // namespace

void runAbicorn(AbicornContext &Context, const AbicornOptions &Options,
                const CompilationDatabase &OCD,
                const std::vector<std::string> &OldPaths,
                const CompilationDatabase &NCD,
                const std::vector<std::string> &NewPaths) {
  IntrusiveRefCntPtr<DiagnosticOptions> Opts(new DiagnosticOptions());
  Opts->ShowColors = Options.UseColor;
  auto *DiagConsumer = new TextDiagnosticPrinter(llvm::errs(), Opts.get());
  std::unique_ptr<DiagnosticsEngine> DiagEng(
      new DiagnosticsEngine(new DiagnosticIDs{}, Opts, DiagConsumer));

  Context.setDiagEngine(DiagEng.get());

  // Note that we want to compare the ASTs of 2 different libraries, so
  // their lifetimes must match, so using frontend actions is not an option.
  std::vector<std::unique_ptr<ASTUnit>> OldASTs;
  if (!buildASTs(OldPaths, OCD, *DiagEng, OldASTs)) {
    reportASTBuildError(*DiagEng) << "old";
    return;
  }

  std::vector<std::unique_ptr<ASTUnit>> NewASTs;
  if (!buildASTs(NewPaths, NCD, *DiagEng, NewASTs)) {
    reportASTBuildError(*DiagEng) << "new";
    return;
  }

  LibDataRef NewData = LibData::create();

  // Pass 1:
  //  - Extract data from the new library.
  Pass Pass1(NewASTs, Context);
  Pass1.addCacheBuilders(NewData);
  Pass1.run();

  // Pass 2:
  //  - Compare the extracted data with the old library.
  Context.setTraversedLibrary(Lib::Old);
  Context.setCachedLibrary(Lib::New);

  Pass Pass2(OldASTs, Context);
  Pass2.addCheckers(NewData);
  Pass2.run();
}
} // namespace abicorn