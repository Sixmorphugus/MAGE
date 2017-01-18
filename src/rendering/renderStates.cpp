#include "renderStates.h"

#include "Game.h"
#include "resources.h"
#include "resourceShader.h"
#include "resourceTexture.h"

using namespace mage;

const blendMode mage::blendDefault = sf::BlendAlpha;

renderStates::renderStates()
{
	blend = blendDefault;
	texture.reset();
	shader.reset();
	usePage = false;
}

renderStates::renderStates(sf::RenderStates& states)
{
	// slow!
	blend = states.blendMode;
	texture.reset();
	shader.reset();
	usePage = false;

	auto texList = theGame()->resources->listType<resourceTexture>();
	auto shList = theGame()->resources->listType<resourceShader>();

	for (unsigned int i = 0; i < texList.size(); i++) {
		if (states.texture == texList[i]->get()) {
			texture = texList[i];
			break;
		}
	}
	for (unsigned int i = 0; i < shList.size(); i++) {
		if (states.shader == shList[i]->get()) {
			shader = shList[i];
			break;
		}
	}
}

renderStates::renderStates(std::shared_ptr<resourceTexture> tex, std::shared_ptr<resourceShader> sha, blendMode b)
{
	texture = tex;
	shader = sha;
	blend = b;
	usePage = false;
}

sf::RenderStates renderStates::toSf()
{
	sf::RenderStates states;

	states.blendMode = blend;
	states.shader = shader.lock()->get();
	states.texture = texture.lock()->get();
}

bool renderStates::operator==(renderStates & rh)
{
	return (
		blend == rh.blend &&
		usePage == rh.usePage &&
		texture.lock() == rh.texture.lock() &&
		shader.lock() == rh.shader.lock()
	);
}

bool renderStates::operator!=(renderStates & rh)
{
	return !(*this == rh);
}

#include "scriptingEngine.h"
// SE

MAGE_DeclareScriptingType(renderStates);
MAGE_DeclareScriptingConstructor(renderStates(), "renderStates");
MAGE_DeclareScriptingConstructor(renderStates(sf::RenderStates&), "renderStates");
MAGE_DeclareScriptingConstructor(renderStates(std::shared_ptr<resourceTexture>, std::shared_ptr<resourceShader>, blendMode), "renderStates");
MAGE_DeclareScriptingFunction(&renderStates::blend, "blend");
MAGE_DeclareScriptingFunction(&renderStates::texture, "texture");
MAGE_DeclareScriptingFunction(&renderStates::shader, "shader");