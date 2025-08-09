#include "Device.h"

using namespace Audio;

Device* Device::open(const char* deviceName) {
	Device* audioDevice = new Device();
	audioDevice->window_handle = alcOpenDevice(deviceName);
	audioDevice->context = alcCreateContext(audioDevice->window_handle, NULL);
	
	if (!audioDevice->window_handle || !audioDevice->context) {
		delete audioDevice;
		return NULL;
	}

	alcMakeContextCurrent(audioDevice->context);
	return audioDevice;
}

void Device::close(Device* device) {
	alcMakeContextCurrent(NULL);
	alcDestroyContext(device->context);
	alcCloseDevice(device->window_handle);
}

std::vector<std::string> Device::readDeviceList(const char* list) {
	std::vector<std::string> names;
	
	unsigned int cur = 0;
	unsigned int offset = 0;
	while (1) {
		if (list[cur + offset] == '\0') {
			names.push_back(&list[cur]);
			cur += offset + 1;
			offset = 0;

			// Double null -> break
			if (list[cur + offset] == '\0')
				break;
		}
		else
			offset++;
	}

	return names;
}

std::vector<std::string> Device::getDevices() {
	const char* devices = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
	std::vector<std::string> device_names = readDeviceList(devices);
	return device_names;
}

std::vector<std::string> Device::getCaptureDevices() {
	const char* devices = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
	std::vector<std::string> device_names = readDeviceList(devices);
	return device_names;
}