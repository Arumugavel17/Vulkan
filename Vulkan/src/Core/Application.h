// Application.hpp
#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <optional>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

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
        void Run();

    private:
        std::vector<const char*> GetRequiredExtensions();
        void CreateInstance();
        void CreateSurface();
        void CreateLogicalDevice();
        bool CheckValidationLayerSupport();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void SetupDebugMessenger();

        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        int RateDeviceSuitability(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);

        void PickPhysicalDevice();

    private:
        GLFWwindow* m_Window;
        VkInstance m_VKInstance;    
        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_LogicalDevice;
        VkQueue m_GraphicsQueue;
        VkSurfaceKHR m_Surface;

        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const std::vector<const char*> m_ValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
    };
} // namespace CHIKU
