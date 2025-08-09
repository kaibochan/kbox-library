#pragma once

#include <AL/alc.h>

#include <vector>
#include <string>

namespace Audio {
	class Device;
}

class Audio::Device {
private:
	ALCdevice* window_handle;
	ALCcontext* context;

	static std::vector<std::string> readDeviceList(const char* list);

public:
	static Device* open(const char* deviceName);
	
	static std::vector<std::string> getDevices();
	static std::vector<std::string> getCaptureDevices();

	static void close(Device* device);
};