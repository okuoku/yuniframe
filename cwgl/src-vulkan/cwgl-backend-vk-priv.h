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
    uint64_t ident;
    VkImage image;
    VkImageView image_view;
    VkDeviceMemory device_memory;
    VkFormat format;
    uint32_t width; /* Cache */
    uint32_t height; /* Cache */
};

struct cwgl_backend_Framebuffer_s {
    int allocated;
    uint64_t ident;
    uint64_t texture_ident;
    uint64_t renderbuffer_ident;
    VkRenderPass renderpass;
    VkFramebuffer framebuffer;
    uint32_t width; /* Cache */
    uint32_t height; /* Cache */
};

struct cwgl_backend_pipeline_identity_s {
    /* Cache data for global context */
    uint64_t framebuffer_ident;
    uint64_t program_ident;
    float blend_color[4]; // FIXME: Dynamic?
    VkPipelineRasterizationStateCreateInfo rsi;
    VkPipelineMultisampleStateCreateInfo msi;
    VkPipelineDepthStencilStateCreateInfo dsi;
    VkPipelineInputAssemblyStateCreateInfo iai;
    VkPipelineColorBlendAttachmentState cbas_color; /* for color attachment */
    /* Cache data for vertex attr fetching */
    int bind_count;
    VkVertexInputAttributeDescription attrs[CWGL_MAX_VAO_SIZE];
    VkVertexInputBindingDescription binds[CWGL_MAX_VAO_SIZE];
};

typedef struct cwgl_backend_pipeline_identity_s cwgl_backend_pipeline_identity_t;

struct cwgl_backend_pipeline_s {
    int allocated;
    VkPipeline pipeline;
    cwgl_backend_pipeline_identity_t cache;
};

typedef struct cwgl_backend_pipeline_s cwgl_backend_pipeline_t;

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
    uint64_t framebuffer_ident;
    VkSemaphore sem_fb;
    int need_wait_fb;
    // FIXME: Implement backbuffer (if required)
    cwgl_backend_Framebuffer_t default_fb;
    cwgl_backend_Renderbuffer_t depth;
    /* Vulkan Queue status */
    int queue_active;
    int queue_has_command;
    /* Object ident */
    uint64_t ident_age;
    /* SHXM */
    shxm_ctx_t* shxm_ctx;
    /* Pipeline cache */
    cwgl_backend_pipeline_t pipelines[1];
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
    uint64_t ident;
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
    cwgl_backend_Buffer_t attribute_registers;
    /* SHXM */
    shxm_program_t* program;
};

struct cwgl_backend_Texture_s {
    int allocated;
    uint64_t ident;
    /* Image data */
    VkImage image;
    VkDeviceMemory device_memory;
    VkImageView view;
    /* Combined sampler (Updated during render kick) */
    int sampler_allocated;
    VkSampler sampler;
    cwgl_texture_state_t cached_tracker_state;
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

#define CWGL_VK_INVALID_IDENT 0
uint64_t cwgl_vkpriv_newident(cwgl_ctx_t* ctx);

#endif
