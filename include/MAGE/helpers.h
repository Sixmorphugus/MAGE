#pragma once
#include "StdAfx.h"
#include "SfmlAfx.h"

namespace mage {

// Helpers! Making life bearable.

// INTERPOLATION
// ---------------------------------------------------------------------
MAGEDLL sf::Color interpolateColors(sf::Color a, sf::Color b, int stepNumber, int lastStepNumber);

// ENGINE
// ---------------------------------------------------------------------
class basic;

MAGEDLL float distanceBetweenObjects(basic* i, basic* j);
MAGEDLL float randomPitch();

} // for once you can see namespace mage from here