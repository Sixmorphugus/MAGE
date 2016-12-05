#pragma once
#include "uiBasic.h"

namespace mage {

class uiMenu;
class uiBoxMenu;
class uiBoxTextEntry;

class MAGEDLL uiFileBrowser : public uiBasic
{
public:
	uiFileBrowser(float x, float y);

	void update(sf::Time elapsed);
	void draw(sf::RenderTarget& target, sf::RenderStates states);

	bool isReady() { return ready; }
	std::string getInput() { return input; }

	void reset() { ready = false; };

	std::string getPath() { return path; }
	std::string getRoutePath() { return input; }
public:
	std::vector<std::string> extFilter;
	std::string autoExt;

private:
	bool ready;
	std::string input;
	std::string path;

	uiBoxMenu* fbMenu;
	uiMenu* opMenu;
	uiBoxTextEntry* fbTextEntry;
};

} // namespace mage