#include "Buffer.h"

#include <sndfile/sndfile.h>

#include <iostream>

using namespace Audio;

PCM* PCM::constrain_bit_depth(PCM* pcm_prev) {

	// Shallow copy previous PCM attributes
	PCM* pcm_new = new PCM(*pcm_prev);
	// Remove duplicate ownership of data before continuing
	pcm_new->data = NULL;

	if (pcm_prev->sampleWidth > 0x0000 && pcm_prev->sampleWidth <= SF_FORMAT_PCM_16)
		return pcm_prev;

	pcm_new->sampleWidth = SF_FORMAT_PCM_16;
	pcm_new->size = pcm_new->sampleWidth * pcm_new->channelCount * pcm_new->sampleCount;
	pcm_new->data = new unsigned char[pcm_new->size];

	for (int sample = 0; sample < pcm_prev->size / (pcm_prev->channelCount * pcm_prev->sampleWidth); sample++) {
		
		for (int channel = 0; channel < pcm_prev->channelCount; channel++) {
			unsigned long long buffer = 0;
			int current_byte;

			for (int byte = 0; byte < pcm_prev->sampleWidth; byte++) {
				current_byte = sample * pcm_prev->sampleWidth * pcm_prev->channelCount + channel * pcm_prev->sampleWidth + byte;
				buffer |= pcm_prev->data[current_byte];

				buffer = buffer << 8;
			}


			for (int byte = 0; byte < pcm_new->sampleWidth; byte++) {
				current_byte = sample * pcm_new->sampleWidth * pcm_prev->channelCount + channel * pcm_new->sampleWidth + byte;
				pcm_new->data[current_byte] = static_cast<unsigned char>(buffer);
				
				buffer = buffer >> 8;
			}
		}
	}

	return pcm_new;
}

PCM::Format PCM::get_format(PCM* pcm) {
	if (pcm->channelCount > 2)
		return FORMAT_INVALID;

	if (pcm->sampleWidth > 2)
		return FORMAT_OVER_WIDTH_MAX;

	if (pcm->channelCount == 1) {
		if (pcm->sampleWidth == 1)
			return FORMAT_MONO8;
		else
			return FORMAT_MONO16;
	}
	else {
		if (pcm->sampleWidth == 1)
			return FORMAT_STEREO8;
		else
			return FORMAT_STEREO16;
	}

	return FORMAT_INVALID;
}

PCM* PCM::from_file(const char* file_path) {
	SF_INFO info;
	SNDFILE* file_handle = sf_open(file_path, SFM_READ, &info);

	if (!file_handle) {
		std::cout << "Could not open file: " << file_path << std::endl;
		return NULL;
	}
	
	PCM* pcm = new PCM();

	int subformat = SF_FORMAT_SUBMASK & info.format;
	if (subformat > 0x0000 && subformat <= SF_FORMAT_PCM_32)
		pcm->sampleWidth = subformat;

	pcm->sampleCount = info.frames;
	pcm->sampleRate = info.samplerate;
	pcm->channelCount = info.channels;
	
	pcm->size = pcm->channelCount * (pcm->sampleCount * pcm->sampleWidth);

	int data_start = sf_seek(file_handle, 0, SF_SEEK_SET);
	pcm->data = new unsigned char[pcm->size];
	sf_read_raw(file_handle, pcm->data, pcm->size);
	sf_close(file_handle);

	pcm = constrain_bit_depth(pcm);

	return pcm;
}

Buffer::Buffer() : ABO(0) {}
unsigned int Buffer::generate() {
	alGenBuffers(1, &ABO);
	return ABO;
}

// TODO: Implement restrictions on signed 8 bit representations
// OpenAL only supports unsigned 8 bit
ALenum Buffer::load(PCM* pcm) {
	PCM::Format format = PCM::get_format(pcm);
	if (format == PCM::FORMAT_INVALID || format == PCM::FORMAT_OVER_WIDTH_MAX)
		return -1;

	alGetError(); // Clear error flags
	alBufferData(ABO, format, pcm->data, pcm->size, pcm->sampleRate);
	
	ALenum error = alGetError();
	return error;
}

unsigned int Buffer::getID() {
	return ABO;
}

Buffer::~Buffer() {
	alDeleteBuffers(1, &ABO);
}