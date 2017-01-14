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

	sf::Texture* get();

	point2U getSize();
	bool getIsRepeated();
	bool getIsSmooth();

	void setRepeated(bool repeated);
	void setSmooth(bool smooth);

private:
	sf::Texture m_data;
};

} // namespace mage