#pragma once
#include "view.h"

namespace mage {

// viewUi can see the ui objects in a group
class MAGEDLL viewUi : public view {
public:
	viewUi();
	viewUi(sf::Vector2f size, std::shared_ptr<groupBase> gr = nullptr);

	void render(sf::RenderTarget& target, sf::Color bgCol = sf::Color::Transparent);
};

} // namespace mage