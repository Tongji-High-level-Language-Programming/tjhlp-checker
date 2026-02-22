#include "../framework/Config.h"
#include "../framework/Registry.h"
#include "../framework/Rule.h"

#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchers.h"

#include <llvm/Support/raw_ostream.h>

using namespace clang::ast_matchers;

class GlobalVarCheck : public Rule<GlobalVarCheck> {
public:
  void registerMatchers(MatchFinder &Finder) override {
    const auto &Options = GlobalConfig::get().GlobalVariableRule;
    
    if (!Options.Enabled) {
      return;
    }

    Finder.addMatcher(
        varDecl(hasGlobalStorage(), unless(isExpansionInSystemHeader()),
                hasDescendant(qualType(unless(isConstQualified()))))
            .bind("var"),
        this);
  }

  void onStartOfTranslationUnit() override {}

  void run(const MatchFinder::MatchResult &Result) override {
    // 调用 name() 会自动返回 "MyGlobalVarCheck"
    const auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var");
    VD->dump();
    llvm::outs() << VD->getType().isConstQualified() << "\n";
    llvm::outs() << "[" << name()
                 << "] Found global variable: " << VD->getNameAsString() << " "
                 << VD->getLocation().printToString(
                        Result.Context->getSourceManager())
                 << "\n";
  }
};

REGISTER_RULE(GlobalVarCheck)
