#include "uiTextEntry.h"

#include "Game.h"
#include "stringHelpers.h"

#include "resourceFont.h"

#include "inputKeyboard.h"
#include "inputMouse.h"

using namespace mage;

// textEntry
uiTextEntry::uiTextEntry(float x, float y)
	: uiBasic(x, y, textureData(nullptr))
{
	active = true;
	ready = false;
	width = 128.f;
	mouseSensitive = true;
	safe = false;
}

void uiTextEntry::update(sf::Time elapsed)
{
	displayFrom = 0;

	sf::Text benchmarkObject(contains, *theGame()->resources->getAs<resourceFont>("__fontDef")->get(), 16U);
	float spaceUsed = benchmarkObject.getGlobalBounds().width;
	float charSpace = spaceUsed / (float)contains.size();

	if (spaceUsed > width) {
		float dif = spaceUsed - width;
		displayFrom = (unsigned int)floor(dif / charSpace);
	}

	if (active) {
		if (theGame()->keyboard->inputStream == "\b") {
			if (contains.size() > 0)
				contains.erase(contains.begin() + (contains.size() - 1));
		}
		else {
			if (safe) {
				if (isalnum(theGame()->keyboard->inputStream[0]) || theGame()->keyboard->inputStream[0] == ' ')
					contains += theGame()->keyboard->inputStream;
			}
			else {
				if (isprint(theGame()->keyboard->inputStream[0]) && theGame()->keyboard->inputStream[0] != '"')
					contains += theGame()->keyboard->inputStream;
			}
		}
	}

	if (mouseSensitive) {
		sf::FloatRect msRect(getPosition(), sf::Vector2f(width, 12.f));

		if (msRect.contains(theGame()->mousePos)) {
			if (theGame()->mouse->getLeftPressed()) {
				active = true;
			}
		}
		else {
			if (theGame()->mouse->getLeftPressed()) {
				active = false;
			}
		}
	}

	if (theGame()->keyboard->getBind("ENTER")->getPressed() && active) {
		ready = true;
		active = false;
	}
}

void uiTextEntry::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	std::string drawText = strSub(contains, displayFrom, drawText.size());

	if (active) {
		drawText += "|";
	}

	drawInfo(target, states, getPosition() - sf::Vector2f(0, 4.f), drawText, sf::Color(255, 255, 100));
}

using namespace chaiscript;

DeclareScriptingCustom(user_type<uiTextEntry>(), "uiTextEntry");
DeclareScriptingCustom(base_class<uiBasic, uiTextEntry>());
DeclareScriptingBaseClass(basic, uiTextEntry);
DeclareScriptingBaseClass(sf::Transformable, uiTextEntry);
DeclareScriptingCustom(constructor<uiTextEntry(float, float)>(), "uiTextEntry");
DeclareScriptingCustom(fun(&uiTextEntry::active), "active");
DeclareScriptingCustom(fun(&uiTextEntry::contains), "contains");
DeclareScriptingCustom(fun(&uiTextEntry::displayFrom), "displayFrom");
DeclareScriptingCustom(fun(&uiTextEntry::ready), "ready");
DeclareScriptingCustom(fun(&uiTextEntry::mouseSensitive), "mouseSensitive");
DeclareScriptingCustom(fun(&uiTextEntry::safe), "safe");
DeclareScriptingCustom(fun(&uiTextEntry::width), "width");