#pragma once
#include "SfmlAfx.h"
#include "StdAfx.h"
#include "platformDeduction.h"

namespace mage {

// MATHS
// ---------------------------------------------------------------------
MAGEDLL float lerp(float a, float b, float f);
MAGEDLL int interpolate(int startValue, int endValue, int stepNumber, int lastStepNumber);
MAGEDLL int randomInt(int low, int high);
MAGEDLL float randomFloat(float low, float high);
MAGEDLL bool isMultiple(float testNum, float of);
MAGEDLL float vectorToAngle(sf::Vector2f vec);
MAGEDLL float vectorLength(sf::Vector2f vec);
MAGEDLL float distanceBetweenVectors(sf::Vector2f v1, sf::Vector2f v2);
MAGEDLL float directionToVector(sf::Vector2f v1, sf::Vector2f v2);
MAGEDLL sf::Vector2f generateVector(float angle, float speed);
MAGEDLL float dot(sf::Vector2f v1, sf::Vector2f v2);
MAGEDLL float clamp(float in, float min, float max);
MAGEDLL sf::Vector2f vectorFloor(sf::Vector2f vec);
MAGEDLL sf::Vector2f vectorCeil(sf::Vector2f vec);

// CONDITIONAL
// ---------------------------------------------------------------------
template<typename T> unsigned int highestOf(std::vector<T> inVecs) {
	unsigned int match = 0;
	T held = inVecs[0];

	for (unsigned int i = 1; i < inVecs.size(); i++) {
		if (inVecs[i] > held) {
			match = i;
			held = inVecs[i];
		}
	}

	return match;
}

template<typename T> unsigned int lowestOf(std::vector<T> inVecs) {
	unsigned int match = 0;
	T held = inVecs[0];

	for (unsigned int i = 1; i < inVecs.size(); i++) {
		if (inVecs[i] < held) {
			match = i;
			held = inVecs[i];
		}
	}

	return match;
}

} // namespace mage