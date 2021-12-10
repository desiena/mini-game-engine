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


    // ---------------- BEGIN INIT SWAPCHAIN ----------

	vkb::SwapchainBuilder swapchainBuilder{ physicalDevice, device, surface };

	vkb::Swapchain vkbSwapchain = swapchainBuilder
		.use_default_format_selection()
		.set_desired_present_mode(VK_PRESENT_MODE_MAILBOX_KHR)
		.set_desired_extent(1920, 1080)
		.build()
		.value();

	//store swapchain and its related images
	VkSwapchainKHR swapchain = vkbSwapchain.swapchain;
	std::vector<VkImage> swapchainImages = vkbSwapchain.get_images().value();
	std::vector<VkImageView> swapchainImageViews = vkbSwapchain.get_image_views().value();

	VkFormat swapchainImageFormat = vkbSwapchain.image_format;

	//render image
	{
		//depth image size will match the window
		VkExtent3D renderImageExtent = {
			1920,
			1080,
			1
		};
		VkFormat renderFormat = VK_FORMAT_R32G32B32A32_SFLOAT;


		VkImageCreateInfo imageCreateInfo = { };
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.pNext = nullptr;

		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;

		imageCreateInfo.format = renderFormat;
		imageCreateInfo.extent = VkExtent3D{ 1920, 1080, 1 };

		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
			| VK_IMAGE_USAGE_TRANSFER_SRC_BIT
			| VK_IMAGE_USAGE_SAMPLED_BIT;


		//for the render image, we want to allocate it from gpu local memory
		VmaAllocationCreateInfo dimg_allocinfo = {};
		dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		//allocate and create the image
		VkImage rawRenderImage{};
		VmaAllocation rawRenderImageAllocation{};
		VkImageView rawRenderImageView{};
		vmaCreateImage(allocator, &imageCreateInfo, &dimg_allocinfo, &rawRenderImage, &rawRenderImageAllocation, nullptr);



		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.pNext = nullptr;

		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.image = rawRenderImage;
		imageViewCreateInfo.format = renderFormat;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		vkCreateImageView(device, &imageViewCreateInfo, nullptr, &rawRenderImageView);
	}



	VkSamplerCreateInfo createInfo = {};

	auto reductionMode = VK_SAMPLER_REDUCTION_MODE_MIN;

	createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	createInfo.magFilter = VK_FILTER_LINEAR;
	createInfo.minFilter = VK_FILTER_LINEAR;
	createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	createInfo.minLod = 0;
	createInfo.maxLod = 16.f;

	VkSamplerReductionModeCreateInfoEXT createInfoReduction = { VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO_EXT };

	if (reductionMode != VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT)
	{
		createInfoReduction.reductionMode = reductionMode;

		createInfo.pNext = &createInfoReduction;
	}


	VK_CHECK(vkCreateSampler(device, &createInfo, 0, &_depthSampler));

	VkSamplerCreateInfo samplerInfo = vkinit::sampler_create_info(VK_FILTER_LINEAR);
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	vkCreateSampler(_device, &samplerInfo, nullptr, &_smoothSampler);

	VkSamplerCreateInfo shadsamplerInfo = vkinit::sampler_create_info(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
	shadsamplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	shadsamplerInfo.compareEnable = true;
	shadsamplerInfo.compareOp = VK_COMPARE_OP_LESS;
	vkCreateSampler(_device, &shadsamplerInfo, nullptr, &_shadowSampler);


	//add to deletion queues
	_mainDeletionQueue.push_function([=]() {
		vkDestroyImageView(_device, _depthImage._defaultView, nullptr);
		vmaDestroyImage(_allocator, _depthImage._image, _depthImage._allocation);
		});
    // ----------------- END INIT SWAPCHAIN ------------

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

	vkDestroySwapchainKHR(_device, _swapchain, nullptr);
}
