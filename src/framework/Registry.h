#pragma once

#include "Rule.h"
#include <vector>
#include <functional>
#include <memory>

using RuleFactory = std::function<std::unique_ptr<RuleBase>()>;

class RuleRegistry {
public:
    static RuleRegistry &get() {
        static RuleRegistry instance;
        return instance;
    }

    void registerRule(RuleFactory factory) {
        factories.push_back(factory);
    }

    std::vector<std::unique_ptr<RuleBase>> createAllRules() {
        std::vector<std::unique_ptr<RuleBase>> rules;
        for (const auto &f : factories) {
            rules.push_back(f());
        }
        return rules;
    }

private:
    std::vector<RuleFactory> factories;
};

// 注册器辅助类
struct RuleRegisterer {
    RuleRegisterer(RuleFactory factory) {
        RuleRegistry::get().registerRule(factory);
    }
};

// 宏定义：这里的魔法在于 lambda
#define REGISTER_RULE(ConcreteRuleClass) \
    static RuleRegisterer global_##ConcreteRuleClass##_registerer([]() { \
        return std::make_unique<ConcreteRuleClass>(); \
    });