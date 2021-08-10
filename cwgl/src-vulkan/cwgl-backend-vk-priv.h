#ifndef __YUNI_CWGL_BACKEND_VK_PRIV_H
#define __YUNI_CWGL_BACKEND_VK_PRIV_H

#include "cwgl-tracker-priv.h"
#include "cwgl-backend-states.h"

#include "shxm.h"

#ifdef CWGL_EXPERIMENTAL_USE_VOLK
#include "volk.h"
#else
#error NEED CWGL_EXPERIMENTAL_USE_VOLK
#endif

#define CWGL_FRAMEBUFFER_COUNT 2

struct cwgl_backend_Renderbuffer_s {
    int allocated;
    VkImage image;
    VkImageView image_view;
    VkDeviceMemory device_memory;
    VkFormat format;
};

struct cwgl_backend_Framebuffer_s {
    int dummy;
};

typedef struct cwgl_backend_fb_state_s cwgl_backend_fb_state_t;

struct cwgl_backend_ctx_s {
    /* Vulkan */
    VkInstance instance;
    VkDevice device;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    VkQueue queue;
    VkPhysicalDeviceMemoryProperties memory_properties;
    uint32_t queue_family_index;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    VkPipelineCache pipeline_cache;
    /* Vulkan framebuffers */
    uint32_t current_image_index;
    VkImage cb[CWGL_FRAMEBUFFER_COUNT];
    VkImageView cb_view[CWGL_FRAMEBUFFER_COUNT];
    VkFormat cb_format;
    int framebuffer_allocated;
    VkSemaphore sem_fb;
    int need_wait_fb;
    // FIXME: Implement backbuffer (if required)
    cwgl_backend_Renderbuffer_t depth;
    VkImageView depth_view;
    /* Vulkan Queue status */
    int queue_active;
    int queue_has_command;
    /* SHXM */
    shxm_ctx_t* shxm_ctx;
};

typedef struct buffer_patch_request_s buffer_patch_request_t;

struct cwgl_backend_Buffer_s {
    int allocated;
    VkBuffer buffer;
    VkDeviceMemory device_memory;
};

struct cwgl_backend_Shader_s {
    /* SHXM */
    shxm_shader_t* shader;
};

struct cwgl_backend_Program_s {
    /* Vulkan */
    int allocated;
    VkShaderModule vertex_shader;
    VkShaderModule pixel_shader;
    /* Vulkan: Uniforms */
    VkDescriptorPool desc_pool;
    VkDescriptorSetLayout desc_set_layout;
    VkDescriptorSet desc_set;
    VkPipelineLayout pipeline_layout;
    cwgl_backend_Buffer_t uniform_buffer;
    /* Vulkan: Vertex attributes */
    int input_count; /* Cache */
    int bind_count;
    cwgl_backend_Buffer_t attribute_registers;
    VkVertexInputAttributeDescription attrs[CWGL_MAX_VAO_SIZE];
    VkVertexInputBindingDescription binds[CWGL_MAX_VAO_SIZE];
    VkBuffer bind_buffers[CWGL_MAX_VAO_SIZE]; /* Cache */
    VkDeviceSize bind_offsets[CWGL_MAX_VAO_SIZE]; /* Cache */
    /* SHXM */
    shxm_program_t* program;
};

struct cwgl_backend_Texture_s {
    int allocated;
    /* Image data */
    VkImage image;
    VkDeviceMemory device_memory;
    VkImageView view;
    /* Combined sampler (Updated during render kick) */
    int sampler_allocated;
    VkSampler sampler;
};

void cwgl_vkpriv_prepare_fb(cwgl_ctx_t* ctx);
void cwgl_vkpriv_graphics_submit(cwgl_ctx_t* ctx);
void cwgl_vkpriv_graphics_wait(cwgl_ctx_t* ctx);
void cwgl_vkpriv_destroy_texture(cwgl_ctx_t* ctx,
                                 cwgl_backend_Texture_t* texture_backend);
void cwgl_vkpriv_destroy_buffer(cwgl_ctx_t* ctx,
                                cwgl_backend_Buffer_t* buffer_backend);
void cwgl_vkpriv_destroy_program(cwgl_ctx_t* ctx,
                                 cwgl_backend_Program_t* program_backend);
int32_t cwgl_vkpriv_select_memory_type(cwgl_ctx_t* ctx, uint32_t requirement,
                                       VkMemoryPropertyFlags request);

#endif
