#include "uiMenu.h"

#include "Game.h"
#include "mixer.h"

#include "inputMouse.h"
#include "inputKeyboard.h"

#include "resourceSoundBuffer.h"

#include "viewUi.h"
#include "uiFormattableText.h"

using namespace mage;

// menu.
uiMenu::uiMenu(float x, float y) :
	uiBasic(x, y, textureData(nullptr))
{
	init();
}

uiMenu::uiMenu(float x, float y, std::vector<std::string> opList) :
	uiBasic(x, y, textureData(nullptr)),
	optionList(opList)
{
	init();
}

void uiMenu::update(sf::Time elapsed) {
	// mouse control (gotta do this regardless of disabledness)
	int considered = 0;
	mouseOver = false;

	selectionChanged = false;

	for (unsigned int i = optionStart; i < optionList.size(); i++) {
		// mouse collision box for this option
		sf::FloatRect mouseBox(sf::Vector2f(getPosition().x + 14, getPosition().y + (i * 12) - (optionStart * 12)), sf::Vector2f(width, 12.f));

		if (mouseBox.contains((sf::Vector2f)theGame()->mousePos) && optionList[i] != "") {
			mouseOver = true;

			if (!disabled) {
				selected = i;

				if (theGame()->mouse->getLeftPressed()) {
					ready = true;
					superSelected = selected;
					selectionChanged = true;
					onSelectionChanged.notify(this, superSelected);
				}
			}
		}

		considered++;

		if (considered >= maxOptions) {
			break;
		}
	}

	if (disabled || optionList.size() == 0)
		return;

	float selectorDist = selectorPos - (getPosition().y + (selected * 12) - 3.f - (optionStart * 12));
	selectorPos = selectorPos - (selectorDist / 2);

	if (!ready) {
		selectorDist = selectorPos2 - (getPosition().y + (selected * 12) - 3.f - (optionStart * 12));
		selectorPos2 = selectorPos - (selectorDist / 2);
	}
	else {
		selectorDist = selectorPos2 - (getPosition().y + (superSelected * 12) - 3.f - (optionStart * 12));
		selectorPos2 = selectorPos2 - (selectorDist / 2);
	}

	// don't bother updating uiBasic (see boxWindow)

	bool autoScroll = false;

	// keyboard control
	if (!noKbd) {
		if (theGame()->keyboard->getBind("Up")->getPressed()) {
			selected--;
			validateSelected();
			skipDown = false;
			autoScroll = true;
		}
		else if (theGame()->keyboard->getBind("Down")->getPressed()) {
			selected++;
			validateSelected();
			skipDown = true;
			autoScroll = true;
		}
		else if (theGame()->keyboard->getBind("Select")->getPressed()) {
			ready = true;
			superSelected = selected;
			selectionChanged = true;
			onSelectionChanged.notify(this, superSelected);
		}
		else if (theGame()->keyboard->getBind("ESCAPE")->getPressed()) {
			dismissed = true;
			onDismissed.notify(this);
		}
	}

	if (!mouseOver && noKbd && ready) {
		selected = superSelected;
	}

	// auto scroll if we go beyond the allowed range
	if (!dragMode) {
		if (selected >= optionStart + maxOptions) {
			optionStart += 1;
		}
		else if (selected < optionStart) {
			optionStart -= 1;
		}
	}

	auto highestScrollAllowed = (int)optionList.size() - maxOptions;

	if (mouseOver) {
		optionStart += theGame()->mouse->scroll;

		if (optionStart < 0 || (int)optionList.size() < maxOptions) {
			optionStart = 0;
		}
		else if (optionStart >= highestScrollAllowed) {
			optionStart = highestScrollAllowed;
		}
	}

	// skip gaps
	selected %= optionList.size(); // hotfix

	if (optionList[selected] == "") {
		if (skipDown) {
			selected++;
			selected %= optionList.size();
		}
		else {
			selected--;
			selected %= optionList.size();
		}
	}

	if (selectionChanged) {
		theGame()->sound->play(theGame()->resources->getAs<resourceSoundBuffer>("sfx_menuchoose"));
	}

	// scroll can be manipulated without the mouse now
	float height = 12.f * (maxOptions);
	float barHeight = (height / optionList.size()) *  (maxOptions);

	float offset = (height / optionList.size()) * optionStart;

	sf::FloatRect barBox(sf::Vector2f(getPosition().x + width + 16.f, getPosition().y - 2.f), sf::Vector2f(2.f, height));
	sf::FloatRect scrollerBox(sf::Vector2f(getPosition().x + width + 15.f, getPosition().y - 2.f + offset), sf::Vector2f(4.f, (barHeight)));

	bool culled = (((int)optionList.size()) - 1 > maxOptions);

	if (culled) { // moving dat scrollbar with dat mouse
		scrollBarGlow = (scrollerBox.contains(theGame()->mousePos));

		if (theGame()->mouse->getLeftPressed() && scrollBarGlow) {
			scrollDragOffset = theGame()->mousePos - sf::Vector2f(scrollerBox.left, scrollerBox.top);
			dragMode = true;
		}

		if (dragMode) {
			sf::Vector2f nsdo = theGame()->mousePos - (sf::Vector2f(scrollerBox.left, scrollerBox.top) + scrollDragOffset);
			float requiredDist = height / optionList.size();

			if (nsdo.y > requiredDist  && optionStart < highestScrollAllowed) {
				optionStart += 1;
			}
			else if (nsdo.y < -requiredDist && optionStart > 0) {
				optionStart -= 1;
			}
		}

		if (theGame()->mouse->getLeftReleased() && dragMode) {
			dragMode = false;
		}
	}
}

void uiMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	// don't bother drawing uiBasic (see boxWindow)

	if (!disabled && !noKbd) {
		if (theGame()->mouseMode != 0) {
			drawBindPrompt(target, states, (int)(theGame()->uiCamera->getSize().y) - 32 - (24 * 2), { "Up", "Down" }, "Select");
			drawBindPrompt(target, states, (int)(theGame()->uiCamera->getSize().y) - 32 - 24, { "Select" }, "Accept");
		}

		drawBindPrompt(target, states, (int)(theGame()->uiCamera->getSize().y) - 32, { "ESCAPE" }, "Back");
	}

	// draw a box
	if (!disabled && selected != superSelected && selectorPos > getPosition().y - 4.f && selectorPos < getPosition().y + (maxOptions * 12.f)) {
		sf::RectangleShape rectBox(sf::Vector2f(width, 12.f));
		rectBox.setPosition(getPosition().x + 14, selectorPos);
		rectBox.setFillColor(sf::Color(50, 50, 80, 100));

		target.draw(rectBox, states);

		// draw selector
		if (!ready)
			drawInfo(target, states, sf::Vector2f(getPosition().x, selectorPos + 2.f), selectorText, sf::Color(255, 255, 200));
	}

	if (ready && !disabled && selectorPos2 > getPosition().y - 4.f && selectorPos2 < getPosition().y + (maxOptions * 12.f)) {
		sf::RectangleShape rectBox(sf::Vector2f(width, 12.f));
		rectBox.setPosition(getPosition().x + 14, selectorPos2);
		rectBox.setFillColor(sf::Color(50, 50, 80, 200));

		target.draw(rectBox, states);

		drawInfo(target, states, sf::Vector2f(getPosition().x, selectorPos2 + 2.f), selectorText, sf::Color(255, 255, 200));
	}

	// draw our options (todo: actually bother with the alignment setting, it's kinda important)
	int rendered = 0;
	bool culled = (((int)optionList.size()) - 1 > maxOptions);

	for (unsigned int i = optionStart; i < optionList.size(); i++) {
		std::string opString = optionList[i];
		//drawInfo(target, states, sf::Vector2f(getPosition().x + 16.f, getPosition().y + (i * 12.f) - (optionStart * 12.f)), opString, sf::Color(255, 255, 150));
		// no longer will we draw info!
		// now we use magicText here.

		uiFormattableText temp(getPosition().x + 16.f, getPosition().y + (i * 12.f) - (optionStart * 12.f), "[c:ffff96]" + opString);
		temp.draw(target, states);

		rendered++;

		if (rendered >= maxOptions) {
			//culled = true;
			break;
		}
	}

	if (culled) {
		// draw scroll bar along the side of the list
		float height = 12.f * (maxOptions);
		float barHeight = (height / optionList.size()) *  (maxOptions);

		float offset = (height / optionList.size()) * optionStart;

		sf::RectangleShape rectBox(sf::Vector2f(2.f, height));
		rectBox.setPosition(getPosition().x + width + 16.f, getPosition().y - 2.f);
		rectBox.setFillColor(sf::Color(50, 50, 80, 100));

		sf::RectangleShape rectBox2(sf::Vector2f(4.f, (barHeight)));
		rectBox2.setPosition(getPosition().x + width + 15.f, getPosition().y - 2.f + offset);
		rectBox2.setFillColor(sf::Color(50, 50, scrollBarGlow ? 160 : 80, 255));

		target.draw(rectBox, states);
		target.draw(rectBox2, states);
	}
}

void uiMenu::validateSelected() {
	if (selected < 0) {
		selected = 0;
	}
	else if (selected >((int)optionList.size()) - 1) {
		selected = optionList.size() - 1;
	}
	else {
		theGame()->sound->play(theGame()->resources->getAs<resourceSoundBuffer>("sfx_menuselect"));
	}
}

void uiMenu::init()
{
	selected = 0;
	superSelected = -1;
	disabled = false;
	noKbd = false;

	alignMode = uiMenu::LEFT;
	skipDown = true;
	width = 128.f;

	ready = false;
	dismissed = false;

	selectorPos = getPosition().y + (selected * 12) - 2;
	selectorPos2 = 0;
	selectorText = "@";
	maxOptions = 8;

	optionStart = 0;

	scrollBarGlow = false;
	dragMode = false;
}

/*
void uiMenu::setSelectorPosition(int op)
{
selected = op;

optionStart = op;
auto highestScrollAllowed = (int)optionList.size() - maxOptions;

if (optionStart > highestScrollAllowed) {
optionStart = highestScrollAllowed;
}
}
*/

void uiMenu::reset()
{
	ready = false;
	superSelected = -1;
	optionStart = 0;
	selected = 0;
}

// SE
using namespace chaiscript;

DeclareScriptingCustom(user_type<uiMenu>(), "uiMenu");
DeclareScriptingCustom(base_class<uiBasic, uiMenu>());
DeclareScriptingBaseClass(basic, uiMenu);
DeclareScriptingBaseClass(sf::Transformable, uiMenu);
DeclareScriptingCustom(constructor<uiMenu(float, float)>(), "uiMenu");
DeclareScriptingCustom(constructor<uiMenu(float, float, std::vector<std::string>)>(), "uiMenu");
DeclareScriptingCustom(fun(&uiMenu::disabled), "disabled");
DeclareScriptingCustom(fun(&uiMenu::wasDismissed), "wasDismissed");
DeclareScriptingCustom(fun(&uiMenu::maxOptions), "maxOptions");
DeclareScriptingCustom(fun(&uiMenu::isMouseOver), "isMouseOverMenu");
DeclareScriptingCustom(fun(&uiMenu::noKbd), "noKbd");
DeclareScriptingCustom(fun(&uiMenu::optionList), "optionList");
DeclareScriptingCustom(fun(&uiMenu::optionStart), "optionStart");