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

// OTHER
// ---------------------------------------------------------------------
template<typename To, typename From> std::vector<To> dynamicVectorCast(From& from)
{
	std::vector<To> output;

	for (unsigned int i = 0; i < from.size(); i++) {
		output.push_back(dynamic_cast<To>(from[i]));
	}

	return output;
}

template<typename To, typename From> std::vector<std::shared_ptr<To>> dynamicShPVectorCast(From& from)
{
	std::vector<std::shared_ptr<To>> output;

	for (unsigned int i = 0; i < from.size(); i++) {
		output.push_back(std::dynamic_pointer_cast<To>(from[i]));
	}

	return output;
}

} // namespace mage