add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

add_requires("llvm", { kind = "library"})

target("tjhlp-checker")
    set_kind("binary")
    set_languages("c++latest")
    if is_plat("windows") then
        add_syslinks("version", "ntdll")
        set_runtimes(is_mode("debug") and "MTd" or "MT")
    end
    add_files("src/**.cpp")
    add_packages("llvm")
