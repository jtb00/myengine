#pragma once

//#define WEBGPU_RAII_DEBUG
//#define WEBGPU_RAII_LEAK
#include "webgpu_raii.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"

class GUIManager {
public:
	GUIManager();
	bool start(GLFWwindow* window, WGPUDeviceRef wgpuDevice, WGPUTextureFormat swap_chain_format);
	void shutdown();
	void draw(WGPUCommandEncoderRef encoder, WGPUTextureViewRef current_texture_view);
};