#pragma once
#include "inputs.h"

namespace mage {

// KEYBOARD
// ---------------------------------------------------
class MAGEDLL inputKeyboard : public input
{
public:
	inputKeyboard();

	class MAGEDLL keybind {
	public:
		enum state {
			RELEASED = -1,
			IDLE = 0,
			DOWN = 1,
			PRESSED = 2
		};

	public:
		keybind();
		keybind(sf::Keyboard::Key singleKey, bool rebindable = false);
		keybind(std::initializer_list<sf::Keyboard::Key> keysIn, bool rebindable = false);

	public:
		hook<inputKeyboard*, keybind*> onPressed;
		hook<inputKeyboard*, keybind*> onReleased;

		std::vector<sf::Keyboard::Key> keys;
		std::vector<bool> keysIn;
		state s = IDLE;
		bool rebindable = false;

		inline bool getDown();
		inline bool getPressed();
		inline bool getReleased();
		inline bool getUp();
		inline bool getIdle();
		void consumeEvent();
	};

	void setBind(std::string bName, keybind& bind);
	void setBind(std::string bName, sf::Keyboard::Key singleKey, bool rebindable = false);
	void setBind(std::string bName, std::initializer_list<sf::Keyboard::Key> keysIn, bool rebindable = false);

	keybind* getBind(std::string bName);
	bool isBind(std::string bName);

	std::vector<std::string> getBindTextures(std::string bName);

	void processEvent(sf::Event evt);
	void update();

	keybind* operator[](std::string bName);

	void setDefaultBindTextures();

public:
	bool keyJustPressed;
	std::string inputStream;

	std::map<sf::Keyboard::Key, std::string> keyTextureCache;

private:
	std::map<std::string, keybind> bindings;
};

bool inputKeyboard::keybind::getDown() {
	return s > 0;
}

bool inputKeyboard::keybind::getPressed() {
	return s == 2;
}

bool inputKeyboard::keybind::getReleased() {
	return s == -1;
}

bool inputKeyboard::keybind::getUp() {
	return s <= 0;
}

bool inputKeyboard::keybind::getIdle() {
	return s == 0;
}

} // namespace mage