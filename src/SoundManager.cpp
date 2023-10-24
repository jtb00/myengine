#include "Engine.h"
#include "ResourceManager.h"
#include "SoundManager.h"

SoundManager::SoundManager() {
	engine = &globalEngine;
	resources = &(engine->resources);
}

bool SoundManager::start() {
	sl.init();
	return true;
}

void SoundManager::shutdown() {
	sl.deinit();
}

void SoundManager::loadSound(const std::string& name, const std::string& path) {
	std::string soundPath = (*resources).resolvePath(path);
	SoLoud::Wav sound;
	sound.load(soundPath.c_str());
	nameToSound.insert(std::pair(name, sound));
}

bool SoundManager::destroySound(const std::string& name) {
	nameToSound.erase(name);
	return true;
}

void SoundManager::playSound(const std::string& name) {
	sl.play(nameToSound[name]);
}