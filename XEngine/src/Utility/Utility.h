#pragma once

#include <vulkan/vk_enum_string_helper.h>


namespace XEngine {

#define CHECK_VK_RES(res) \
	if(res != 0) {\
		XEngine_CRITICAL(fmt::runtime("Error during validation via CHECK_VK_RES: {0}"), string_VkResult(res)); \
		abort(); }\

}