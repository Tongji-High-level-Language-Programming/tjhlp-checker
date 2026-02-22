#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/TypeName.h"

class RuleBase : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
  virtual ~RuleBase() = default;

  // 每个规则必须实现这个方法：向 Finder 注册自己的 Matcher
  // 并将 'this' 作为回调传递进去
  virtual void registerMatchers(clang::ast_matchers::MatchFinder &Finder) = 0;

  virtual llvm::StringRef name() const = 0;
};

template <typename Derived> class Rule : public RuleBase {
public:
  llvm::StringRef name() const override {
    static const llvm::StringRef TypeName = []() {
      llvm::StringRef FullName = llvm::getTypeName<Derived>();
      // 去掉命名空间
      auto LastColon = FullName.rfind(':');
      return (LastColon == llvm::StringRef::npos)
                 ? FullName
                 : FullName.substr(LastColon + 1);
    }();
    return TypeName;
  }
};
