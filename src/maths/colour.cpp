#include "colour.h"
#include "maths.h"

using namespace mage;

colour::colour()
{
	// the default color is white
	r = 1.f;
	g = 1.f;
	b = 1.f;
	a = 1.f;
}

colour::colour(float red, float green, float blue, float alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

colour::colour(int8_t red, int8_t green, int8_t blue, int8_t alpha)
{
	r = (float)red / 255.f;
	g = (float)green / 255.f;
	b = (float)blue / 255.f;
	a = (float)alpha / 255.f;
}

mage::colour::colour(float gray)
{
	r = gray;
	g = gray;
	b = gray;
	a = 1.f;
}

mage::colour::colour(int8_t gray)
{
	float grayF = (float)gray / 255.f;

	r = grayF;
	g = grayF;
	b = grayF;
	a = 1.f;
}

colour::colour(const sf::Color& col)
{
	r = (float)col.r / 255.f;
	g = (float)col.g / 255.f;
	b = (float)col.b / 255.f;
	a = (float)col.a / 255.f;
}

colour::colour(std::string& hex)
{
	std::string filtered = strFilter(strFilter(hex, "0x"), "#");

	// first two characters are our RED channel, and so on
	std::string redString = "00";
	std::string greenString = "00";
	std::string blueString = "00";
	std::string alphaString = "FF";

	if (filtered.size() >= 2)
		redString = strSub(filtered, 0, 2);

	if (filtered.size() >= 4)
		greenString = strSub(filtered, 2, 4);

	if (filtered.size() >= 6)
		greenString = strSub(filtered, 4, 6);

	if (filtered.size() >= 8)
		alphaString = strSub(filtered, 6, 8);

	int8_t red = (int8_t)std::strtol(redString.c_str(), NULL, 16);
	int8_t green = (int8_t)std::strtol(greenString.c_str(), NULL, 16);
	int8_t blue = (int8_t)std::strtol(blueString.c_str(), NULL, 16);
	int8_t alpha = (int8_t)std::strtol(alphaString.c_str(), NULL, 16);

	r = (float)red / 255.f;
	g = (float)green / 255.f;
	b = (float)blue / 255.f;
	a = (float)alpha / 255.f;
}

colour colour::getMix(colour& other, float weight, bool processAlpha) const
{
	colour finalc;

	finalc.r = lerp(r, other.r, weight);
	finalc.g = lerp(g, other.g, weight);
	finalc.b = lerp(b, other.b, weight);

	finalc.a = processAlpha ? lerp(a, other.a, weight) : a;

	return finalc;
}

colour colour::getScaled(float s, bool processAlpha) const
{
	colour scaled;

	scaled.r = r * s;
	scaled.g = g * s;
	scaled.b = b * s;

	if (processAlpha) scaled.a = a * s;

	return scaled;
}

colour colour::getScaled(float rS, float gS, float bS, float aS) const
{
	colour scaled;

	scaled.r = r * rS;
	scaled.g = g * gS;
	scaled.b = b * bS;
	scaled.a = a * aS;

	return scaled;
}

float colour::getHue() const
{
	float min, max, delta;

	min = r < g ? r : g;
	min = min  < b ? min : b;

	max = r > g ? r : g;
	max = max > b ? max : b;

	delta = max - min;

	if (delta < 0.00001)
	{
		return 0.f;
	}
	
	if (!(max > 0.0)) { // NOTE: if Max is == 0, this divide would cause a crash
		// if max is 0, then r = g = b = 0              
		// s = 0, v is undefined
		return NAN; // normally would be NAN
	}

	float h;

	if (r >= max) // is bogus, just keeps compilor happy
		h = (g - b) / delta; // between yellow & magenta
	else
		if (g >= max)
			h = 2.0 + (b - r) / delta; // between cyan & yellow
		else
			h = 4.0 + (r - g) / delta; // between magenta & cyan

	h *= 60.0; // degrees

	if (h < 0.0)
		h += 360.0;

	return h;
}

float colour::getSaturation() const
{
	float min, max, delta;

	min = r < g ? r : g;
	min = min  < b ? min : b;

	max = r > g ? r : g;
	max = max > b ? max : b;

	delta = max - min;

	if (!(delta < 0.00001))
	{
		return 0;
	}

	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		return (delta / max); // s
	}
	
	return 0.f;
}

float colour::getLightness() const
{
	// easiest to calculate.
	float max;

	max = r > g ? r : g;
	max = max > b ? max : b;

	return max;
}

std::string colour::getHex() const
{
	std::stringstream colourString;
	colourString << std::hex << toSf().r << toSf().g << toSf().b << toSf().a;

	return colourString.str();
}

sf::Color colour::toSf() const
{
	return sf::Color((int8_t)(255.f * r), (int8_t)(255.f * g), (int8_t)(255.f * b), (int8_t)(255.f * a));
}

int8_t mage::colour::r8()
{
	return (int8_t)(r * 255.f);
}

int8_t mage::colour::g8()
{
	return (int8_t)(g * 255.f);
}

int8_t mage::colour::b8()
{
	return (int8_t)(b * 255.f);
}

int8_t mage::colour::a8()
{
	return (int8_t)(a * 255.f);
}

bool colour::operator==(const colour & rhs) const
{
	if (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a) {
		return true;
	}

	return false;
}

bool colour::operator!=(const colour & rhs) const
{
	return !(*this == rhs);
}

colour& colour::operator*=(const float rhs)
{
	colour scaled = getScaled(rhs, true);
	r = scaled.r;
	b = scaled.b;
	g = scaled.g;
	a = scaled.a;

	return *this;
}

colour& colour::operator/=(const float rhs)
{
	colour scaled = getScaled(1.f / rhs, true);
	r = scaled.r;
	b = scaled.b;
	g = scaled.g;
	a = scaled.a;

	return *this;
}

colour colour::operator*(const float rhs)
{
	colour multiplied(*this);
	multiplied *= rhs;

	return multiplied;
}

colour colour::operator/(const float rhs)
{
	colour multiplied(*this);
	multiplied /= rhs;

	return multiplied;
}

// se
#include "scriptingEngine.h"

using namespace chaiscript;

MAGE_DeclareScriptingType(colour);
MAGE_DeclareScriptingConstructor(colour(), "colour");
MAGE_DeclareScriptingConstructor(colour(float), "colour");
MAGE_DeclareScriptingConstructor(colour(int8_t), "colour");
MAGE_DeclareScriptingConstructor(colour(float, float, float, float), "colour");
MAGE_DeclareScriptingConstructor(colour(int8_t, int8_t, int8_t, int8_t), "colour");
MAGE_DeclareScriptingConstructor(colour(sf::Color), "colour");
MAGE_DeclareScriptingConstructor(colour(std::string), "colour");
MAGE_DeclareScriptingConstructor(colour(const colour&), "colour");
MAGE_DeclareScriptingCopyOperator(colour);
MAGE_DeclareScriptingFunction(&colour::r, "r");
MAGE_DeclareScriptingFunction(&colour::g, "g");
MAGE_DeclareScriptingFunction(&colour::b, "b");
MAGE_DeclareScriptingFunction(&colour::a, "a");
MAGE_DeclareScriptingFunction(&colour::r8, "r8");
MAGE_DeclareScriptingFunction(&colour::g8, "g8");
MAGE_DeclareScriptingFunction(&colour::b8, "b8");
MAGE_DeclareScriptingFunction(&colour::a8, "a8");
MAGE_DeclareScriptingFunction(&colour::getMix, "getMix");
MAGE_DeclareScriptingFunction(&colour::getHue, "getHue");
MAGE_DeclareScriptingFunction(&colour::getSaturation, "getSaturation");
MAGE_DeclareScriptingFunction(&colour::getLightness, "getLightness");
MAGE_DeclareScriptingFunction(&colour::getHex, "getHex");
MAGE_DeclareScriptingFunction(&colour::operator*, "*");
MAGE_DeclareScriptingFunction(&colour::operator/, "/");
MAGE_DeclareScriptingFunction(&colour::operator*=, "*=");
MAGE_DeclareScriptingFunction(&colour::operator/=, "/=");
MAGE_DeclareScriptingFunction(&colour::operator==, "==");
MAGE_DeclareScriptingFunction(&colour::operator!=, "!=");
MAGE_DeclareScriptingCustom(fun<colour, colour, float, bool>(&colour::getScaled), "getScaled");
MAGE_DeclareScriptingCustom(fun<colour, colour, float, float, float, float>(&colour::getScaled), "getScaled");
