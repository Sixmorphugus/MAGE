#include "textHelpers.h"

#include "Game.h"
#include "platform.h"
#include "helpers.h"
#include "mixer.h"
#include "resources.h"
#include "io.h"

#include "resourceTexture.h"

namespace mage {
// universal draw functions
void renderBindPrompt(sf::RenderTarget & target, sf::RenderStates states, int yPosition, std::vector<std::string> keybindNames, std::string caption)
{
	// find the needed textures
	std::vector<std::shared_ptr<resourceTexture>> textures;

	for (unsigned int i = 0; i < keybindNames.size(); i++) {
		std::vector<std::shared_ptr<resourceTexture>> bindTextures = theGame()->getKeybindTextures(keybindNames[i]);

		for (unsigned int j = 0; j < bindTextures.size(); j++) {
			textures.push_back(bindTextures[j]);
		}
	}

	// calculate x position
	int xPosition = (int)(theGame()->uiCamera->getSize().x) - 64;

	// draw text
	renderInfo(target, states, sf::Vector2f((float)xPosition, (float)yPosition), caption);

	// draw keybind sprites in reverse order
	for (int i = (int)textures.size() - 1; i >= 0; i--) {
		sf::Sprite kbSprite(*textures[i]->get());
		kbSprite.setPosition((float)xPosition - kbSprite.getGlobalBounds().width - 8.f, (float)yPosition - floor(kbSprite.getGlobalBounds().height / 2.f));

		target.draw(kbSprite, states);

		xPosition -= (int)floor(kbSprite.getGlobalBounds().width) - 4;
	}
}

float renderInfo(sf::RenderTarget & target, sf::RenderStates states, sf::Vector2f screenPos, std::string info)
{
	return renderInfo(target, states, screenPos, info, sf::Color::White);
}

float renderInfo(sf::RenderTarget &target, sf::RenderStates states, sf::Vector2f screenPos, std::string info, sf::Color col, bool center, unsigned int charSize, float rot, std::shared_ptr<resourceFont> customFont) {
	std::shared_ptr<resourceFont> defaultFont = theGame()->resources->getAs<resourceFont>("__fontDef");

	if (!defaultFont)
		p::fatal("Font does not exist");

	if (customFont) {
		defaultFont = customFont;
	}

	// secretly round screenPos
	screenPos.x = floor(screenPos.x);
	screenPos.y = floor(screenPos.y);

	// create a message in the default font
	sf::Text shadow;
	shadow.setString(info);
	shadow.setScale(.5f, .5f); // (hotfix)
	shadow.setFont(*defaultFont->get());
	shadow.setCharacterSize(charSize);
	shadow.setPosition((sf::Vector2f)screenPos);
	shadow.setFillColor(sf::Color(50, 50, 80, col.a));
	shadow.setRotation(rot);

	sf::Text main;
	main.setString(info);
	main.setScale(.5f, .5f);
	main.setFont(*defaultFont->get());
	main.setCharacterSize(charSize);
	main.setPosition((sf::Vector2f)screenPos - sf::Vector2f(1.f, 1.f));
	main.setFillColor(col);
	main.setRotation(rot);

	if (center) {
		sf::Vector2f halfSize(main.getGlobalBounds().width / 2, main.getGlobalBounds().height / 2);

		main.move((sf::Vector2f) - halfSize);
		shadow.move((sf::Vector2f) - halfSize);
	}

	target.draw(shadow, states);
	target.draw(main, states);

	return floor(main.getGlobalBounds().width);
}

using namespace chaiscript;

MAGE_DeclareScriptingFunction(&renderBindPrompt, "renderBindPrompt");
MAGE_DeclareScriptingCustom(fun<float, sf::RenderTarget&, sf::RenderStates, sf::Vector2f, std::string>(&renderInfo), "renderInfo");
MAGE_DeclareScriptingCustom(fun<float, sf::RenderTarget&, sf::RenderStates, sf::Vector2f, std::string, sf::Color, bool, unsigned int, float, std::shared_ptr<resourceFont>>(&renderInfo), "renderInfo");

} // namespace mage