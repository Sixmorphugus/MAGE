#pragma once
#include "objBasic.h"

#include "group.h"
#include "viewObj.h"
#include "Game.h"
#include "mixer.h"
#include "maths.h"

using namespace mage;

// BASIC WORLD OBJECT
// ----------------------------------------------------------------------------
objBasic::objBasic(float x, float y, textureData sd)
	: basic(x, y, sd)
{
	drawBottomOffset = 0;
	uiName = "";
}

void objBasic::registerProperties()
{
	basic::registerProperties();

	// reg props
	registerProperty("drawBottomOffset", prop(drawBottomOffset));
	deleteProperty("drawBottomOffset"); // drawBottomOffset is legacy

	registerProperty("_Z", prop(drawBottomOffset));
}

void objBasic::update(sf::Time elapsed)
{
	basic::update(elapsed);

	if (hasMoved() && getGroup()) {
		getGroup()->objectListDirty = true;
	}

	if (!pulledCamera.expired()) {
		auto l = pulledCamera.lock();
		l->moveTowards(*this, pulledCameraMultiplier);
	}
}

void objBasic::drawShadows(sf::RenderTarget & target) const
{

}

void objBasic::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	basic::draw(target, states);
}

void objBasic::drawCollisionBoxes(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (unsigned int i = 0; i < collisionBoxes.size(); i++) {
		auto colBox = transformedCollisionBox(i);

		sf::RectangleShape rectPreview(sf::Vector2f(colBox.width, colBox.height));

		rectPreview.setOutlineThickness(1.f);
		rectPreview.setFillColor(sf::Color::Transparent);
		rectPreview.setOutlineColor(sf::Color::Magenta);

		rectPreview.setPosition(colBox.left, colBox.top);

		target.draw(rectPreview, states);
	}
}

void objBasic::setPositionC(sf::Vector2f newPosition)
{
	// move object so its first collision box ends up in a certain place
	if (collisionBoxes.size() > 0)
		setPosition(newPosition - sf::Vector2f(collisionBoxes[0].left, collisionBoxes[0].top));
	else
		setPosition(newPosition);
}

void objBasic::setPositionC(float x, float y)
{
	setPositionC(sf::Vector2f(x, y));
}

void objBasic::generateCollisionBox(float offL, float offR, float offT, float offB) {
	// just generates a square box the size of the bounds.
	sf::FloatRect box(0, 0, sfSprite.getGlobalBounds().width, sfSprite.getGlobalBounds().height);

	box.left += offL;
	box.width -= offL;

	box.top += offT;
	box.height -= offT;

	box.width -= offR;

	box.height -= offB;

	collisionBoxes.push_back(box);
}

void objBasic::generateCollisionBoxWithPercentages(float offL, float offR, float offT, float offB)
{
	// just generates a square box the size of the bounds.
	sf::FloatRect box(0, 0, sfSprite.getGlobalBounds().width, sfSprite.getGlobalBounds().height);

	box.left += offL * sfSprite.getGlobalBounds().width;
	box.width -= offL * sfSprite.getGlobalBounds().width;

	box.top += offT * sfSprite.getGlobalBounds().height;
	box.height -= offT * sfSprite.getGlobalBounds().height;

	box.width -= offR * sfSprite.getGlobalBounds().width;
	box.height -= offB * sfSprite.getGlobalBounds().height;

	collisionBoxes.push_back(box);
}

sf::FloatRect objBasic::transformedCollisionBox(int id) const {
	return sf::FloatRect(getPosition().x + (getScale().x *collisionBoxes[id].left),
		getPosition().y + (getScale().y * collisionBoxes[id].top),
		getScale().x * collisionBoxes[id].width,
		getScale().y * collisionBoxes[id].height);
}

objBasic::collision objBasic::resolveMovement(float byX, float byY) {
	return theGame()->resolveMovement(this, (int)byX, (int)byY);
}

float objBasic::getDrawBottom() {
	return getMainBounds().top + getMainBounds().height + drawBottomOffset;
}

void objBasic::playAudio(std::shared_ptr<resourceSoundBuffer> sound, bool replaceSame, bool generatePitch)
{
	// use the center of the screen as the "listen" point

	// calculate vector distance without normalisation or pythag
	sf::Vector2f lp(theGame()->worldCamera->bounds().left + (theGame()->worldCamera->bounds().top / 2.f), theGame()->worldCamera->bounds().top + (theGame()->worldCamera->bounds().height / 2.f)); // find center of camera focus
	sf::Vector2f dist = getPosition() - lp;

	// play sound
	theGame()->sound->play(sound, dist, replaceSame, generatePitch);
}

void objBasic::pullCamera(std::shared_ptr<view> toPull, float multiplier)
{
	pulledCamera = toPull;
	pulledCameraMultiplier = clamp(multiplier, 0.f, 1.f);
}

void objBasic::stopPullingCamera()
{
	pulledCamera.reset();
}

bool compareObjects(std::shared_ptr<objBasic> i, std::shared_ptr<objBasic> j) {
	return i->operator<(*j);
}

using namespace chaiscript;

DeclareScriptingBasic(objBasic);
DeclareScriptingCustom(constructor<objBasic(float, float, basic::textureData)>(), "objBasic");
DeclareScriptingCustom(fun(&objBasic::collisionBoxes), "collisionBoxes");
DeclareScriptingCustom(fun(&objBasic::displayText), "displayText");
DeclareScriptingCustom(fun(&objBasic::drawShadows), "drawShadows");
DeclareScriptingCustom(fun(&objBasic::generateCollisionBox), "generateCollisionBox");
DeclareScriptingCustom(fun(&objBasic::generateCollisionBoxWithPercentages), "generateCollisionBoxWithPercentages");
DeclareScriptingCustom(fun(&objBasic::transformedCollisionBox), "transformedCollisionBox");
DeclareScriptingCustom(fun(&objBasic::drawBottomOffset), "drawBottomOffset");
DeclareScriptingCustom(fun(&objBasic::playAudio), "playAudio");
DeclareScriptingCustom(fun(&objBasic::getPrefabSource), "getPrefabSource");
DeclareScriptingCustom(fun(&objBasic::pullCamera), "pullCamera");
DeclareScriptingCustom(fun(&objBasic::stopPullingCamera), "pullCamera");
DeclareScriptingCustom(fun<void, objBasic, float, float>(&objBasic::setPositionC), "setPositionC");
DeclareScriptingCustom(fun<void, objBasic, sf::Vector2f>(&objBasic::setPositionC), "setPositionC");
DeclareScriptingCustom(fun(&objBasic::uiName), "uiName");