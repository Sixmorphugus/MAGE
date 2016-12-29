#include "inputKeyboard.h"

using namespace mage;

std::map<std::string, inputKeyboard::keybind> bindingsDefault;

// KEYBOARD
// ---------------------------------------------------
inputKeyboard::inputKeyboard() {
	// set some defeaults
	keyJustPressed = false;
	inputStream = "";
}

void inputKeyboard::processEvent(sf::Event evt)
{
	if (evt.type == sf::Event::KeyPressed) {
		keyJustPressed = true;

		for (auto i = bindings.begin(); i != bindings.end(); i++) {
			unsigned int keysDown = 0;
			for (unsigned int k = 0; k < i->second.keys.size(); k++) {
				// i'm too young for this
				if (evt.key.code == i->second.keys[k]) {
					i->second.keysIn[k] = true;
				}

				if (i->second.keysIn[k]) {
					keysDown++;
				}
			}

			// is this binding ready to activate?
			if (keysDown >= i->second.keys.size() && i->second.getUp()) {
				i->second.s = keybind::PRESSED;
				i->second.onPressed.notify(this, &i->second);
				//std::cout << i->first << std::endl;
			}
		}
	}
	else if (evt.type == sf::Event::KeyReleased)
	{
		for (auto i = bindings.begin(); i != bindings.end(); i++) {
			unsigned int keysDown = 0;
			for (unsigned int k = 0; k < i->second.keys.size(); k++) {
				// i'm too young for this
				if (evt.key.code == i->second.keys[k]) {
					i->second.keysIn[k] = false;
				}

				if (i->second.keysIn[k]) {
					keysDown++;
				}
			}

			// is this binding ready to deactivate?
			if (keysDown < i->second.keys.size() && i->second.getDown()) {
				i->second.s = keybind::RELEASED;
				i->second.onReleased.notify(this, &i->second);
			}
		}
	}
	else if (evt.type == sf::Event::TextEntered) {
		inputStream = (char)evt.text.unicode;
	}
}

void inputKeyboard::update() {
	keyJustPressed = false;
	inputStream = "";

	for (auto i = bindings.begin(); i != bindings.end(); i++) {
		i->second.consumeEvent();
	}
}

void inputKeyboard::setBind(std::string bName, keybind& bind)
{
	bindings[bName] = bind;
}

void inputKeyboard::setBind(std::string bName, sf::Keyboard::Key singleKey, bool rebindable) {
	keybind nkb(singleKey, rebindable);
	bindings[bName] = nkb;
}

void inputKeyboard::setBind(std::string bName, std::initializer_list<sf::Keyboard::Key> keysIn, bool rebindable) {
	keybind nkb(keysIn, rebindable);
	bindings[bName] = nkb;
}

inputKeyboard::keybind* inputKeyboard::getBind(std::string name) {
	if (isBind(name)) {
		return &bindings[name];
	}
	else {
		return nullptr;
	}
}

inputKeyboard::keybind* inputKeyboard::operator[](std::string name)
{
	if (isBind(name)) {
		return &bindings[name];
	}
	else {
		bindings[name] = keybind(); // create a new one and return that
		return &bindings[name];
	}
}

void inputKeyboard::setDefaultBindTextures()
{
	keyTextureCache[sf::Keyboard::A] = "ui_input_keyA";
	keyTextureCache[sf::Keyboard::B] = "ui_input_keyB";
	keyTextureCache[sf::Keyboard::C] = "ui_input_keyC";
	keyTextureCache[sf::Keyboard::D] = "ui_input_keyD";
	keyTextureCache[sf::Keyboard::E] = "ui_input_keyE";
	keyTextureCache[sf::Keyboard::F] = "ui_input_keyF";
	keyTextureCache[sf::Keyboard::G] = "ui_input_keyG";
	keyTextureCache[sf::Keyboard::H] = "ui_input_keyH";
	keyTextureCache[sf::Keyboard::I] = "ui_input_keyI";
	keyTextureCache[sf::Keyboard::J] = "ui_input_keyJ";
	keyTextureCache[sf::Keyboard::K] = "ui_input_keyK";
	keyTextureCache[sf::Keyboard::L] = "ui_input_keyL";
	keyTextureCache[sf::Keyboard::M] = "ui_input_keyM";
	keyTextureCache[sf::Keyboard::N] = "ui_input_keyN";
	keyTextureCache[sf::Keyboard::O] = "ui_input_keyO";
	keyTextureCache[sf::Keyboard::P] = "ui_input_keyP";
	keyTextureCache[sf::Keyboard::Q] = "ui_input_keyQ";
	keyTextureCache[sf::Keyboard::R] = "ui_input_keyR";
	keyTextureCache[sf::Keyboard::S] = "ui_input_keyS";
	keyTextureCache[sf::Keyboard::T] = "ui_input_keyT";
	keyTextureCache[sf::Keyboard::U] = "ui_input_keyU";
	keyTextureCache[sf::Keyboard::V] = "ui_input_keyV";
	keyTextureCache[sf::Keyboard::W] = "ui_input_keyW";
	keyTextureCache[sf::Keyboard::X] = "ui_input_keyX";
	keyTextureCache[sf::Keyboard::Y] = "ui_input_keyY";
	keyTextureCache[sf::Keyboard::Z] = "ui_input_keyZ";
	keyTextureCache[sf::Keyboard::Num0] = "ui_input_keyNum0";
	keyTextureCache[sf::Keyboard::Num1] = "ui_input_keyNum1";
	keyTextureCache[sf::Keyboard::Num2] = "ui_input_keyNum2";
	keyTextureCache[sf::Keyboard::Num3] = "ui_input_keyNum3";
	keyTextureCache[sf::Keyboard::Num4] = "ui_input_keyNum4";
	keyTextureCache[sf::Keyboard::Num5] = "ui_input_keyNum5";
	keyTextureCache[sf::Keyboard::Num6] = "ui_input_keyNum6";
	keyTextureCache[sf::Keyboard::Num7] = "ui_input_keyNum7";
	keyTextureCache[sf::Keyboard::Num8] = "ui_input_keyNum8";
	keyTextureCache[sf::Keyboard::Num9] = "ui_input_keyNum9";
	keyTextureCache[sf::Keyboard::Numpad0] = "ui_input_keyNum0";
	keyTextureCache[sf::Keyboard::Numpad1] = "ui_input_keyNum1";
	keyTextureCache[sf::Keyboard::Numpad2] = "ui_input_keyNum2";
	keyTextureCache[sf::Keyboard::Numpad3] = "ui_input_keyNum3";
	keyTextureCache[sf::Keyboard::Numpad4] = "ui_input_keyNum4";
	keyTextureCache[sf::Keyboard::Numpad5] = "ui_input_keyNum5";
	keyTextureCache[sf::Keyboard::Numpad6] = "ui_input_keyNum6";
	keyTextureCache[sf::Keyboard::Numpad7] = "ui_input_keyNum7";
	keyTextureCache[sf::Keyboard::Numpad8] = "ui_input_keyNum8";
	keyTextureCache[sf::Keyboard::Numpad9] = "ui_input_keyNum9";
	keyTextureCache[sf::Keyboard::Escape] = "ui_input_keyEscape";
	keyTextureCache[sf::Keyboard::LControl] = "ui_input_keyLControl";
	keyTextureCache[sf::Keyboard::LShift] = "ui_input_keyLShift";
	keyTextureCache[sf::Keyboard::LAlt] = "ui_input_keyLAlt";
	keyTextureCache[sf::Keyboard::LBracket] = "ui_input_keyLBracket";
	keyTextureCache[sf::Keyboard::RBracket] = "ui_input_keyRBracket";
	keyTextureCache[sf::Keyboard::SemiColon] = "ui_input_keySemiColon";
	keyTextureCache[sf::Keyboard::Comma] = "ui_input_keyComma";
	keyTextureCache[sf::Keyboard::Period] = "ui_input_keyPeriod";
	keyTextureCache[sf::Keyboard::Quote] = "ui_input_keyQuote";
	keyTextureCache[sf::Keyboard::Slash] = "ui_input_keySlash";
	keyTextureCache[sf::Keyboard::BackSlash] = "ui_input_keyBackSlash";
	keyTextureCache[sf::Keyboard::Tilde] = "ui_input_keyTilde";
	keyTextureCache[sf::Keyboard::Equal] = "ui_input_keyEqual";
	keyTextureCache[sf::Keyboard::Dash] = "ui_input_keyDash";
	keyTextureCache[sf::Keyboard::Space] = "ui_input_keySpace";
	keyTextureCache[sf::Keyboard::Return] = "ui_input_keyReturn";
	keyTextureCache[sf::Keyboard::Tab] = "ui_input_keyTab";
	keyTextureCache[sf::Keyboard::PageUp] = "ui_input_keyPageUp";
	keyTextureCache[sf::Keyboard::PageDown] = "ui_input_keyPageDown";
	keyTextureCache[sf::Keyboard::End] = "ui_input_keyEnd";
	keyTextureCache[sf::Keyboard::Home] = "ui_input_keyHome";
	keyTextureCache[sf::Keyboard::Insert] = "ui_input_keyInsert";
	keyTextureCache[sf::Keyboard::Delete] = "ui_input_keyDelete";
	keyTextureCache[sf::Keyboard::Add] = "ui_input_keyAdd";
	keyTextureCache[sf::Keyboard::Multiply] = "ui_input_keyMultiply";
	keyTextureCache[sf::Keyboard::Left] = "ui_input_keyLeft";
	keyTextureCache[sf::Keyboard::Right] = "ui_input_keyRight";
	keyTextureCache[sf::Keyboard::Up] = "ui_input_keyUp";
	keyTextureCache[sf::Keyboard::Down] = "ui_input_keyDown";
	keyTextureCache[sf::Keyboard::F1] = "ui_input_keyF1";
	keyTextureCache[sf::Keyboard::F2] = "ui_input_keyF2";
	keyTextureCache[sf::Keyboard::F3] = "ui_input_keyF3";
	keyTextureCache[sf::Keyboard::F4] = "ui_input_keyF4";
	keyTextureCache[sf::Keyboard::F5] = "ui_input_keyF5";
	keyTextureCache[sf::Keyboard::F6] = "ui_input_keyF6";
	keyTextureCache[sf::Keyboard::F7] = "ui_input_keyF7";
	keyTextureCache[sf::Keyboard::F8] = "ui_input_keyF8";
	keyTextureCache[sf::Keyboard::F9] = "ui_input_keyF9";
	keyTextureCache[sf::Keyboard::F10] = "ui_input_keyF10";
	keyTextureCache[sf::Keyboard::F11] = "ui_input_keyF11";
	keyTextureCache[sf::Keyboard::F12] = "ui_input_keyF12";
	keyTextureCache[sf::Keyboard::F13] = "ui_input_keyF13";
	keyTextureCache[sf::Keyboard::F14] = "ui_input_keyF14";
	keyTextureCache[sf::Keyboard::F15] = "ui_input_keyF15";
	keyTextureCache[sf::Keyboard::Unknown] = "ui_input_keyUnknown";

	// I had an algorythm write this
	// Can you blame me?
}

bool inputKeyboard::isBind(std::string name) {
	return (bindings.count(name) != 0);
}

std::vector<std::string> inputKeyboard::getBindTextures(std::string bName)
{
	std::vector<std::string> textureList;
	keybind* kb = getBind(bName);

	if (kb != nullptr) {
		std::vector<sf::Keyboard::Key> keys = kb->keys;

		for (unsigned int i = 0; i < keys.size(); i++) {
			sf::Keyboard::Key key = keys[i];

			if (keyTextureCache.count(key) != 0) { // don't include keys with no bound texture
				textureList.push_back(keyTextureCache[key]);
			}
			else {
				p::warn("No texture found for key " + std::to_string((int)key) + "! UI will display unknown.");
				textureList.push_back(keyTextureCache[sf::Keyboard::Unknown]);
			}
		}
	}

	return textureList;
}

inputKeyboard::keybind::keybind() {
	// ...
}

inputKeyboard::keybind::keybind(sf::Keyboard::Key singleKey, bool r) {
	keys.push_back(singleKey);
	keysIn.push_back(false);
	s = keybind::IDLE;
	rebindable = r;
}

inputKeyboard::keybind::keybind(std::vector<sf::Keyboard::Key> keysInA, bool r) {
	keys = keysInA;

	for (unsigned int i = 0; i < keys.size(); i++) {
		keysIn.push_back(false);
	}

	s = keybind::IDLE;
	rebindable = r;
}

void inputKeyboard::keybind::consumeEvent() {
	if (s == keybind::PRESSED)
		s = keybind::DOWN;

	if (s == keybind::RELEASED)
		s = keybind::IDLE;
}

// SE
#include "scriptingEngine.h"
using namespace mage;
using namespace chaiscript;

MAGE_DeclareScriptingEnum(inputKeyboard::keybind::state, DOWN);
MAGE_DeclareScriptingEnum(inputKeyboard::keybind::state, IDLE);
MAGE_DeclareScriptingEnum(inputKeyboard::keybind::state, PRESSED);
MAGE_DeclareScriptingEnum(inputKeyboard::keybind::state, RELEASED);

MAGE_DeclareScriptingTypeNamed(inputKeyboard::keybind, "keybind");
MAGE_DeclareScriptingConstructor(inputKeyboard::keybind(), "keybind");
MAGE_DeclareScriptingConstructor(inputKeyboard::keybind(std::initializer_list<sf::Keyboard::Key>, bool), "keybind");
MAGE_DeclareScriptingFunction(&inputKeyboard::keybind::getDown, "getDown");
MAGE_DeclareScriptingFunction(&inputKeyboard::keybind::getIdle, "getIdle");
MAGE_DeclareScriptingFunction(&inputKeyboard::keybind::getPressed, "getPressed");
MAGE_DeclareScriptingFunction(&inputKeyboard::keybind::getReleased, "getReleased");
MAGE_DeclareScriptingFunction(&inputKeyboard::keybind::keys, "keys");
MAGE_DeclareScriptingFunction(&inputKeyboard::keybind::keysIn, "keysIn");
MAGE_DeclareScriptingFunction(&inputKeyboard::keybind::rebindable, "rebindable");
MAGE_DeclareScriptingFunction(&inputKeyboard::keybind::s, "state");

MAGE_DeclareScriptingType(inputKeyboard);
MAGE_DeclareScriptingConstructor(inputKeyboard(), "inputKeyboard");
MAGE_DeclareScriptingBaseClass(input, inputKeyboard);
MAGE_DeclareScriptingFunction(&inputKeyboard::getBind, "getBind");
MAGE_DeclareScriptingFunction(&inputKeyboard::getBindTextures, "getBindTextures");
MAGE_DeclareScriptingFunction(&inputKeyboard::inputStream, "inputStream");
MAGE_DeclareScriptingFunction(&inputKeyboard::isBind, "isBind");
MAGE_DeclareScriptingFunction(&inputKeyboard::keyJustPressed, "keyJustPressed");
MAGE_DeclareScriptingCustom(fun<void, inputKeyboard, std::string, std::initializer_list<sf::Keyboard::Key>, bool>(&inputKeyboard::setBind), "setBind");
MAGE_DeclareScriptingCustom(fun<void, inputKeyboard, std::string, sf::Keyboard::Key, bool>(&inputKeyboard::setBind), "setBind");
MAGE_DeclareScriptingCustom(fun<void, inputKeyboard, std::string, inputKeyboard::keybind&>(&inputKeyboard::setBind), "setBind");
MAGE_DeclareScriptingFunction(&inputKeyboard::setDefaultBindTextures, "setDefaultBindTextures");

mage::inputKeyboardBindRegistration::inputKeyboardBindRegistration(std::string name, std::vector<sf::Keyboard::Key> keys, bool rebindable)
{
	bindingsDefault[name] = inputKeyboard::keybind(keys, rebindable);
}

mage::inputKeyboardBindRegistration::inputKeyboardBindRegistration(std::string name, sf::Keyboard::Key key, bool rebindable)
{
	bindingsDefault[name] = inputKeyboard::keybind(key, rebindable);
}

// default keys
MAGE_DeclareInputKeyboardBind("CTRL", sf::Keyboard::LControl);
MAGE_DeclareInputKeyboardBind("SHIFT", sf::Keyboard::LShift);

MAGE_DeclareInputKeyboardBind("CUT", { sf::Keyboard::LControl, sf::Keyboard::X });
MAGE_DeclareInputKeyboardBind("COPY", { sf::Keyboard::LControl, sf::Keyboard::C });
MAGE_DeclareInputKeyboardBind("PASTE", { sf::Keyboard::LControl, sf::Keyboard::V });

MAGE_DeclareInputKeyboardBind("DELETE", sf::Keyboard::Delete);
MAGE_DeclareInputKeyboardBind("ESCAPE", sf::Keyboard::Escape);
MAGE_DeclareInputKeyboardBind("ENTER", sf::Keyboard::Return);
MAGE_DeclareInputKeyboardBind("TILDE", sf::Keyboard::Tilde);
