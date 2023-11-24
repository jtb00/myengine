#include "GUIManager.h"

template< typename T > constexpr const T* to_ptr(const T& val) { return &val; }
template< typename T, std::size_t N > constexpr const T* to_ptr(const T(&& arr)[N]) { return arr; }

GUIManager::GUIManager() {

}

bool GUIManager::start(GLFWwindow *window, WGPUDeviceRef wgpuDevice, WGPUTextureFormat swap_chain_format) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOther(window, true);
    ImGui_ImplWGPU_Init(wgpuDevice, 2, swap_chain_format);
    return true;
}

void GUIManager::shutdown() {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplWGPU_Shutdown();
    ImGui::DestroyContext();
}

void GUIManager::draw(WGPUCommandEncoderRef encoder, WGPUTextureViewRef current_texture_view) {
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

    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::End();
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), render_pass);
}