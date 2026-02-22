#pragma once

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/FrontendActions.h>

class GlobalVariableMatchingCallback
    : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
  void
  run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override {
    if (const auto *Var = Result.Nodes.getNodeAs<clang::VarDecl>("globalVar")) {
      llvm::outs() << "Found global variable: " << Var->getNameAsString() << " " << Var->getLocation().printToString(*Result.SourceManager)
                   << "\n";
    }
  }
};

class GlobalVariableMatcher {

public:
  GlobalVariableMatcher();
  clang::ast_matchers::DeclarationMatcher matcher;
  GlobalVariableMatchingCallback Callback;
};