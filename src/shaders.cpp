#include "shaders.h"

#include "resourceShader.h"
#include "Game.h"

using namespace mage;

shadable::shadable() {
	clearShader();
}

void shadable::setShader(std::shared_ptr<resourceShader> shad) {
	shader = shad;
}

void shadable::clearShader() {
	shader.reset();
}

void shadable::shaderRestartEffect()
{
	auto shL = getShader();

	if (shL)
		return;

	shL->get()->setUniform("clockTimeStart", theGame()->getSimTime().asSeconds());
}

void shadable::shaderUpdate(sf::Vector2f texSize) const
{
	shaderUpdateCustom(getShader()->get().get(), texSize);
}

void shadable::shaderUpdateCustom(sf::Shader* cShader, sf::Vector2f texSize) const
{
	if (!cShader)
		return;

	cShader->setUniform("textureSize", texSize);
	cShader->setUniform("texture", sf::Shader::CurrentTexture);
	cShader->setUniform("clockTime", theGame()->getSimTime().asSeconds());
	cShader->setUniform("delta", theGame()->getDelta());
}

std::shared_ptr<resourceShader> shadable::getShader() const {
	if (shader.expired())
		return nullptr;

	auto shL = shader.lock();

	return shL;
}

void shadable::applyShader(sf::RenderStates& st, sf::Vector2f texSize) const {
	// basically just add our shader to the state if we have one, and update it so it works correctly
	auto sh = getShader();
	if (sh) {
		st.shader = sh->get().get();
		shaderUpdate(texSize);
	}
}

// SE Binding
#include "scriptingEngine.h"
MAGE_DeclareScriptingType(shadable);
MAGE_DeclareScriptingFunction(&shadable::setShader, "setShader");
MAGE_DeclareScriptingFunction(&shadable::getShader, "getShader");
MAGE_DeclareScriptingFunction(&shadable::shaderUpdate, "shaderUpdate");
MAGE_DeclareScriptingFunction(&shadable::clearShader, "clearShader");
MAGE_DeclareScriptingFunction(&shadable::shaderRestartEffect, "shaderRestartEffect");
MAGE_DeclareScriptingFunction(&shadable::shaderUpdate, "shaderUpdate");