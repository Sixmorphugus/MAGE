#pragma once
#include "resourceFilePath.h"

namespace mage {

	// SFML Font Resource
	class MAGEDLL resourceFont : public resourceFilepath
	{
	public:
		resourceFont();
		resourceFont(std::string filepath);

		bool load();
		void unload();

		std::shared_ptr<sf::Font> get();

	private:
		std::shared_ptr<sf::Font> data;
	};

} //namespace mage