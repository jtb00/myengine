#pragma once

#include <string>
#include <webgpu/webgpu.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
//#define WEBGPU_RAII_DEBUG
//#define WEBGPU_RAII_LEAK
#include "webgpu_raii.h"

#include "Types.h"

class Engine;
class ResourceManager;

struct ImageData {
	int width;
	int height;
	WGPUTextureRef texture;
	WGPUBindGroupRef bind_group;

	ImageData(int w, int h, WGPUTextureRef t) {
		width = w;
		height = h;
		texture = t;
	}

	/*
	~ImageData() {
		if (texture != nullptr) {
			wgpuTextureDestroy(texture);
			wgpuTextureRelease(texture);
		}
	}
	*/
};

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
	//pointer to ResourceManager
	ResourceManager* resources;
	//pointer to window
	GLFWwindow* window;
	//Name-to-image map
	std::unordered_map<std::string, ImageData> nameToImage;
	WGPUInstanceRef wgpuInstance;
	WGPUSurfaceRef wgpuSurface;
	WGPUAdapterRef wgpuAdapter;
	WGPUDeviceRef wgpuDevice;
	WGPUQueueRef wgpuQueue;
	WGPUBufferRef vertex_buffer;
	WGPUSwapChainRef swapchain;
	WGPUBufferRef uniform_buffer;
	WGPUSamplerRef sampler;
	WGPUShaderModuleRef shader_module;
	WGPURenderPipelineRef pipeline;

	GraphicsManager(int width = 640, int height = 480, std::string name = "Window", bool fs = false);
	bool start();
	void shutdown();
	bool loadImg(const std::string& name, const std::string& path);
	void draw();
};