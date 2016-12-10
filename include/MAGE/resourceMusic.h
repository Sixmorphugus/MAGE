#pragma once
#include "resourceFilepath.h"

namespace mage {

// SFML Music Resource
// The only one that can unload when unused
class MAGEDLL resourceMusic : public resourceFilepath
{
public:
	resourceMusic();
	resourceMusic(std::string filepath);

	bool load();
	void unload();

	bool isNeeded();

	std::shared_ptr<sf::Music> get();

private:
	std::shared_ptr<sf::Music> data;
};

} // namespace mage