#pragma once

#include <string>
#include <webgpu/webgpu.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class Engine;

//handles graphics
class GraphicsManager {
public:
	//width of window in pixels
	int windowWidth;
	//height of window in pixels
	int windowHeight;
	std::string windowName;
	bool fullscreen;
	//pointer to globalEngine
	Engine* engine;
	//pointer to window
	GLFWwindow* window;
	WGPUInstance wgpuInstance;
	WGPUSurface wgpuSurface;
	WGPUAdapter wgpuAdapter;
	WGPUDevice wgpuDevice;
	WGPUQueue wgpuQueue;
	WGPUBuffer vertex_buffer;
	WGPUSwapChain swapchain;
	WGPUBuffer uniform_buffer;
	WGPUSampler sampler;

	GraphicsManager(int width = 640, int height = 480, std::string name = "Window", bool fs = false);
	bool start();
	void shutdown();
	void draw();
};