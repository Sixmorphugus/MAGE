#pragma once

// mage::timer
// -------------
// Wrapper for sf::Clock.
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "interval.h"

namespace mage {
	class MAGEDLL timer
	{
	public:
		timer();

		interval getElapsedTime();
		interval restart();

	private:
		sf::Clock m_clock;
	};
}