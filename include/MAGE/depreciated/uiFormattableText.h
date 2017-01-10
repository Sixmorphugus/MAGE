#pragma once
#include "uiBasic.h"

namespace mage {

// uiFormattableText
// text formattable with BBC tags
class MAGEDLL uiFormattableText : public uiBasic
{
public:
	uiFormattableText(float x, float y, std::string text = "");

	enum animationMode {
		NONE,
		JITTER,
		SLEEPY,
		SCOOBY,
		BULGE
	};

	enum colorMode {
		SOLID,
		BLINK,
		RAINBOW
	};

	enum alignmentMode {
		LEFT,
		CENTER
	};

	std::string text;

	void update(sf::Time elapsed);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void newLine(sf::RenderTarget &target, sf::RenderStates states, unsigned int startPosition, animationMode am, colorMode cm, unsigned int s, sf::Color c, std::shared_ptr<resourceFont> f, int num = 1) const;

	static std::string stopAtScrollPoint(std::string text, unsigned int textPhase);
	static unsigned int highestScrollPoint(std::string text);

	float findSizeX() const;

	int lineLimit;
	sf::Uint8 opacity;

	animationMode defaultAnimationMode;
	colorMode defaultColorMode;
	unsigned int defaultSize;
	sf::Color defaultColor;
	std::shared_ptr<resourceFont> defaultFont;

	alignmentMode alignment;

	float lineSpacing;
};

}