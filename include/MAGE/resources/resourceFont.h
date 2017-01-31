#pragma once
#include "resourceFilepath.h"

namespace mage {

	// SFML Font Resource
	class MAGEDLL resourceFont : public resourceFilepath
	{
	public:
		resourceFont();
		resourceFont(std::string filepath);

		bool load();
		void unload();

		sf::Font* get();

	private:
		sf::Font data;
	};

} //namespace mage