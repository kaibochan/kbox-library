#include "SoundManager.h"

// Initialize sounds dictionary
std::map<std::string, Audio::Buffer*> SoundManager::sounds;

void SoundManager::registerSound(std::string name, Audio::PCM* pcm) {
	Audio::Buffer* buffer = new Audio::Buffer();
	buffer->generate();
	buffer->load(pcm);

	sounds.insert({ name, buffer });
}

std::vector<std::string> SoundManager::getRegistered() {
	std::vector<std::string> names;
	for (auto entry : sounds)
		names.push_back(entry.first);

	return names;
}

Audio::Buffer* SoundManager::getBuffer(std::string name) {
	return sounds[name];
}

void SoundManager::deleteSound(std::string name) {
	sounds.erase(name);
}

void SoundManager::deleteSounds() {
	for (auto it = sounds.begin(); it != sounds.end();) {
		delete it->second;
		it = sounds.erase(it);
	}
}