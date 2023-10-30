#pragma once

#include <string>
#include <unordered_map>
#include "soloud.h"
#include "soloud_wav.h"

//class Engine;
//class ResourceManager;

class SoundManager {
public:
	SoLoud::Soloud sl;
	std::unordered_map< std::string, SoLoud::Wav > nameToSound;
	//pointer to globalEngine
	Engine* engine;
	//pointer to ResourceManager
	ResourceManager* resources;

	SoundManager();
	bool start();
	void shutdown();
	void loadSound(const std::string& name, const std::string& path);
	bool destroySound(const std::string& name);
	void playSound(const std::string& name);
};