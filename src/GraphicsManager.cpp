#include <iostream>
#include <glfw3webgpu.h>

#include "spdlog/spdlog.h"
#include "glm/glm.hpp"

#include "Engine.h"
#include "GraphicsManager.h"

using namespace glm;

template< typename T > const T* to_ptr(const T& val) { return &val; }

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

namespace {
    struct InstanceData {
        vec3 translation;
        vec2 scale;
    };

    struct Uniforms {
        mat4 projection;
    };

    struct VertexInput {
        vec2 position;
        vec2 texCoords;
        vec3 translation;
        float scale;
    };

    struct VertexOutput {
        vec4 position;
        vec2 texCoords;
    };
}

GraphicsManager::GraphicsManager(int width, int height, std::string name, bool fs) {
	windowWidth = width;
	windowHeight = height;
	windowName = name;
	fullscreen = fs;
	engine = &globalEngine;
}

//Creates window
bool GraphicsManager::start() {
	glfwInit();
	// We don't want GLFW to set up a graphics API.
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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

    vertex_buffer = wgpuDeviceCreateBuffer(wgpuDevice, to_ptr(WGPUBufferDescriptor{
    .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex,
    .size = sizeof(vertices)
        }));

    wgpuQueueWriteBuffer(wgpuQueue, vertex_buffer, 0, vertices, sizeof(vertices));

    WGPUTextureFormat swap_chain_format = wgpuSurfaceGetPreferredFormat(wgpuSurface, wgpuAdapter);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    swapchain = wgpuDeviceCreateSwapChain(wgpuDevice, wgpuSurface, to_ptr(WGPUSwapChainDescriptor{
        .usage = WGPUTextureUsage_RenderAttachment,
        .format = swap_chain_format,
        .width = (uint32_t)width,
        .height = (uint32_t)height
        }));

    uniform_buffer = wgpuDeviceCreateBuffer(wgpuDevice, to_ptr(WGPUBufferDescriptor{
    .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform,
    .size = sizeof(Uniforms)
        }));

    sampler = wgpuDeviceCreateSampler(wgpuDevice, to_ptr(WGPUSamplerDescriptor{
    .addressModeU = WGPUAddressMode_ClampToEdge,
    .addressModeV = WGPUAddressMode_ClampToEdge,
    .magFilter = WGPUFilterMode_Linear,
    .minFilter = WGPUFilterMode_Linear,
    .maxAnisotropy = 1
        }));

    WGPUShaderModuleWGSLDescriptor code_desc = {};
    code_desc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    code_desc.code = source; // The shader source as a `char*`
    WGPUShaderModuleDescriptor shader_desc = {};
    shader_desc.nextInChain = &code_desc.chain;
    WGPUShaderModule shader_module = wgpuDeviceCreateShaderModule(wgpuDevice, &shader_desc);

	return true;
}

void GraphicsManager::shutdown() {
    wgpuSamplerRelease(sampler);
    wgpuBufferRelease(uniform_buffer);
    wgpuSwapChainRelease(swapchain);
    wgpuBufferRelease(vertex_buffer);
    wgpuQueueRelease(wgpuQueue);
    wgpuDeviceRelease(wgpuDevice);
    wgpuAdapterRelease(wgpuAdapter);
    wgpuSurfaceRelease(wgpuSurface);
    wgpuInstanceRelease(wgpuInstance);
    glfwTerminate();
}

void GraphicsManager::draw() {
	
}