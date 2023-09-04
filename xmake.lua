add_rules("mode.debug", "mode.release")
add_requires("glfw")
add_requires("spdlog")

set_policy("build.warning", true) -- show warnings
set_warnings("all") -- warn about many things

target("illengine")
	set_kind("static")
	set_languages("cxx17")
	
	-- Declare our engine's header path.
    -- This allows targets that depend on the engine to #include them.
    add_includedirs("src", {public = true})
    
    -- Add all .cpp files in the `src` directory.
    add_files("src/*.cpp")
	
	add_packages("glfw", {public = true})
	add_packages("spdlog")

target("helloworld")
    set_kind("binary")
    set_languages("cxx17")
	
	add_deps("illengine")
    
    add_files("demo/helloworld.cpp")
	
	add_packages("spdlog")