set_version("0.0.1")
set_xmakever("2.5.1")
set_languages("cxx11")
set_project("ILisp")
set_targetdir("xmake-build")

add_rules("mode.release", "mode.debug")
if is_mode("relwithdeb") then
    set_symbols("debug")
    set_optimize("faster")
    set_strip("all")
end

if is_mode("debug") then
    -- 添加DEBUG编译宏
    add_defines("DEBUG")
end

target("ilisp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_links("editline","re2")
    add_linkdirs("/usr/local/lib")
    add_includedirs("/usr/local/include")