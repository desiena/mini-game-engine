

#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL_events.h>
#include <SDL.h>
#include <SDL_vulkan.h>

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
#include "VkBootstrap.h"

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



	// We initialize SDL and create a window with it. 
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WindowFlags window_flags = (SDL_WINDOW_VULKAN);

	SDL_Window* window = SDL_CreateWindow(
		"",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1920,
		1080,
		window_flags
	);

	SDL_bool err = SDL_Vulkan_CreateSurface(window, vkb_instance.instance, &surface);
	if (!err) {
        std::cerr << "Failed to create Window." << std::endl;
        return -1;
    }
    std::cout << "SDL2 window initialized.";



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





	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.pNext = nullptr;

	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;


	VkSampler smoothSampler;
	vkCreateSampler(device, &samplerInfo, nullptr, &smoothSampler);


	bool bQuit = false;
	while (!bQuit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				bQuit = true;
			}
		}
	}

	vkDestroySwapchainKHR(device, swapchain, nullptr);
}
