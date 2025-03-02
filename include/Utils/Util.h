#ifndef ABICORN_UTIL_H
#define ABICORN_UTIL_H

#include <string>

namespace abicorn {
#ifdef CLANG_STDLIB_PATH
static const std::string ClangStdlibPath = CLANG_STDLIB_PATH;
#else
static const std::string ClangStdlibPath;
#endif

} // namespace abicorn

#endif // ABICORN_UTIL_H
