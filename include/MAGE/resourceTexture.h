#pragma once
#include "resourceFilepath.h"

namespace mage {

// SFML Texture Resource
class MAGEDLL resourceTexture : public resourceFilepath
{
public:
	resourceTexture();
	resourceTexture(std::string filepath);

	bool load();
	void unload();

	std::shared_ptr<sf::Texture> get();

private:
	std::shared_ptr<sf::Texture> data;
};

} // namespace mage