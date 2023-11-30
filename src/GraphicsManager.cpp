#include <iostream>
#include <glfw3webgpu.h>
#include <algorithm>

#include "spdlog/spdlog.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"

#include "Engine.h"
#include "GraphicsManager.h"
#include "ResourceManager.h"
#include "ECS.h"

template< typename T > constexpr const T* to_ptr(const T& val) { return &val; }
template< typename T, std::size_t N > constexpr const T* to_ptr(const T(&& arr)[N]) { return arr; }

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
    resources = &(engine->resources);
}

//Creates window
bool GraphicsManager::start() {
    glfwInit();
	// We don't want GLFW to set up a graphics API.
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// Create the window.
    // make screen size 1280x720
    windowWidth = 1280;
    windowHeight = 720;

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
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOther(window, true);
    ImGui_ImplWGPU_Init(wgpuDevice, 2, swap_chain_format);
    
    //gui.start(window, wgpuDevice, swap_chain_format);

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
    @location(3) scale: vec2f,
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
    shader_module = wgpuDeviceCreateShaderModule(wgpuDevice, &shader_desc);

    pipeline = wgpuDeviceCreateRenderPipeline(wgpuDevice, to_ptr(WGPURenderPipelineDescriptor{

        // Describe the data types of our uniforms.
        // This is called the bind group layout. We are using one group (0) and three bindings (0), (1), (2).
        // This exactly matches what's in the shader.
        // We're not actually passing data, we're just declaring matching types.
        .layout = wgpuDeviceCreatePipelineLayout(wgpuDevice, to_ptr(WGPUPipelineLayoutDescriptor{
            .bindGroupLayoutCount = 1, .bindGroupLayouts = to_ptr<WGPUBindGroupLayout>({
                wgpuDeviceCreateBindGroupLayout(
                    wgpuDevice, to_ptr(WGPUBindGroupLayoutDescriptor{.entryCount = 3, .entries = to_ptr<WGPUBindGroupLayoutEntry>({
                        {
                            .binding = 0,
                            .visibility = WGPUShaderStage_Vertex,
                            .buffer = {.type = WGPUBufferBindingType_Uniform}
                        },
                        {
                            .binding = 1,
                            .visibility = WGPUShaderStage_Fragment,
                            .sampler = {.type = WGPUSamplerBindingType_Filtering}
                        },
                        {
                            .binding = 2,
                            .visibility = WGPUShaderStage_Fragment,
                            .texture = {.sampleType = WGPUTextureSampleType_Float, 
                            .viewDimension = WGPUTextureViewDimension_2D},
                        }
                    }) }))
                }) })),
        
        // Describe the vertex shader inputs
        .vertex = {
            .module = shader_module,
            .entryPoint = "vertex_shader_main",
            // Vertex attributes.
            .bufferCount = 2,
            .buffers = to_ptr<WGPUVertexBufferLayout>({
            // We have one buffer with our per-vertex position and UV data. This data never changes.
            // Note how the type, byte offset, and stride (bytes between elements) exactly matches our `vertex_buffer`.
            {
                .arrayStride = 4 * sizeof(float),
                .attributeCount = 2,
                .attributes = to_ptr<WGPUVertexAttribute>({
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
                    })
            },
                // We will use a second buffer with our per-sprite translation and scale. This data will be set in our draw function.
                {
                    .arrayStride = sizeof(InstanceData),
                    // This data is per-instance. All four vertices will get the same value. Each instance of drawing the vertices will get a different value.
                    // The type, byte offset, and stride (bytes between elements) exactly match the array of `InstanceData` structs we will upload in our draw function.
                    .stepMode = WGPUVertexStepMode_Instance,
                    .attributeCount = 2,
                    .attributes = to_ptr<WGPUVertexAttribute>({
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
                        })
                }
                })
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
            .targets = to_ptr<WGPUColorTargetState>({
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
                }})
            })
        }));

	return true;
}

void GraphicsManager::shutdown() {
    nameToImage.clear();
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplWGPU_Shutdown();
    ImGui::DestroyContext();
    //gui.shutdown();
    glfwTerminate();
}

bool GraphicsManager::loadImg(const std::string& name, const std::string& path) {
    std::string imagepath = (*resources).resolvePath(path);

    int width, height, channels;
    unsigned char* data = stbi_load(imagepath.c_str(), &width, &height, &channels, 4);
    if (stbi_failure_reason()) {
        std::cerr << stbi_failure_reason();
    }

    WGPUTextureRef tex = wgpuDeviceCreateTexture(wgpuDevice, to_ptr(WGPUTextureDescriptor{
    .usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst,
    .dimension = WGPUTextureDimension_2D,
    .size = { (uint32_t)width, (uint32_t)height, 1 },
    .format = WGPUTextureFormat_RGBA8Unorm,
    .mipLevelCount = 1,
    .sampleCount = 1
        }));

    nameToImage.insert(std::pair(name, ImageData(width, height, tex)));

    wgpuQueueWriteTexture(
        wgpuQueue,
        to_ptr<WGPUImageCopyTexture>({ .texture = tex }),
        data,
        width * height * 4,
        to_ptr<WGPUTextureDataLayout>({ .bytesPerRow = (uint32_t)(width * 4), .rowsPerImage = (uint32_t)height }),
        to_ptr(WGPUExtent3D{ (uint32_t)width, (uint32_t)height, 1 })
    );

    stbi_image_free(data);

	return true;
}

void GraphicsManager::draw() {
    std::vector<Sprite> sprites;
    globalEngine.ecs.ForEach<Sprite>([&](EntityID e) {
        Sprite& s = globalEngine.ecs.Get<Sprite>(e);
        sprites.push_back(s);
        });
    
    int health;
    globalEngine.ecs.ForEach<Physics, Health>([&](EntityID e) {
        Health& h = globalEngine.ecs.Get<Health>(e);
        health = h.percent;
        });
        
    WGPUBufferRef instance_buffer = wgpuDeviceCreateBuffer(wgpuDevice, to_ptr<WGPUBufferDescriptor>({
    .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex,
    .size = sizeof(InstanceData) * sprites.size()
        }));
    WGPUCommandEncoderRef encoder = wgpuDeviceCreateCommandEncoder(wgpuDevice, nullptr);
    WGPUTextureViewRef current_texture_view = wgpuSwapChainGetCurrentTextureView(swapchain);
    WGPURenderPassEncoderRef render_pass = wgpuCommandEncoderBeginRenderPass(encoder, to_ptr<WGPURenderPassDescriptor>({
    .colorAttachmentCount = 1,
    .colorAttachments = to_ptr<WGPURenderPassColorAttachment>({{
        .view = current_texture_view,
        .loadOp = WGPULoadOp_Clear,
        .storeOp = WGPUStoreOp_Store,
        // Choose the background color.
        .clearValue = WGPUColor{ 1.0, 1.0, 1.0, 1.0 }
        }})
        }));
    wgpuRenderPassEncoderSetPipeline(render_pass, pipeline);
    wgpuRenderPassEncoderSetVertexBuffer(render_pass, 0, vertex_buffer, 0, 4 * 4 * sizeof(float));
    wgpuRenderPassEncoderSetVertexBuffer(render_pass, 1 /* slot */, instance_buffer, 0, sizeof(InstanceData) * sprites.size());
    Uniforms uniforms;
    // Start with an identity matrix.
    uniforms.projection = mat4{ 1 };
    // Scale x and y by 1/100.
    uniforms.projection[0][0] = uniforms.projection[1][1] = 1. / 100.;
    // Scale the long edge by an additional 1/(long/short) = short/long.
    if (windowWidth < windowHeight) {
        uniforms.projection[1][1] *= windowWidth;
        uniforms.projection[1][1] /= windowHeight;
    }
    else {
        uniforms.projection[0][0] *= windowHeight;
        uniforms.projection[0][0] /= windowWidth;
    }
    wgpuQueueWriteBuffer(wgpuQueue, uniform_buffer, 0, &uniforms, sizeof(Uniforms));
    std::sort(sprites.begin(), sprites.end(), [](const Sprite& lhs, const Sprite& rhs) { return lhs.z > rhs.z; });
    //This stops sprites from rendering for some reason, probably don't have time to troubleshoot
    /*
    ImGuiStyle* style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    */
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove 
        | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;
    
    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::SetNextWindowBgAlpha(0.0);
    //ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    /*
    ImGui::Begin("Hello World!");
    ImGui::Text("This is some useful text.");
    */
    ImGui::Begin("", NULL, flags);
    ImGui::Text("Health: %d", health);
    
    ImGui::End();
    
    int i = 0;
    for (Sprite s : sprites) {
        ImageData* id = &nameToImage.at(s.name);
        InstanceData d;
        if (id->width < id->height) {
            d.scale = vec2(double(id->width) / id->height, 1.0);
        }
        else {
            d.scale = vec2(1.0, double(id->height) / id->width);
        }
        d.scale *= s.scale;
        d.translation.x = s.xPos;
        d.translation.y = s.yPos;
        wgpuQueueWriteBuffer(wgpuQueue, instance_buffer, i * sizeof(InstanceData), &d, sizeof(InstanceData));
        if (!id->bind_group) {
            id->bind_group = wgpuDeviceCreateBindGroup(wgpuDevice, to_ptr(WGPUBindGroupDescriptor{
            .layout = ref(wgpuRenderPipelineGetBindGroupLayout(pipeline, 0)),
            .entryCount = 3,
            // The entries `.binding` matches what we wrote in the shader.
            .entries = to_ptr<WGPUBindGroupEntry>({
                {
                    .binding = 0,
                    .buffer = uniform_buffer,
                    .size = sizeof(Uniforms)
                },
                {
                    .binding = 1,
                    .sampler = sampler,
                },
                {
                    .binding = 2,
                    .textureView = wgpuTextureCreateView(id->texture, nullptr)
                }
                })
                }));
        }
        
        wgpuRenderPassEncoderSetBindGroup(render_pass, 0, id->bind_group, 0, nullptr);
        wgpuRenderPassEncoderDraw(render_pass, 4, 1, 0, i);
        i++;
        }
    
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), render_pass);
    
    //gui.draw(encoder, current_texture_view);
    wgpuRenderPassEncoderEnd(render_pass);
    WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, nullptr);
    wgpuQueueSubmit(wgpuQueue, 1, &command);
    wgpuSwapChainPresent(swapchain);
    wgpuCommandBufferRelease(command);
}