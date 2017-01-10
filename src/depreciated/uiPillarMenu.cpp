#include "uiPillarMenu.h"

#include "Game.h"

using namespace mage;

uiPillarMenu::uiPillarMenu(float x, float y)
	: uiMenu(x, y)
{
	fi = false;
	fo = false;
	drawLines = false;

	targetX = x;
}

uiPillarMenu::uiPillarMenu(float x, float y, std::vector<std::string> opList)
	: uiMenu(x, y, opList)
{
	fi = false;
	fo = false;
	drawLines = false;

	targetX = x;
}

void uiPillarMenu::update(sf::Time elapsed)
{
	uiMenu::update(elapsed);

	if (fi) {
		float dist = (targetX - getPosition().x);
		move(dist * 0.1f, 0.f);

		if (dist < 0.1f) {
			fi = false;
			setPosition(targetX, getPosition().y);
		}
	}

	if (fo) {
		float dist = (getPosition().x - (-width));
		move(dist * 0.1f, 0.f);

		if (getPosition().x > theGame()->getRenderWindow().getSize().x) {
			fo = false;
		}
	}
}

void uiPillarMenu::floatIn() {
	setPosition(-width, getPosition().y);
	fi = true;
	fo = false;

	disabled = false;
}

void uiPillarMenu::floatOut()
{
	//setPosition(targetX, getPosition().y);
	fo = true;
	fi = false;

	disabled = true;
}

void uiPillarMenu::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// draw a pillar the size of the menu going from the top of the screen to the bottom.
	sf::RectangleShape rec;

	rec.setPosition(sf::Vector2f(getPosition().x - 4.f, 0.f));
	rec.setSize(sf::Vector2f(width + 22.f, (float)theGame()->getRenderWindow().getSize().y));
	rec.setFillColor(sf::Color(0, 0, 20, 150));

	target.draw(rec);

	rec.setSize(sf::Vector2f(rec.getSize().x + 2.f, rec.getSize().y));
	rec.move(-1.f, 0.f);

	target.draw(rec);

	// lines
	if (drawLines) {
		rec.setPosition(rec.getPosition().x + 8.f, getPosition().y - 8.f);
		rec.setSize(sf::Vector2f(rec.getSize().x - 16.f, 1.f));
		rec.setFillColor(sf::Color::White);

		target.draw(rec);

		if ((int)optionList.size() < maxOptions)
			rec.setPosition(rec.getPosition().x, getPosition().y + (optionList.size() * 12.f) + 4.f);
		else
			rec.setPosition(rec.getPosition().x, getPosition().y + (maxOptions * 12.f) + 4.f);

		rec.setSize(sf::Vector2f(rec.getSize().x, 1.f));
		rec.setFillColor(sf::Color::White);

		target.draw(rec);
	}

	uiMenu::draw(target, states);
}

using namespace chaiscript;

MAGE_DeclareScriptingCustom(user_type<uiPillarMenu>(), "pillarMenu");
MAGE_DeclareScriptingCustom(base_class<uiMenu, uiPillarMenu>());
MAGE_DeclareScriptingCustom(type_conversion<uiPillarMenu*, uiBasic*>());
MAGE_DeclareScriptingBaseClass(basic, uiPillarMenu);
MAGE_DeclareScriptingBaseClass(sf::Transformable, uiPillarMenu);
MAGE_DeclareScriptingCustom(type_conversion<uiPillarMenu*, uiBasic*>());
MAGE_DeclareScriptingCustom(constructor<uiPillarMenu(float, float)>(), "pillarMenu");
MAGE_DeclareScriptingCustom(constructor<uiPillarMenu(float, float, std::vector<std::string>)>(), "pillarMenu");
MAGE_DeclareScriptingCustom(fun(&uiPillarMenu::floatIn), "floatIn");
MAGE_DeclareScriptingCustom(fun(&uiPillarMenu::floatIn), "floatOut");
MAGE_DeclareScriptingCustom(fun(&uiPillarMenu::drawLines), "drawLines");
MAGE_DeclareScriptingCustom(fun(&uiPillarMenu::targetX), "targetX");