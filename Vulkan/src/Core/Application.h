#pragma once
#include "GLFW/glfw3.h"

#include <vector>
#include <iostream>
#include <vulkan/vulkan.h>
#include <algorithm>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace CHIKU
{
    static VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void DestroyDebugUtilsMessengerEXT(
        VkInstance instance, 
        VkDebugUtilsMessengerEXT debugMessenger, 
        const VkAllocationCallbacks* pAllocator) 
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) 
        {
            func(instance, debugMessenger, pAllocator);
        }
    }

	class Application
	{
	public:
		Application()
		{
			glfwInit();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

			CreateInstance();
            SetupDebugMessenger();
		}

		void Run()
		{
			while (!glfwWindowShouldClose(m_Window)) {
				glfwPollEvents();
			}
		}

        void CreateInstance()
        {
            if (enableValidationLayers && !CheckValidationLayerSupport()) {
                throw std::runtime_error("validation layers requested, but not available!");
            }

            // Step 1: Fill out application info
            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "NOGAME";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "CHIKU";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            std::vector<const char* > extensions = GetRequiredExtensions();

            std::cout << "Required extensions:\n";
            for (uint32_t i = 0; i < extensions.size(); ++i)
            {
                std::cout << "\t" << extensions[i] << "\n";
            }

            // Step 3: Get available Vulkan extensions
            uint32_t extensionCount = 0;
            if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to get extension count");
            }

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data()) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to enumerate extensions");
            }

            std::cout << "Available extensions:\n";
            for (const auto& ext : availableExtensions)
            {
                std::cout << "\t" << ext.extensionName << "\n";
            }

            // Step 4: Validate GLFW-required extensions
            bool allSupported = true;
            for (uint32_t i = 0; i < extensions.size(); ++i)
            {
                const char* requiredExt = extensions[i];
                bool found = std::any_of(
                    availableExtensions.begin(),
                    availableExtensions.end(),
                    [requiredExt](const VkExtensionProperties& ext) {
                        return strcmp(requiredExt, ext.extensionName) == 0;
                    });

                if (found) {
                    std::cout << "Found: " << requiredExt << "\n";
                }
                else {
                    std::cout << "Not Found: " << requiredExt << "\n";
                    allSupported = false;
                }
            }

            if (!allSupported)
            {
                throw std::runtime_error("Some required extensions are not supported!");
            }

            // Step 5: Fill out instance creation struct
            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            createInfo.enabledExtensionCount = extensions.size();
            createInfo.ppEnabledExtensionNames = extensions.data();
            createInfo.enabledLayerCount = 0;

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            if (enableValidationLayers) 
            {
                createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
                createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

                PopulateDebugMessengerCreateInfo(debugCreateInfo);
                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
            }
            else 
            {
                createInfo.enabledLayerCount = 0;
            }

            // Step 6: Create Vulkan instance
            VkResult result = vkCreateInstance(&createInfo, nullptr, &m_VKInstance);
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create Vulkan instance!");
            }

            std::cout << "Vulkan instance created successfully!\n";
        }

        bool CheckValidationLayerSupport() 
        {
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            for (const char* layerName : m_ValidationLayers) 
            {
                bool layerFound = false;

                for (const auto& layerProperties : availableLayers) 
                {
                    if (strcmp(layerName, layerProperties.layerName) == 0) 
                    {
                        layerFound = true;
                        break;
                    }
                }
                if (!layerFound) 
                {
                    return false;
                }
            }

            return true;
        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) 
        {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
            return VK_FALSE;
        }

        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
            createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = DebugCallback;
        }

        std::vector<const char*> GetRequiredExtensions() {
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            if (enableValidationLayers) {
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensions;
        }

        void SetupDebugMessenger() 
        {
            if (!enableValidationLayers)
            {
                return;
            }

            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            PopulateDebugMessengerCreateInfo(createInfo);

            if (CreateDebugUtilsMessengerEXT(m_VKInstance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) 
            {
                throw std::runtime_error("failed to set up debug messenger!");
            }

        }

		virtual ~Application()
		{
            if (enableValidationLayers) 
            {
                DestroyDebugUtilsMessengerEXT(m_VKInstance, m_DebugMessenger, nullptr);
            }

			vkDestroyInstance(m_VKInstance, nullptr);
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}

	private:
		GLFWwindow* m_Window;
		VkInstance m_VKInstance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const std::vector<const char*> m_ValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
	};
}