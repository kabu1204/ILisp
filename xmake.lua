set_version("0.0.1")
set_xmakever("2.5.1")
set_languages("cxx11")
set_project("ILisp")
set_targetdir("xmake-build")

add_rules("mode.release", "mode.debug", "mode.releasedbg")
if is_mode("release") then
    add_cxflags("-fno-elide-constructors")   -- 禁止优化临时对象的构造，否则Stack的拷贝有问题
end
if is_mode("releasedbg") then
    add_cxflags("-fno-elide-constructors")   -- 禁止优化临时对象的构造，否则Stack的拷贝有问题
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