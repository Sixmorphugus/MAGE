#include "viewUi.h"

using namespace mage;

// uiView
viewUi::viewUi()
{
}

viewUi::viewUi(sf::Vector2f size, std::shared_ptr<groupBase> gr)
	: view(size, gr)
{
}

void viewUi::render(sf::RenderTarget & target, sf::Color bgCol)
{
	view::render(target, bgCol);

	// draw all the objects in the group
	sf::RenderStates rs;
	applyShader(rs, sf::Vector2f(internalRT.getSize().x, internalRT.getSize().y));

	if (!group.expired()) {
		auto groupLck = group.lock();

		groupLck->drawUiObjects(internalRT, sf::RenderStates());

		internalRT.display();

		sf::Sprite irtSpr(internalRT.getTexture());

		target.draw(irtSpr, rs);
	}
}

DeclareScriptingView(viewUi);