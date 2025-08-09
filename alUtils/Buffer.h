#pragma once

#include <AL/al.h>

namespace Audio {
	class PCM;
	class Buffer;
}

class Audio::PCM {
public:
	enum Format {
		FORMAT_INVALID = 0xFFF0,
		FORMAT_OVER_WIDTH_MAX = 0xFFF1,

		FORMAT_MONO8 = AL_FORMAT_MONO8,
		FORMAT_MONO16 = AL_FORMAT_MONO16,
		FORMAT_STEREO8 = AL_FORMAT_STEREO8,
		FORMAT_STEREO16 = AL_FORMAT_STEREO16
	};

private:
	friend class Buffer;

	unsigned char* data;
	unsigned int size;
	unsigned int channelCount;
	unsigned int sampleWidth;
	unsigned int sampleRate;
	unsigned int sampleCount;

public:
	static Format get_format(PCM* pcm);
	static PCM* constrain_bit_depth(PCM* pcm);
	static PCM* from_file(const char* file_path);

};

// Audio Buffer class
class Audio::Buffer {
private:
	// Audio Buffer Object ID
	unsigned int ABO;

public:
	Buffer();
	unsigned int generate();

	unsigned int getID();

	ALenum load(PCM* pcm);

	~Buffer();
};