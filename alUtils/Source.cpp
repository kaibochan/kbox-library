#include "Source.h"

#include <AL/al.h>

using namespace Audio;

Source::Source() : ASO(0) {}

unsigned int Source::generate() {
	alGenSources(1, &ASO);
	return ASO;
}

Source::~Source() {
	alDeleteSources(1, &ASO);
}

unsigned int Source::getID() {
	return ASO;
}

void Source::addBuffer(Buffer* buffer) {
	ABOs.push_back(buffer->getID());
}

void Source::playAll() {
	if (ABOs.empty())
		return;

	alSourceQueueBuffers(ASO, ABOs.size(), &ABOs[0]);
	alSourcePlay(ASO);
}

void Source::removeProcessed() {
	int buffersProcessed;
	alGetSourcei(ASO, AL_BUFFERS_PROCESSED, &buffersProcessed);
	if (buffersProcessed == 0)
		return;
	
	alSourceUnqueueBuffers(ASO, buffersProcessed, &ABOs[0]);
}