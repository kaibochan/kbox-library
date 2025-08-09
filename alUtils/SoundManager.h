#pragma once

#include "Buffer.h"

#include <vector>
#include <map>
#include <string>

class SoundManager {
private:
	static std::map<std::string, Audio::Buffer*> sounds;

public:
	static std::vector<std::string> getRegistered();
	static Audio::Buffer* getBuffer(std::string name);

	static void registerSound(std::string name, Audio::PCM* pcm);
	static void deleteSound(std::string name);
	static void deleteSounds();
};