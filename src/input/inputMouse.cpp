#include "inputMouse.h"

using namespace mage;

// MOUSE
// ---------------------------------------------------
inputMouse::inputMouse()
{
	leftState = 0;
	rightState = 0;
	middleState = 0;

	scroll = 0;

	justUsed = false;

	// Doesn't support your batshit 8 button mouse. YOU ONLY NEED THREE.
}

void inputMouse::processEvent(sf::Event evt) {
	// the Game state passes mouse events our way. According to these, we change our state to reflect that of the mouse.
	if (evt.type == sf::Event::MouseMoved) {
		position.x = (float)evt.mouseMove.x;
		position.y = (float)evt.mouseMove.y;

		justUsed = true;
	}
	else if (evt.type == sf::Event::MouseButtonPressed) {
		lpPosition.x = (float)evt.mouseButton.x;
		lpPosition.y = (float)evt.mouseButton.y;

		position.x = (float)evt.mouseButton.x;
		position.y = (float)evt.mouseButton.y;

		if (evt.mouseButton.button == sf::Mouse::Button::Left) {
			leftState = 2;
			onLeftPressed.notify(this);
		}
		else if (evt.mouseButton.button == sf::Mouse::Button::Right) {
			rightState = 2;
			onRightPressed.notify(this);
		}
		else if (evt.mouseButton.button == sf::Mouse::Button::Middle) {
			middleState = 2;
			onMiddlePressed.notify(this);
		}

		justUsed = true;
	}
	else if (evt.type == sf::Event::MouseButtonReleased) {
		lrPosition.x = (float)evt.mouseButton.x;
		lrPosition.y = (float)evt.mouseButton.y;

		position.x = (float)evt.mouseButton.x;
		position.y = (float)evt.mouseButton.y;

		if (evt.mouseButton.button == sf::Mouse::Button::Left) {
			leftState = -1;
			onLeftReleased.notify(this);
		}
		else if (evt.mouseButton.button == sf::Mouse::Button::Right) {
			rightState = -1;
			onRightReleased.notify(this);
		}
		else if (evt.mouseButton.button == sf::Mouse::Button::Middle) {
			middleState = -1;
			onMiddleReleased.notify(this);
		}

		justUsed = true;
	}
	else if (evt.type == sf::Event::MouseWheelScrolled) {
		scroll = evt.mouseWheelScroll.delta > 0 ? -1 : 1;
		justUsed = true;
	}
}

void inputMouse::update() {
	// normalize everything

	// LEFT BUTTON
	if (leftState == 2)
		leftState = 1;
	else if (leftState == -1)
		leftState = 0;

	// RIGHT BUTTON
	if (rightState == 2)
		rightState = 1;
	else if (rightState == -1)
		rightState = 0;

	// MIDDLE BUTTON
	if (middleState == 2)
		middleState = 1;
	else if (middleState == -1)
		middleState = 0;

	scroll = 0;
	justUsed = false;
}

void inputMouse::scrollReset() {
	scroll = 0; // !?!
}

void inputMouse::reset()
{
	leftState = 0;
	rightState = 0;
	middleState = 0;

	scroll = 0;

	justUsed = false;
}

bool inputMouse::getLeftDown() {
	return (leftState > 0);
}

bool inputMouse::getRightDown() {
	return (rightState > 0);
}

bool inputMouse::getMiddleDown() {
	return (middleState > 0);
}

bool inputMouse::getLeftPressed() {
	return (leftState == 2);
}

bool inputMouse::getRightPressed() {
	return (rightState == 2);
}

bool inputMouse::getMiddlePressed() {
	return (middleState == 2);
}

bool inputMouse::getLeftReleased() {
	return (leftState == -1);
}

bool inputMouse::getRightReleased() {
	return (rightState == -1);
}

bool inputMouse::getMiddleReleased() {
	return (middleState == -1);
}

void inputMouse::setDefaultButtonTextures()
{
	mouseLeftTexture = "ui_input_mouseLeft";
	mouseRightTexture = "ui_input_mouseRight";
	mouseMiddleTexture = "ui_input_mouseMiddle";
}

// SE
#include "scriptingEngine.h"

using namespace mage;

MAGE_DeclareScriptingType(inputMouse);
MAGE_DeclareScriptingBaseClass(input, inputMouse);
MAGE_DeclareScriptingConstructor(inputMouse(), "mouseMngr");
MAGE_DeclareScriptingFunction(&inputMouse::getLeftDown, "getLeftDown");
MAGE_DeclareScriptingFunction(&inputMouse::getLeftPressed, "getLeftPressed");
MAGE_DeclareScriptingFunction(&inputMouse::getLeftReleased, "getLeftReleased");
MAGE_DeclareScriptingFunction(&inputMouse::getMiddleDown, "getMiddleDown");
MAGE_DeclareScriptingFunction(&inputMouse::getMiddlePressed, "getMiddlePressed");
MAGE_DeclareScriptingFunction(&inputMouse::getMiddleReleased, "getMiddleReleased");
MAGE_DeclareScriptingFunction(&inputMouse::justUsed, "justUsed");
MAGE_DeclareScriptingFunction(&inputMouse::lpPosition, "lpPosition");
MAGE_DeclareScriptingFunction(&inputMouse::lrPosition, "lrPosition");
MAGE_DeclareScriptingFunction(&inputMouse::position, "position");
MAGE_DeclareScriptingFunction(&inputMouse::mouseLeftTexture, "mouseLeftTexture");
MAGE_DeclareScriptingFunction(&inputMouse::mouseMiddleTexture, "mouseMiddleTexture");
MAGE_DeclareScriptingFunction(&inputMouse::mouseRightTexture, "mouseRightTexture");
MAGE_DeclareScriptingFunction(&inputMouse::reset, "reset");
MAGE_DeclareScriptingFunction(&inputMouse::scroll, "scroll");
MAGE_DeclareScriptingFunction(&inputMouse::scrollReset, "scrollReset");
MAGE_DeclareScriptingFunction(&inputMouse::setDefaultButtonTextures, "setDefaultButtonTextures");