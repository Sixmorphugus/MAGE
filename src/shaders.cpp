#include "shaders.h"

#include "resourceShader.h"
#include "Game.h"

using namespace mage;

shadable::shadable() {

}

void shadable::shaderRestartEffect()
{
	auto shL = shader.lock();

	if (shL)
		return;

	shL->get()->setUniform("clockTimeStart", theGame()->getSimTime().asSeconds());
}

void shadable::shaderUpdate(point2F texSize) const
{
	shaderUpdateCustom(shader.lock()->get().get(), texSize);
}

void shadable::shaderUpdateCustom(sf::Shader* cShader, point2F texSize) const
{
	if (!cShader)
		return;

	cShader->setUniform("textureSize", texSize.toSf2());
	cShader->setUniform("texture", sf::Shader::CurrentTexture); // sfml black magic
	cShader->setUniform("clockTime", theGame()->getSimTime().asSeconds());
	cShader->setUniform("delta", theGame()->getDelta());
}


// SE Binding
#include "scriptingEngine.h"
MAGE_DeclareScriptingType(shadable);
MAGE_DeclareScriptingFunction(&shadable::shader, "shader");
MAGE_DeclareScriptingFunction(&shadable::shaderUpdate, "shaderUpdate");
MAGE_DeclareScriptingFunction(&shadable::shaderRestartEffect, "shaderRestartEffect");
MAGE_DeclareScriptingFunction(&shadable::shaderUpdate, "shaderUpdate");