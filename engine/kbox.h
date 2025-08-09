#pragma once

namespace kbox {
	class Engine;
	class Context;
	class Window;
	class Scene;

	enum Error {
		NO_ERROR						= 0x0000,
		GLFW_INIT_FAIL					= 0x0001,

		WINDOW_CREATION_FAILED			= 0x0002,
		GLAD_LOADER_FAILED				= 0x0004,

		AUDIO_DEVICE_FAILED				= 0x0008,
		AUDIO_CONTEXT_FAILED			= 0x0010,

		CONTEXT_USE_FAILED_NO_WINDOW	= 0x0020,
		CONTEXT_USE_FAILED_NO_AUDIO		= 0x0040,
	};
}