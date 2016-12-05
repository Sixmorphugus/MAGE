#pragma once
#include "view.h"

namespace mage {

// objView can see the world objects in a group
class MAGEDLL viewObj : public view {
public:
	viewObj();
	viewObj(sf::Vector2f size, std::shared_ptr<groupBase> gr = nullptr);

	void render(sf::RenderTarget& target, sf::Color bgCol = sf::Color::Transparent);
public:
	bool useStateBounds;
};

}