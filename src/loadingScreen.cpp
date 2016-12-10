#include "loadingScreen.h"

#include "textHelpers.h"
#include "viewUi.h"
#include "Game.h"

using namespace mage;

// loading screen.
loadingScreen::loadingScreen()
{
}

void loadingScreen::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (!canDraw()) return;

	target.clear(sf::Color(0, 0, 10));
	auto screenCenter = theGame()->uiCamera->getSize() / 2.f;

	// info
	renderInfo(target, states, screenCenter - sf::Vector2f(0.f, -(screenCenter.y - 64.f)), loading, sf::Color(255, 255, 200), true, 16U, 0.f);

	// loading bar
	float barLength = theGame()->uiCamera->getSize().x - 128.f;
	float barHt = 2.f;

	// back
	sf::RectangleShape bar;
	bar.setFillColor(sf::Color(50, 50, 50));
	bar.setSize(sf::Vector2f(barLength, barHt));
	bar.setPosition(screenCenter - sf::Vector2f(barLength / 2.f, -(screenCenter.y - 40.f)));

	target.draw(bar, states);

	// front
	bar.setFillColor(sf::Color(255, 255, 255));
	bar.setSize(sf::Vector2f((barLength / (float)countMax) * (float)(count + 1), barHt));

	target.draw(bar, states);
}

bool loadingScreen::canDraw() const {
	auto fd = theGame()->resources->get("__fontDef", false);

	if (fd) {
		return fd->isLoaded();
	}

	return false;
}