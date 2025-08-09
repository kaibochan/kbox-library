#pragma once

#include "Buffer.h"

#include <vector>


namespace Audio {
	class Source;
}

class Audio::Source {
private:
	// Audio Source Object ID
	unsigned int ASO;

	std::vector<unsigned int> ABOs;

public:
	Source();
	~Source();

	unsigned int generate();
	unsigned int getID();

	void addBuffer(Buffer* buffer);
	void playAll();
	void removeProcessed();
};