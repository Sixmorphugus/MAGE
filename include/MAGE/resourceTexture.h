#pragma once
#include "resourceFilepath.h"

#include "point.h"

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

	point2U getSize();

private:
	std::shared_ptr<sf::Texture> m_data;
};

} // namespace mage