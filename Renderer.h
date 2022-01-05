#pragma once

#include <vulkan/vulkan.h>
#include <vector>



struct Renderable
{

};

class Renderer
{
private:
	VkExtent2D extent;
	std::vector<Renderable> renderables;
public:
	void init();
};

