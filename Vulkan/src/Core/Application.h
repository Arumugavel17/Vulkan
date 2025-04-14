// Application.hpp
#pragma once

#define STR2(x) #x
#define STR(x) STR2(x)

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <optional>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef PLT_WINDOWS
    #define GLFW_EXPOSE_NATIVE_WIN32 
    #define NOMINMAX  // Add this before glfw3native.h to suppress min/max macros
    #include <GLFW/glfw3native.h>
#elif PLT_UNIX
    #define GLFW_EXPOSE_NATIVE_WAYLAND
    #include <GLFW/glfw3native.h>
#endif

#define ENABLE_VALIDATION_LAYERS

namespace CHIKU
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> GraphicsFamily;
        std::optional<uint32_t> PresentFamily;

        bool isComplete()
        {
            return GraphicsFamily.has_value() && PresentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

    namespace VKUtils
    {
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);

        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator);
    }

    class Application
    {
    public:
        Application();
        virtual ~Application();
        void GLFWCleanUp();
        void Run();

    private:
        void VulkanCleanUp();

        void WindowInit();
        void VulkanInit();

        std::vector<const char*> GetRequiredExtensions();
        void CreateInstance();
        void CreateSurface();
        void CreateLogicalDevice();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateGraphicsPipeline();
        void CreateRenderPass();

        bool CheckValidationLayerSupport();


        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
        
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void SetupDebugMessenger();

        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        int RateDeviceSuitability(VkPhysicalDevice device);
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        bool IsDeviceSuitable(VkPhysicalDevice device);

        void PickPhysicalDevice();
        
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkShaderModule CreateShaderModule(const std::vector<char>& code);

    private:
        GLFWwindow* m_Window;
        VkInstance m_VKInstance;    
        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_LogicalDevice;
        VkQueue m_GraphicsQueue;
        VkSurfaceKHR m_Surface;
        VkSwapchainKHR m_SwapChain;
        VkFormat m_SwapChainImageFormat;
        VkExtent2D m_SwapChainExtent;
        VkRenderPass m_RenderPass;
        VkPipelineLayout m_PipelineLayout;
        VkPipeline m_GraphicsPipeline;
        
        std::vector<VkImage> m_SwapChainImages;
        std::vector<VkImageView> m_SwapChainImageViews;

        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const std::vector<const char*> m_ValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        const std::vector<const char*> m_DeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    };
} // namespace CHIKU
