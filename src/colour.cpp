#include "colour.h"
#include "maths.h"

using namespace mage;

colour colour::mix(colour& other, float weight, bool processAlpha) const
{
	colour finalc;

	finalc.r = lerp(r, other.r, weight);
	finalc.g = lerp(g, other.g, weight);
	finalc.b = lerp(b, other.b, weight);

	finalc.a = processAlpha ? lerp(a, other.a, weight) : a;

	return finalc;
}