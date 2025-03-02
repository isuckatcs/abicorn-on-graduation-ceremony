#include <iostream>
#include <memory>

#include "clang/Tooling/CompilationDatabase.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"

#include "Abicorn.h"
#include "AbicornContext.h"
#include "Options.h"

using namespace llvm;
using namespace abicorn;
using namespace clang::tooling;

static std::unique_ptr<CompilationDatabase>
extractCompilationDB(int Argc, const char **Argv,
                     std::vector<std::string> Sources, StringRef BuildPath) {
  std::string ErrorStr;
  std::unique_ptr<CompilationDatabase> CompilationDB =
      FixedCompilationDatabase::loadFromCommandLine(Argc, Argv, ErrorStr);
  if (!ErrorStr.empty()) {
    llvm::errs() << ErrorStr << '\n';
  }

  if (!CompilationDB) {
    if (!BuildPath.empty())
      CompilationDB = FixedCompilationDatabase::autoDetectFromDirectory(
          BuildPath, ErrorStr);
    else
      CompilationDB =
          FixedCompilationDatabase::autoDetectFromSource(Sources[0], ErrorStr);
  }

  if (!CompilationDB) {
    llvm::errs() << ErrorStr << '\n' << "Running withoug flags\n";
    CompilationDB = std::make_unique<FixedCompilationDatabase>(
        ".", std::vector<std::string>());
  }

  return CompilationDB;
};

static cl::OptionCategory ToolCategory("abicorn options");

static cl::opt<std::string>
    OldBuildPath("p-old",
                 cl::desc("Path to the build directory of the old library"),
                 cl::Optional, cl::cat(ToolCategory));

static cl::opt<std::string>
    NewBuildPath("p-new",
                 cl::desc("Path to the build directory of the new library"),
                 cl::Optional, cl::cat(ToolCategory));

static cl::list<std::string>
    OldInputFilenames("old", cl::desc("<Old source 0> [... <Old source N>]"),
                      cl::Positional, cl::cat(ToolCategory));

static cl::list<std::string>
    NewInputFilenames("new", cl::desc("<New source 0> [... <New source N>]"),
                      cl::Positional, cl::cat(ToolCategory));

static cl::list<std::string>
    OldInputSettings("s-old", cl::desc("<Setting 0> [... <Setting N>]"),
                     cl::Optional, cl::PositionalEatsArgs,
                     cl::cat(ToolCategory));

static cl::list<std::string>
    NewInputSettings("s-new", cl::desc("<Setting 0> [... <Setting N>]"),
                     cl::Optional, cl::PositionalEatsArgs,
                     cl::cat(ToolCategory));

static cl::opt<bool> NoColors("no-color",
                              cl::desc("Emit warnings that are not colorful"),
                              cl::cat(ToolCategory));

int main(int argc, const char **argv) {
  InitLLVM X(argc, argv);

  cl::HideUnrelatedOptions(ToolCategory);

  // Split argv before and after --. This is needed because the flags after --
  // will be appended to one of the lists, which we want to avoid. The arguments
  // after -- should only be passed down to the compiler instances.
  std::vector<const char *> FilteredArgv;
  for (int I = 0; I < argc; ++I) {
    if (!strcmp("--", argv[I]))
      break;

    FilteredArgv.emplace_back(argv[I]);
  }

  if (!cl::ParseCommandLineOptions(static_cast<int>(FilteredArgv.size()),
                                   FilteredArgv.data())) {
    llvm::errs() << "Error: failed to parse command line arguments\n";
    return 1;
  };

  if (OldInputFilenames.empty()) {
    llvm::errs() << "Error: old input files not specified\n";
    cl::PrintHelpMessage();
    return 1;
  }

  if (NewInputFilenames.empty()) {
    llvm::errs() << "Error: new input files not specified\n";
    cl::PrintHelpMessage();
    return 1;
  }

  AbicornContext Context;
  AbicornOptions Options;
  Options.UseColor = !NoColors;

  std::vector<const char *> OldArgs = FilteredArgv;
  if (OldBuildPath.getValue().empty()) {
    OldArgs.push_back("--");
    for (auto &&S : OldInputSettings)
      OldArgs.push_back(S.c_str());
  }

  std::vector<const char *> NewArgs = FilteredArgv;
  if (NewBuildPath.getValue().empty()) {
    NewArgs.push_back("--");
    for (auto &&S : NewInputSettings)
      NewArgs.push_back(S.c_str());
  }

  // Build the compilation databases.
  std::unique_ptr<CompilationDatabase> OldCompilationDB = extractCompilationDB(
      OldArgs.size(), OldArgs.data(), OldInputFilenames, OldBuildPath);
  std::unique_ptr<CompilationDatabase> NewCompilationDB = extractCompilationDB(
      NewArgs.size(), NewArgs.data(), NewInputFilenames, NewBuildPath);

  runAbicorn(Context, Options, *OldCompilationDB, OldInputFilenames,
             *NewCompilationDB, NewInputFilenames);

  return 0;
}