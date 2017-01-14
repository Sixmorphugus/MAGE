#pragma once

// mage::clock
// -------------
// Wrapper for sf::Clock.
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "time.h"

namespace mage {
	class MAGEDLL clock
	{
		clock();

		time getElapsedTime();
		time restart();

	private:
		sf::Clock m_clock;
	};
}