#pragma once
#include "basic.h"

namespace mage {

// BASIC UI (user interface) OBJECT
// ----------------------------------------------------------------------------
// Objects that follow the camera and always render at the same scale.
// These don't depth render.
// We're kind of just ignoring the property/clone functionality with this since it's not really meant to be a complex gui system.

class MAGEDLL uiBasic
	: public basic
{
public:
	CLONEABLE(uiBasic);

	uiBasic(float x, float y, textureData sd = textureData(nullptr));

	virtual void worldDraw(sf::RenderTarget &target, sf::RenderStates states) const;

	virtual void registerProperties();

	virtual bool isMouseOver();
	bool isOnScreen() const;

	virtual bool operator<(uiBasic& right)
	{
		return (getPosition().y > right.getPosition().y);
	}

	void drawBindPrompt(sf::RenderTarget &target, sf::RenderStates states, int yPosition, std::vector<std::string> keybindNames, std::string caption) const;
	float drawInfoSimple(sf::RenderTarget &target, sf::RenderStates states, sf::Vector2f screenPos, std::string info) const;
	float drawInfo(sf::RenderTarget &target, sf::RenderStates states, sf::Vector2f screenPos, std::string info, sf::Color col, bool center = false, unsigned int charSize = 16, float rot = 0.f, std::shared_ptr<resourceFont> customFont = nullptr) const;

	void drawSelf(sf::RenderTarget &target, sf::RenderStates states, sf::Vector2f screenPos, unsigned int sprite, unsigned int frame = 0, sf::Color col = sf::Color::White) const;

	void bringToFront();

public:
	hook<const basic*> onWorldDraw;
};

MAGEDLL bool compareUi(std::shared_ptr<uiBasic> i, std::shared_ptr<uiBasic> j);

} // namespace mage