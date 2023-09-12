#include <iostream>
#include <glfw3webgpu.h>

#include "spdlog/spdlog.h"

#include "Engine.h"
#include "GraphicsManager.h"

template< typename T > const T* to_ptr(const T& val) { return &val; }

GraphicsManager::GraphicsManager(int width, int height, std::string name, bool fs) {
	windowWidth = width;
	windowHeight = height;
	windowName = name;
	fullscreen = fs;
	engine = &globalEngine;
}

// A vertex buffer containing a textured square.
const struct {
    // position
    float x, y;
    // texcoords
    float u, v;
} vertices[] = {
    // position       // texcoords
  { -1.0f,  -1.0f,    0.0f,  1.0f },
  {  1.0f,  -1.0f,    1.0f,  1.0f },
  { -1.0f,   1.0f,    0.0f,  0.0f },
  {  1.0f,   1.0f,    1.0f,  0.0f },
};

//Creates window
bool GraphicsManager::start() {
	glfwInit();
	// We don't want GLFW to set up a graphics API.
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// Create the window.
	window = glfwCreateWindow( windowWidth, windowHeight, windowName.c_str(), fullscreen ? glfwGetPrimaryMonitor() : 0, 0 );
	glfwSetWindowAspectRatio( window, windowWidth, windowHeight );
	if( !window ) {
		spdlog::error("Failed to create a window.");
		glfwTerminate();
		return false;
	}
	wgpuInstance = wgpuCreateInstance(to_ptr(WGPUInstanceDescriptor{}));
	if (wgpuInstance == NULL) {
		spdlog::error("Failed to initialize WebGPU.");
		glfwTerminate();
		return false;
	}
    wgpuSurface = glfwGetWGPUSurface(wgpuInstance, window);

    wgpuAdapter = nullptr;
    wgpuInstanceRequestAdapter(
        wgpuInstance,
        to_ptr(WGPURequestAdapterOptions{ .compatibleSurface = wgpuSurface }),
        [](WGPURequestAdapterStatus status, WGPUAdapter wgpuAdapter, char const* message, void* adapter_ptr) {
            if (status != WGPURequestAdapterStatus_Success) {
                std::cerr << "Failed to get a WebGPU wgpuAdapter: " << message << std::endl;
                glfwTerminate();
            }

    *static_cast<WGPUAdapter*>(adapter_ptr) = wgpuAdapter;
        },
        &(wgpuAdapter)
            );

    wgpuDevice = nullptr;
    wgpuAdapterRequestDevice(
        wgpuAdapter,
        nullptr,
        [](WGPURequestDeviceStatus status, WGPUDevice wgpuDevice, char const* message, void* device_ptr) {
            if (status != WGPURequestDeviceStatus_Success) {
                std::cerr << "Failed to get a WebGPU wgpuDevice: " << message << std::endl;
                glfwTerminate();
            }

    *static_cast<WGPUDevice*>(device_ptr) = wgpuDevice;
        },
        &(wgpuDevice)
            );

    // An error callback to help with debugging
    wgpuDeviceSetUncapturedErrorCallback(
        wgpuDevice,
        [](WGPUErrorType type, char const* message, void*) {
            std::cerr << "WebGPU uncaptured error type " << type << " with message: " << message << std::endl;
        },
        nullptr
            );

    wgpuQueue = wgpuDeviceGetQueue(wgpuDevice);
    WGPUBuffer vertex_buffer = wgpuDeviceCreateBuffer(wgpuDevice, to_ptr(WGPUBufferDescriptor{
    .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex,
    .size = sizeof(vertices)
        }));
	return true;
}

void GraphicsManager::shutdown() {
    wgpuQueueRelease(wgpuQueue);
    wgpuDeviceRelease(wgpuDevice);
    wgpuAdapterRelease(wgpuAdapter);
    wgpuSurfaceRelease(wgpuSurface);
    wgpuInstanceRelease(wgpuInstance);
    glfwTerminate();
}

void GraphicsManager::draw() {
	
}