#pragma once
#include "resourceFilepath.h"

namespace mage {

// SFML Sound Resource
class MAGEDLL resourceSoundBuffer : public resourceFilepath
{
public:
	resourceSoundBuffer();
	resourceSoundBuffer(std::string filepath);

	bool load();
	void unload();

	std::shared_ptr<sf::SoundBuffer> get();

private:
	std::shared_ptr<sf::SoundBuffer> data;
};

} // namespace mage