#pragma once
#include "uiBasic.h"
#include "textHelpers.h"
#include "Game.h"
#include "gameState.h"

using namespace mage;

// BASIC UI (user interface) OBJECT
// ----------------------------------------------------------------------------
uiBasic::uiBasic(float x, float y, textureData sd)
	: basic(x, y, sd)
{
}

void uiBasic::worldDraw(sf::RenderTarget &target, sf::RenderStates states) const {
	onWorldDraw.notify(this);
}

void uiBasic::registerProperties()
{
	basic::registerProperties();
}

void uiBasic::drawBindPrompt(sf::RenderTarget & target, sf::RenderStates states, int yPosition, std::vector<std::string> keybindNames, std::string caption) const
{
	renderBindPrompt(target, states, yPosition, keybindNames, caption);
}

float uiBasic::drawInfoSimple(sf::RenderTarget & target, sf::RenderStates states, sf::Vector2f screenPos, std::string info) const
{
	return renderInfo(target, states, screenPos, info);
}

float uiBasic::drawInfo(sf::RenderTarget & target, sf::RenderStates states, sf::Vector2f screenPos, std::string info, sf::Color col, bool center, unsigned int charSize, float rot, std::shared_ptr<resourceFont> customFont) const
{
	return renderInfo(target, states, screenPos, info, col, center, charSize, rot, customFont);
}

void uiBasic::drawSelf(sf::RenderTarget & target, sf::RenderStates states, sf::Vector2f screenPos, unsigned int sprite, unsigned int frame, sf::Color col) const
{
	// sanity!
	if (sprites.size() <= sprite)
		return;

	if (sprites[sprite].animations.frameRects.size() <= frame)
		return;

	// drawing can be unsafe because of the sanity checks
	const sf::Texture* tex = getTexPointer();

	sf::Sprite fakeSprite(*tex, sprites[sprite].animations.frameRects[frame]);
	fakeSprite.setPosition(screenPos);
	fakeSprite.setColor(col);

	target.draw(fakeSprite, states);
}

void uiBasic::bringToFront()
{
	theGame()->state->bringToFront(theGame()->state->localIndexOf(this));
}

bool uiBasic::isMouseOver()
{
	sf::Vector2f mousePos(theGame()->mousePos.x, theGame()->mousePos.y);
	sf::FloatRect bounds = sfSprite.getGlobalBounds();

	return bounds.contains(mousePos);
}

bool uiBasic::isOnScreen() const
{
	return true; // let's just always assume this is visible.
				 // There are never normally enough UI on the screen to lag the game anyway.
				 // And there never will be. Right?
				 // RIGHT?
				 // Yeah I can see myself rewriting this function in the near future.
}

bool compareUi(std::shared_ptr<uiBasic> i, std::shared_ptr<uiBasic> j)
{
	return i->operator<(*j);
}

using namespace chaiscript;

DeclareScriptingBasic(uiBasic);
DeclareScriptingCustom(constructor<uiBasic(float, float, basic::textureData)>(), "uiBasic");
DeclareScriptingCustom(fun(&uiBasic::drawBindPrompt), "drawBindPrompt");
DeclareScriptingCustom(fun(&uiBasic::drawInfoSimple), "drawInfoSimple");
DeclareScriptingCustom(fun(&uiBasic::drawInfo), "drawInfo");