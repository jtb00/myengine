add_rules("mode.debug", "mode.release")
add_requires("glfw")
add_requires("spdlog")
add_requires("wgpu-native", "glfw3webgpu")
add_requires("glm")
add_requires("stb")
add_requires("lua", "sol2")
add_requires("soloud")
add_requires("imgui", {configs = {glfw = true, wgpu = true}})

set_policy("build.warning", true) -- show warnings
set_warnings("all") -- warn about many things

target("myengine")
	set_kind("static")
	set_languages("cxx20")
	
	-- Declare our engine's header path.
    -- This allows targets that depend on the engine to #include them.
    add_includedirs("src", {public = true})
    
    -- Add all .cpp files in the `src` directory.
    add_files("src/*.cpp")
	
	add_packages("glfw", {public = true})
	add_packages("spdlog")
	add_packages("wgpu-native", "glfw3webgpu", {public = true})
	add_packages("glm", {public = true})
	add_packages("stb")
	add_packages("lua")
	add_packages("sol2", {public = true})
	add_packages("soloud", {public = true})
	add_packages("imgui")

target("helloworld")
    set_kind("binary")
    set_languages("cxx20")
	
	set_rundir("$(projectdir)")
	
	add_deps("myengine")
    
    add_files("demo/helloworld.cpp")
	
	add_packages("spdlog")