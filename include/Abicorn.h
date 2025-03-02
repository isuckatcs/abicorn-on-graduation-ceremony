#ifndef ABICORN_ABICORN_H
#define ABICORN_ABICORN_H

#include <iosfwd>
#include <vector>

#include "clang/Tooling/Core/Diagnostic.h"
#include "clang/Tooling/Tooling.h"

#include "AbicornContext.h"
#include "Options.h"

namespace abicorn {
void runAbicorn(AbicornContext &Context, const AbicornOptions &Options,
                const clang::tooling::CompilationDatabase &OCD,
                const std::vector<std::string> &OldPaths,
                const clang::tooling::CompilationDatabase &NCD,
                const std::vector<std::string> &NewPaths);
} // namespace abicorn

#endif // ABICORN_ABICORN_H
