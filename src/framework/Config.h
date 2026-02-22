#pragma once

#include "llvm/Support/YAMLTraits.h"

// 定义每个规则具体的配置结构体

// 对全局变量的限制
struct GlobalVariableOptions {
  bool Enabled;
  bool AllowConst; // 是否允许 const 全局变量
  // 最多允许的具有外部链接的全局变量个数, -1 表示不限制
  int MaxExternalGlobals;
  // 最多允许的具有内部链接（static）的全局变量个数，-1 表示不限制
  int MaxInternalGlobals;
};

struct NoGotoOptions {
  bool Enabled;
};

// 全局配置，变量名对应 YAML 中的顶层 Key
struct AppConfig {
  GlobalVariableOptions GlobalVariableRule;
  NoGotoOptions NoGotoRule;
};

// 3. LLVM YAML 映射特化 (Boilerplate, 但只需要写一次)
namespace llvm {
namespace yaml {

// 映射 GlobalVariableOptions
template <> struct MappingTraits<GlobalVariableOptions> {
  static void mapping(IO &io, GlobalVariableOptions &info) {
    io.mapOptional("Enabled", info.Enabled, true); // 默认 true
    io.mapOptional("AllowConst", info.AllowConst, true);
    io.mapOptional("MaxExternalGlobals", info.MaxExternalGlobals, 0);
    io.mapOptional("MaxInternalGlobals", info.MaxInternalGlobals, 0);
  }
};

// 映射 NoGotoOptions
template <> struct MappingTraits<NoGotoOptions> {
  static void mapping(IO &io, NoGotoOptions &info) {
    io.mapOptional("Enabled", info.Enabled, true);
  }
};

// 映射 AppConfig
template <> struct MappingTraits<AppConfig> {
  static void mapping(IO &io, AppConfig &info) {
    // 这里决定了 YAML 的顶层 Key
    io.mapOptional("GlobalVariableRule", info.GlobalVariableRule);
    io.mapOptional("NoGotoRule", info.NoGotoRule);
  }
};

} // namespace yaml
} // namespace llvm


class GlobalConfig {
public:
  GlobalConfig(const GlobalConfig &) = delete;
  void operator=(const GlobalConfig &) = delete;

  static const AppConfig &get() {
    return instance().ConfigData;
  }

  // 初始化函数：通常应当在 main 开头调用
  // 返回 true 表示成功
  static bool load(llvm::StringRef FilePath) {
    if (FilePath.empty()) return true; // 没有配置文件，直接使用默认值

    auto BufferOrError = llvm::MemoryBuffer::getFile(FilePath);
    if (!BufferOrError) {
      llvm::errs() << "Error reading config file: " << FilePath << "\n";
      return false;
    }

    llvm::yaml::Input yin(BufferOrError.get()->getMemBufferRef());
    yin >> instance().ConfigData;

    if (yin.error()) {
      llvm::errs() << "Error parsing YAML: " << yin.error().message() << "\n";
      return false;
    }
    
    return true;
  }

private:
  AppConfig ConfigData;

  GlobalConfig() = default;

  static GlobalConfig &instance() {
    static GlobalConfig Instance;
    return Instance;
  }
};
