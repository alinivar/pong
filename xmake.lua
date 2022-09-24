
add_rules("mode.debug", "mode.release")

set_targetdir("bin")
set_objectdir("bin-int")

set_rundir(".")
set_warnings("allextra", "error")

add_requires("glfw", "glad")

target("pong")
    set_kind("binary")
    add_files("src/**.c")

    add_packages("glfw", "glad")
