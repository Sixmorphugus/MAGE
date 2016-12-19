#pragma once
#include "SfmlAfx.h"
#include "StdAfx.h"
#include "platformDeduction.h"

namespace mage {

// MATHS
// ---------------------------------------------------------------------
MAGEDLL float lerp(float a, float b, float f); // linear interpolation
MAGEDLL int interpolate(int startValue, int endValue, int stepNumber, int lastStepNumber); // linear interpolation with intigers
MAGEDLL int randomInt(int low, int high); // random intiger
MAGEDLL float randomFloat(float low, float high); // random float
MAGEDLL bool isMultiple(float testNum, float of); // is this number a multiple of "of"?
MAGEDLL float vectorToAngle(sf::Vector2f vec); // get the direction a given vector points.
MAGEDLL float vectorLength(sf::Vector2f vec); // pythagoras.
MAGEDLL float distanceBetweenVectors(sf::Vector2f v1, sf::Vector2f v2); // the distance between two positions as a float.
MAGEDLL float directionToVector(sf::Vector2f v1, sf::Vector2f v2); // angle between two positions as a float.
MAGEDLL sf::Vector2f generateVector(float angle, float speed); // generate a movement vector from an angle and an "intensity" of movement
MAGEDLL float dot(sf::Vector2f v1, sf::Vector2f v2); // dot product.
MAGEDLL float clamp(float in, float min, float max); // clamp a number between two values.
MAGEDLL sf::Vector2f vectorFloor(sf::Vector2f vec); // floor the two values of a vector.
MAGEDLL sf::Vector2f vectorCeil(sf::Vector2f vec); // ceil the two values of a vector.

// RECTANGLES
// ---------------------------------------------------------------------
template<typename T> sf::Rect<T> boxIn(std::vector<sf::Rect<T>> fBoxes)
// "Box In" several rectangles with a box that just contains all of them.
{
	if (!fBoxes.size())
		return sf::Rect<T>();

	// initialize with just the first values
	sf::Vector2<T> tlMost = sf::Vector2<T>(fBoxes[0].left, fBoxes[0].top);
	sf::Vector2<T> brMost = sf::Vector2<T>(fBoxes[0].left + fBoxes[0].width, fBoxes[0].top + fBoxes[0].height);

	for (unsigned int i = 1; i < fBoxes.size(); i++) {
		sf::Vector2<T> tlCurrent = sf::Vector2<T>(fBoxes[i].left, fBoxes[i].top);
		sf::Vector2<T> brCurrent = sf::Vector2<T>(fBoxes[i].left + fBoxes[i].width, fBoxes[i].top + fBoxes[i].height);

		if (tlCurrent.x < tlMost.x) {
			tlMost.x = tlCurrent.x;
		}
		if (tlCurrent.y < tlMost.y) {
			tlMost.y = tlCurrent.y;
		}

		if (brCurrent.x > brMost.x) {
			brMost.x = brCurrent.x;
		}
		if (brCurrent.y > brMost.y) {
			brMost.y = brCurrent.y;
		}
	}

	// make a rectangle from final values
	return sf::Rect<T>(tlMost, tlMost - brMost);
}

// CONDITIONAL
// ---------------------------------------------------------------------
template<typename T> unsigned int highestOf(std::vector<T> inVecs)
// find the highest value in a set of values.
{
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

template<typename T> unsigned int lowestOf(std::vector<T> inVecs)
// find the lowest value in a set of values.
{
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