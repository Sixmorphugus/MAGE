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

DeclareScriptingType(inputMouse);
DeclareScriptingBaseClass(input, inputMouse);
DeclareScriptingConstructor(inputMouse(), "mouseMngr");
DeclareScriptingFunction(&inputMouse::getLeftDown, "getLeftDown");
DeclareScriptingFunction(&inputMouse::getLeftPressed, "getLeftPressed");
DeclareScriptingFunction(&inputMouse::getLeftReleased, "getLeftReleased");
DeclareScriptingFunction(&inputMouse::getMiddleDown, "getMiddleDown");
DeclareScriptingFunction(&inputMouse::getMiddlePressed, "getMiddlePressed");
DeclareScriptingFunction(&inputMouse::getMiddleReleased, "getMiddleReleased");
DeclareScriptingFunction(&inputMouse::justUsed, "justUsed");
DeclareScriptingFunction(&inputMouse::lpPosition, "lpPosition");
DeclareScriptingFunction(&inputMouse::mouseLeftTexture, "mouseLeftTexture");
DeclareScriptingFunction(&inputMouse::mouseMiddleTexture, "mouseMiddleTexture");
DeclareScriptingFunction(&inputMouse::mouseRightTexture, "mouseRightTexture");
DeclareScriptingFunction(&inputMouse::reset, "reset");
DeclareScriptingFunction(&inputMouse::scroll, "scroll");
DeclareScriptingFunction(&inputMouse::scrollReset, "scrollReset");
DeclareScriptingFunction(&inputMouse::setDefaultButtonTextures, "setDefaultButtonTextures");