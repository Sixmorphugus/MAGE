#pragma once
#include "uiMenu.h"

namespace mage {

// uiPillarMenu
class MAGEDLL uiPillarMenu : public uiMenu
{
public:
	uiPillarMenu(float x, float y);
	uiPillarMenu(float x, float y, std::vector<std::string> opList);

	void update(sf::Time elapsed);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void floatIn();
	void floatOut();

public:
	bool drawLines;
	float targetX;

private:
	bool fi, fo;
};

} // namespace mage