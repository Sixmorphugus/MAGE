#pragma once

// mage::color
// -------------
// A more powerful color class than SFML's.
// Stores normalized colors in RGBA.
// Offers SFML conversion.
// -------------
// Category: Rendering
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "StdAfx.h"
#include "SfmlAfx.h"

namespace mage {
class MAGEDLL colour
{
public:
	colour();
	colour(float red, float green, float blue, float alpha = 1.f);
	colour(int8_t red, int8_t green, int8_t blue, int8_t alpha = 255U);
	colour(float gray);
	colour(int8_t gray);
	colour(sf::Color& col);
	colour(std::string& hex);

	colour getMix(colour& b, float weight = 0.5f, bool processAlpha = false) const;
	colour getScaled(float s, bool processAlpha = false) const;
	colour getScaled(float rS, float gS, float bS, float aS = 0.f) const;

	// conversions
	float getHue() const;
	float getSaturation() const;
	float getLightness() const;

	std::string getHex() const;

	sf::Color toSf() const;

	// 16
	int8_t r8();
	int8_t g8();
	int8_t b8();
	int8_t a8();

	// comparison operators
	bool operator== (const colour &rhs) const;
	bool operator!= (const colour &rhs) const;

	colour& operator*=(const float rhs);
	colour& operator/=(const float rhs);

	colour operator*(const float rhs);
	colour operator/(const float rhs);

public:
	float r;
	float g;
	float b;

	float a;
};
}