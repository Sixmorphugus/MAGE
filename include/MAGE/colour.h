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
	class colour
	{
	public:
		colour();
		colour(float red, float green, float blue, float alpha = 1.f);
		colour(int8_t red, int8_t green, int8_t blue, int8_t alpha = 255U);
		colour(std::string hex);
		colour(sf::Color col);

		colour get(colour& b, float weight = 0.5f, bool processAlpha = false) const;
		colour getScaled(float s, bool processAlpha = false) const;
		colour getScaled(float rS, float gS, float bS, float aS = 0.f) const;

		float getHue() const;
		float getSaturation() const;
		float getLightness() const;

		std::string getHex() const;

		sf::Color toSf();

	public:
		float r;
		float g;
		float b;

		float a;
	};
}