#include "maths.h"

#include "platform.h"

namespace mage {

int interpolate(int startValue, int endValue, int stepNumber, int lastStepNumber)
{
	return (endValue - startValue) * stepNumber / lastStepNumber + startValue;
}

float lerp(float a, float b, float f)
{
	return (a * (1.0f - f)) + (b * f);
}

int randomInt(int min, int max) //range : [min, max)
{
	if (!(max - min)) {
		return min;
	}

	return min + rand() % (max - min);
}

float randomFloat(float low, float high)
{
	float r3 = low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
	return r3;
}

bool isMultiple(float testNum, float of) {
	return (testNum / of) == floor(testNum / of);
}

float clamp(float in, float min, float max)
{
	if (in < min) in = min;
	if (in > max) in = max;

	return in;
}

sf::Vector2f vectorFloor(sf::Vector2f vec)
{
	return sf::Vector2f(floor(vec.x), floor(vec.y));
}

sf::Vector2f vectorCeil(sf::Vector2f vec)
{
	return sf::Vector2f(ceil(vec.x), ceil(vec.y));
}

} // namespace mage

// se
#include "scriptingEngine.h"
using namespace chaiscript;
using namespace mage;

MAGE_DeclareScriptingCustom(fun(&lerp), "lerp");
MAGE_DeclareScriptingCustom(fun(&interpolate), "interpolate");
MAGE_DeclareScriptingCustom(fun(&randomInt), "randomInt");
MAGE_DeclareScriptingCustom(fun(&randomFloat), "randomFloat");
MAGE_DeclareScriptingCustom(fun(&isMultiple), "isMultiple");
MAGE_DeclareScriptingCustom(fun(&vectorToAngle), "vectorToAngle");
MAGE_DeclareScriptingCustom(fun(&vectorLength), "vectorLength");
MAGE_DeclareScriptingCustom(fun(&distanceBetweenVectors), "distanceBetweenVectors");
MAGE_DeclareScriptingCustom(fun(&directionToVector), "directionToVector");
MAGE_DeclareScriptingCustom(fun(&generateVector), "generateVector");
MAGE_DeclareScriptingCustom(fun(&dot), "dot");
MAGE_DeclareScriptingCustom(fun(&clamp), "clamp");
MAGE_DeclareScriptingCustom(fun<float, float>(&sin), "sin");
MAGE_DeclareScriptingCustom(fun<float, float>(&cos), "cos");
MAGE_DeclareScriptingCustom(fun<float, float>(&tan), "tan");
MAGE_DeclareScriptingCustom(fun<float, float>(&abs), "abs");

MAGE_DeclareScriptingCustom(fun(&highestOf<float>), "highestOf");
MAGE_DeclareScriptingCustom(fun(&lowestOf<float>), "lowestOf");