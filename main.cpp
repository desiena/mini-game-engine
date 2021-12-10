#include "VkBootstrap.h"
#include "vk_mem_alloc.h"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

int main()
{
    vkb::InstanceBuilder instance_builder;
    auto instance_builder_return = instance_builder
        .request_validation_layers()
        .use_default_debug_messenger()
        .build();

    if (!instance_builder_return) {
        std::cerr << "Failed to create Vulkan instance. Error: " << instance_builder_return.error().message() << "\n";
        return -1;
    }
    std::cout << "Vulkan instance initialized.";

    vkb::Instance vkb_instance = instance_builder_return.value();

    VkInstance instance = vkb_instance.instance;

    VkSurfaceKHR surface = nullptr;




    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    GLFWwindow* window;
    window = glfwCreateWindow(1920, 1080, "Vulkan", nullptr, nullptr);

    VkResult err = glfwCreateWindowSurface(instance, window, NULL, &surface);
    if (err != VK_SUCCESS) {
        std::cerr << "Failed to create Window." << std::endl;
        return -1;
    }
    std::cout << "GLFW window initialized.";



    vkb::PhysicalDeviceSelector selector { vkb_instance };
    VkPhysicalDeviceFeatures feats{};
    feats.pipelineStatisticsQuery = true;
    feats.multiDrawIndirect = true;
    feats.drawIndirectFirstInstance = true;
    feats.samplerAnisotropy = true;
    selector.set_required_features(feats);

    auto selectorReturn = selector
        .set_minimum_version(1, 1)
        .set_surface(surface)
        .add_required_extension(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
        .select();
    if (!selectorReturn)
    {
        std::cerr << "Failed to select GPU." << std::endl;
        return -1;
    }
    std::cout << "GPU selected.";
    vkb::PhysicalDevice vkb_physicalDevice = selectorReturn.value();





    vkb::DeviceBuilder deviceBuilder{ vkb_physicalDevice };
    auto deviceReturn = deviceBuilder.build();
    if (!deviceReturn)
    {
        std::cerr << "Failed to build physical device.";
        return -1;
    }
    std::cout << "Built physical device.";

    vkb::Device vkbDevice = deviceReturn.value();

    // Get the VkDevice handle used in the rest of a vulkan application
    VkDevice device = vkbDevice.device;
    VkPhysicalDevice physicalDevice = vkb_physicalDevice.physical_device;

    // use vkbootstrap to get a Graphics queue
    VkQueue graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();

    uint32_t graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();





    //initialize the memory allocator
    VmaAllocator allocator;
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = physicalDevice;
    allocatorInfo.device = device;
    allocatorInfo.instance = instance;
    vmaCreateAllocator(&allocatorInfo, &allocator);

    VkPhysicalDeviceProperties physicalDeviceProperties{};
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

}
