#include "viewObj.h"
#include "gameState.h"

using namespace mage;

// worldView
viewObj::viewObj()
{
	useStateBounds = true;
}

viewObj::viewObj(sf::Vector2f size, std::shared_ptr<groupBase> gr)
	: view(size, gr)
{
	useStateBounds = true;
}

void viewObj::render(sf::RenderTarget & target, sf::Color bgCol)
{
	// if the group we're viewing is a gameState, we can use its bounds (though this can be overidden)
	if (useStateBounds) {
		auto gs = std::dynamic_pointer_cast<gameState>(group.lock());

		if (gs) {
			cameraLimits = gs->mapBounds;
		}
	}

	view::render(target, bgCol); // this is called here so the pixel grid respect still works after moving the camera

	// draw all the objects in the group
	sf::RenderStates rs;
	applyShader(rs, sf::Vector2f(internalRT.getSize().x, internalRT.getSize().y));

	if (!group.expired()) {
		auto groupLck = group.lock();

		groupLck->drawWorldObjects(internalRT, sf::RenderStates());

		internalRT.display();

		sf::Sprite irtSpr((internalRT.getTexture()));
		target.draw(irtSpr, rs);
	}
}

MAGE_DeclareScriptingView(viewObj);
MAGE_DeclareScriptingFunction(&viewObj::useStateBounds, "useStateBounds");