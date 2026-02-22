#include "GlobalVariablesMatcher.h"
using namespace clang::ast_matchers;

GlobalVariableMatcher::GlobalVariableMatcher()
    : matcher{varDecl(hasGlobalStorage(), unless(isExpansionInSystemHeader()))
                  .bind("globalVar")} {}