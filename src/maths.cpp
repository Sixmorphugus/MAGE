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

float vectorToAngle(sf::Vector2f vec)
{
	return atan2(vec.y, vec.x);
}

float vectorLength(sf::Vector2f vec)
{
	return sqrt(vec.y*vec.y + vec.x*vec.x); // this is just pythag again isnt it
}

float distanceBetweenVectors(sf::Vector2f v1, sf::Vector2f v2)
{
	// pythagoras method
	float xDist = fabs(v1.x - v2.x);
	float yDist = fabs(v1.y - v2.y);

	return sqrt(pow(xDist, 2) + pow(yDist, 2));
}

float directionToVector(sf::Vector2f v1, sf::Vector2f v2)
{
	sf::Vector2f v3(v1 - v2);

	return vectorToAngle(v3);
}

sf::Vector2f generateVector(float angle, float speed)
{
	sf::Vector2f vector;

	vector.x = cos(angle);
	vector.y = sin(angle);

	vector *= speed;

	return vector;
}

float dot(sf::Vector2f v1, sf::Vector2f v2)
{
	return (v1.x*v2.x + v1.y*v2.y);
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

DeclareScriptingCustom(fun(&lerp), "lerp");
DeclareScriptingCustom(fun(&interpolate), "interpolate");
DeclareScriptingCustom(fun(&randomInt), "randomInt");
DeclareScriptingCustom(fun(&randomFloat), "randomFloat");
DeclareScriptingCustom(fun(&isMultiple), "isMultiple");
DeclareScriptingCustom(fun(&vectorToAngle), "vectorToAngle");
DeclareScriptingCustom(fun(&vectorLength), "vectorLength");
DeclareScriptingCustom(fun(&distanceBetweenVectors), "distanceBetweenVectors");
DeclareScriptingCustom(fun(&directionToVector), "directionToVector");
DeclareScriptingCustom(fun(&generateVector), "generateVector");
DeclareScriptingCustom(fun(&dot), "dot");
DeclareScriptingCustom(fun(&clamp), "clamp");
DeclareScriptingCustom(fun<float, float>(&sin), "sin");
DeclareScriptingCustom(fun<float, float>(&cos), "cos");
DeclareScriptingCustom(fun<float, float>(&tan), "tan");
DeclareScriptingCustom(fun<float, float>(&abs), "abs");

DeclareScriptingCustom(fun(&highestOf<float>), "highestOf");
DeclareScriptingCustom(fun(&lowestOf<float>), "lowestOf");