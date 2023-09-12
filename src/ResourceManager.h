#pragma once

#include <string>

//resolves resource paths
class ResourceManager {
public:
	ResourceManager(void);
	bool start();
	void shutdown();
	std::string resolvePath(std::string path);
};