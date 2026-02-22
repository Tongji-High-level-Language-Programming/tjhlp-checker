cmake_minimum_required(VERSION 3.31)
project(tjhlp-checker)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

llvm_map_components_to_libnames(llvm_libs support core)
add_definitions(${LLVM_DEFINITIONS})

message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")

add_executable(tjhlp-checker 
    src/main.cpp
    src/rules/GlobalVariablesRule.cpp
)

target_link_libraries(tjhlp-checker
    PRIVATE
    ${llvm_libs}
    clangTooling
    clangBasic
    clangASTMatchers
    clangFrontend
)

target_include_directories(tjhlp-checker
    PRIVATE
    ${LLVM_INCLUDE_DIRS}
)
