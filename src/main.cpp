#include "framework/Config.h"
#include "framework/Registry.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang::tooling;
using namespace llvm;

class TjhlpCheckerConsumer : public clang::ASTConsumer {
public:
  TjhlpCheckerConsumer(clang::ast_matchers::MatchFinder &Finder)
      : Finder(Finder) {}

  void HandleTranslationUnit(clang::ASTContext &Context) override {
    Finder.matchAST(Context);
  }

private:
  clang::ast_matchers::MatchFinder &Finder;
};

// Action 负责创建 Rules 并将它们绑定到 Finder
class CheckerAction : public clang::ASTFrontendAction {
public:
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &CI, StringRef InFile) override {
    // 实例化注册表中的所有规则
    Rules = RuleRegistry::get().createAllRules();

    for (auto &rule : Rules) {

      rule->registerMatchers(Finder);
    }

    return std::make_unique<TjhlpCheckerConsumer>(Finder);
  }

private:
  clang::ast_matchers::MatchFinder Finder;
  std::vector<std::unique_ptr<RuleBase>> Rules;
};

static cl::OptionCategory TjhlpCheckerCategory("tjhlp-checker options");

static cl::opt<std::string> ConfigFile("config",
                                       cl::desc("Path to configuration file"),
                                       cl::value_desc("filename"),
                                       cl::init(".tjhlp-checker"));

int main(int argc, const char **argv) {
  auto ExpectedParser =
      CommonOptionsParser::create(argc, argv, TjhlpCheckerCategory);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser &OptionsParser = ExpectedParser.get();

  // 读取配置文件
  GlobalConfig::load(ConfigFile);

  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  return Tool.run(newFrontendActionFactory<CheckerAction>().get());
}