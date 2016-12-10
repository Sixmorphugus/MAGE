#include "helpers.h"

#include "basic.h"
#include "platform.h"
#include "maths.h"

namespace mage {
// This file is just all of the implementations for helpers in Super Alchemist.

sf::Color interpolateColors(sf::Color a, sf::Color b, int stepNumber, int lastStepNumber) {
	sf::Color finalc;

	finalc.r = interpolate(a.r, b.r, stepNumber, lastStepNumber);
	finalc.g = interpolate(a.g, b.g, stepNumber, lastStepNumber);
	finalc.b = interpolate(a.b, b.b, stepNumber, lastStepNumber);
	finalc.a = a.a;

	return finalc;
}

float distanceBetweenObjects(basic * i, basic * j)
{
	return distanceBetweenVectors(i->getCenter(), j->getCenter());
}

float randomPitch()
{
	return 1.f + ((float)randomInt(-2, 2) / 10.f);
}

} // namespace mage


#include "scriptingEngine.h"
using namespace mage;
using namespace chaiscript;

DeclareScriptingCustom(fun(&interpolateColors), "interpolateColors");
DeclareScriptingCustom(fun(&distanceBetweenObjects), "distanceBetweenObjects");