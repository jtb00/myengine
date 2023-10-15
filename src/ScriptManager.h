#pragma once

#include <string>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

struct Script {
	std::string name;
};

//manages Lua scripts
class ScriptManager {
public:
	sol::state lua;
	std::unordered_map<std::string, sol::load_result> nameToLoadResult;

	ScriptManager();
	bool start();
	void update();
	bool LoadScript(const std::string& name, const std::string& path);
	sol::load_result& GetScript(const std::string& name);
};