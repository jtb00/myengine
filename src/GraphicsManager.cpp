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

    const char* source = R"(struct Uniforms {
    projection: mat4x4f,
};

@group(0) @binding(0) var<uniform> uniforms: Uniforms;
@group(0) @binding(1) var texSampler: sampler;
@group(0) @binding(2) var texData: texture_2d<f32>;

struct VertexInput {
    @location(0) position: vec2f,
    @location(1) texcoords: vec2f,
    @location(2) translation: vec3f,
    @location(3) scale: f32,
};

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) texcoords: vec2f,
};

@vertex
fn vertex_shader_main( in: VertexInput ) -> VertexOutput {
    var out: VertexOutput;
    out.position = uniforms.projection * vec4f( vec3f( in.scale * in.position, 0.0 ) + in.translation, 1.0 );
    out.texcoords = in.texcoords;
    return out;
}

@fragment
fn fragment_shader_main( in: VertexOutput ) -> @location(0) vec4f {
    let color = textureSample( texData, texSampler, in.texcoords ).rgba;
    return color;
})";

    WGPUShaderModuleWGSLDescriptor code_desc = {};
    code_desc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    code_desc.code = source; // The shader source as a `char*`
    WGPUShaderModuleDescriptor shader_desc = {};
    shader_desc.nextInChain = &code_desc.chain;
    WGPUShaderModule shader_module = wgpuDeviceCreateShaderModule(wgpuDevice, &shader_desc);

    WGPURenderPipeline pipeline = wgpuDeviceCreateRenderPipeline(wgpuDevice, to_ptr(WGPURenderPipelineDescriptor{

        // Describe the vertex shader inputs
        .vertex = {
            .module = shader_module,
            .entryPoint = "vertex_shader_main",
            // Vertex attributes.
            .bufferCount = 2,
            .buffers = (WGPUVertexBufferLayout[]){
            // We have one buffer with our per-vertex position and UV data. This data never changes.
            // Note how the type, byte offset, and stride (bytes between elements) exactly matches our `vertex_buffer`.
            {
                .arrayStride = 4 * sizeof(float),
                .attributeCount = 2,
                .attributes = (WGPUVertexAttribute[]){
            // Position x,y are first.
            {
                .format = WGPUVertexFormat_Float32x2,
                .offset = 0,
                .shaderLocation = 0
            },
        // Texture coordinates u,v are second.
        {
            .format = WGPUVertexFormat_Float32x2,
            .offset = 2 * sizeof(float),
            .shaderLocation = 1
        }
        }
},
// We will use a second buffer with our per-sprite translation and scale. This data will be set in our draw function.
{
    .arrayStride = sizeof(InstanceData),
    // This data is per-instance. All four vertices will get the same value. Each instance of drawing the vertices will get a different value.
    // The type, byte offset, and stride (bytes between elements) exactly match the array of `InstanceData` structs we will upload in our draw function.
    .stepMode = WGPUVertexStepMode_Instance,
    .attributeCount = 2,
    .attributes = (WGPUVertexAttribute[]){
            // Translation as a 3D vector.
            {
                .format = WGPUVertexFormat_Float32x3,
                .offset = offsetof(InstanceData, translation),
                .shaderLocation = 2
            },
        // Scale as a 2D vector for non-uniform scaling.
        {
            .format = WGPUVertexFormat_Float32x2,
            .offset = offsetof(InstanceData, scale),
            .shaderLocation = 3
        }
        }
}
}
},

// Interpret our 4 vertices as a triangle strip
.primitive = WGPUPrimitiveState{
    .topology = WGPUPrimitiveTopology_TriangleStrip,
    },

    // No multi-sampling (1 sample per pixel, all bits on).
    .multisample = WGPUMultisampleState{
        .count = 1,
        .mask = ~0u
        },

        // Describe the fragment shader and its output
        .fragment = to_ptr(WGPUFragmentState{
            .module = shader_module,
            .entryPoint = "fragment_shader_main",

            // Our fragment shader outputs a single color value per pixel.
            .targetCount = 1,
            .targets = (WGPUColorTargetState[]){
                {
                    .format = swap_chain_format,
                    // The images we want to draw may have transparency, so let's turn on alpha blending with over compositing (a*foreground + (1-a)*background).
                    // This will blend with whatever has already been drawn.
                    .blend = to_ptr(WGPUBlendState{
                // Over blending for color
                .color = {
                    .operation = WGPUBlendOperation_Add,
                    .srcFactor = WGPUBlendFactor_SrcAlpha,
                    .dstFactor = WGPUBlendFactor_OneMinusSrcAlpha
                    },
                        // Leave destination alpha alone
                        .alpha = {
                            .operation = WGPUBlendOperation_Add,
                            .srcFactor = WGPUBlendFactor_Zero,
                            .dstFactor = WGPUBlendFactor_One
                            }
                        }),
                    .writeMask = WGPUColorWriteMask_All
                }}
            })
        }));

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