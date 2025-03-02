#include <cstddef>
#include <memory>
#include <utility>

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Tooling.h"

#include "gtest/gtest.h"

#include "Utils/Hash.h"

using namespace abicorn;

using namespace clang;
using namespace clang::ast_matchers;

template <typename T>
class DeclExtractor : public MatchFinder::MatchCallback {
  std::string ID;

public:
  const T *D1 = nullptr, *D2 = nullptr;

  DeclExtractor(std::string ID) : ID(std::move(ID)) {}

  void run(const MatchFinder::MatchResult &Result) override {
    if (const auto *FS = Result.Nodes.getNodeAs<T>(ID)) {
      if (D1 == nullptr) {
        D1 = FS;
      } else if (D2 == nullptr) {
        D2 = FS;
      } else {
        FAIL();
      }
    }
  }

  void clear() { D1 = D2 = nullptr; }
};

template <typename T>
class HashExtractor {
private:
  MatchFinder Finder;
  AbicornHash Hash;
  DeclExtractor<T> *DE;

public:
  HashExtractor(const DeclarationMatcher &Matcher, DeclExtractor<T> *Callback)
      : DE(Callback) {
    Finder.addMatcher(Matcher, DE);
  }

  std::pair<std::size_t, std::size_t> extractHashes(StringRef Source) {
    auto AST = tooling::buildASTFromCode(Source);
    DE->clear();

    Finder.matchAST(AST->getASTContext());

    DE->D1->dump();
    DE->D2->dump();

    return {Hash(DE->D1), Hash(DE->D2)};
  }
};

TEST(HashTest, FunctionHash) {
  DeclExtractor<FunctionDecl> DE("matcher");
  auto Matcher =
      traverse(TK_IgnoreUnlessSpelledInSource, functionDecl().bind("matcher"));
  HashExtractor HE(Matcher, &DE);

  size_t H1, H2;
  std::tie(H1, H2) = HE.extractHashes("void foo();"
                                      "void foo(){};");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("void foo();"
                                      "void foo(void){};");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("void foo();"
                                      "void bar(){};");
  ASSERT_NE(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("void foo();"
                                      "int foo();");
  ASSERT_NE(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("void foo();"
                                      "int foo(int);");
  ASSERT_NE(H1, H2);
}

TEST(HashTest, FunctionTemplateSpecHash) {
  DeclExtractor<FunctionDecl> DE("matcher");
  auto Matcher = traverse(
      TK_IgnoreUnlessSpelledInSource,
      functionDecl(unless(hasParent(functionTemplateDecl()))).bind("matcher"));
  HashExtractor HE(Matcher, &DE);

  size_t H1, H2;
  std::tie(H1, H2) = HE.extractHashes("template <int X, typename T> void foo();"
                                      "template <> void foo<0, int>();"
                                      "template <> void foo<0, int>();");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("template <int X, typename T> void foo();"
                                      "template <> void foo<0, float>();"
                                      "template <> void foo<0, int>();");
  ASSERT_NE(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("template <int X, typename T> void foo();"
                                      "template <> void foo<0, int>();"
                                      "template <> void foo<1, int>();");
  ASSERT_NE(H1, H2);
}

TEST(HashTest, RecordHash) {
  DeclExtractor<CXXRecordDecl> DE("matcher");
  auto Matcher = traverse(
      TK_IgnoreUnlessSpelledInSource,
      cxxRecordDecl(unless(hasParent(cxxRecordDecl()))).bind("matcher"));
  HashExtractor HE(Matcher, &DE);

  size_t H1, H2;
  std::tie(H1, H2) = HE.extractHashes("class C;"
                                      "class C;");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("class C;"
                                      "class C final{};");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("class C;"
                                      "class D;");
  ASSERT_NE(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("namespace a {class C;}"
                                      "namespace a {class C;}");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("namespace a {class C;}"
                                      "namespace b {class C;}");
  ASSERT_NE(H1, H2);
}

TEST(HashTest, RecordTemplateHash) {
  DeclExtractor<CXXRecordDecl> DE("matcher");
  auto Matcher = traverse(TK_IgnoreUnlessSpelledInSource,
                          classTemplateSpecializationDecl().bind("matcher"));
  HashExtractor HE(Matcher, &DE);

  size_t H1, H2;
  std::tie(H1, H2) = HE.extractHashes("template <int, typename T> class C;"
                                      "template <typename T> class C<0, T>;"
                                      "template <typename T> class C<0, T>;");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("template <int, typename T> class C;"
                                      "template <typename T> class C<0, T>;"
                                      "template <typename T> class C<1, T>;");
  ASSERT_NE(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("template <int, typename T> class C;"
                                      "template <> class C<0, int>;"
                                      "template <> class C<0, int>;");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("template <int, typename T> class C;"
                                      "template <> class C<0, int>;"
                                      "template <> class C<1, int>;");
  ASSERT_NE(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("template <int, typename T> class C;"
                                      "template <> class C<0, int>;"
                                      "template <> class C<0, float>;");
  ASSERT_NE(H1, H2);
}

TEST(HashTest, VariableHash) {
  DeclExtractor<VarDecl> DE("matcher");
  auto Matcher =
      traverse(TK_IgnoreUnlessSpelledInSource, varDecl().bind("matcher"));
  HashExtractor HE(Matcher, &DE);

  size_t H1, H2;
  std::tie(H1, H2) = HE.extractHashes("int X;"
                                      "int X");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("int X;"
                                      "float X");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("namespace a { int X; }"
                                      "namespace a { int X; }");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("namespace a { int X; }"
                                      "namespace b { int X; }");
  ASSERT_NE(H1, H2);
}

TEST(HashTest, VariableTemplateHash) {
  DeclExtractor<VarDecl> DE("matcher");
  auto Matcher = traverse(TK_IgnoreUnlessSpelledInSource,
                          varDecl(anyOf(hasDescendant(templateTypeParmDecl()),
                                        isExplicitTemplateSpecialization()))
                              .bind("matcher"));
  HashExtractor HE(Matcher, &DE);

  size_t H1, H2;
  std::tie(H1, H2) = HE.extractHashes("template <int, typename T> int X;"
                                      "template <typename T> int X<0, T>;"
                                      "template <typename T> int X<0, T>;");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("template <int, typename T> int X;"
                                      "template <typename T> int X<0, T>;"
                                      "template <typename T> int X<1, T>;");
  ASSERT_NE(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("template <int, typename T> int X;"
                                      "template <> int X<0, int>;"
                                      "template <> int X<0, int>;");
  ASSERT_EQ(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("template <int, typename T> int X;"
                                      "template <> int X<0, int>;"
                                      "template <> int X<1, int>;");
  ASSERT_NE(H1, H2);

  std::tie(H1, H2) = HE.extractHashes("template <int, typename T> int X;"
                                      "template <> int X<0, int>;"
                                      "template <> int X<0, float>;");
  ASSERT_NE(H1, H2);
}
