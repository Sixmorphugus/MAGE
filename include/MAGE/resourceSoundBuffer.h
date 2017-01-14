#pragma once
#include "resourceFilepath.h"

namespace mage {
class time;

// SFML Sound Resource
class MAGEDLL resourceSoundBuffer : public resourceFilepath
{
public:
	resourceSoundBuffer();
	resourceSoundBuffer(std::string filepath);

	bool load();
	void unload();

	sf::SoundBuffer* get();

	unsigned int getChannelCount();
	time getDuration();
	unsigned int getSampleRate();

private:
	sf::SoundBuffer data;
};

} // namespace mage