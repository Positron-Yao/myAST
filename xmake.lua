add_rules("mode.debug", "mode.release")

target("SyntaxParser")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("include")

