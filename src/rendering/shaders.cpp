#include "shaders.h"

#include "resourceShader.h"
#include "Game.h"

using namespace mage;

shadable::shadable() {
	shader.reset();
}

void shadable::shaderRestartEffect()
{
	if (shader.expired())
		return;

	auto shL = shader.lock();
	shL->get()->setUniform("clockTimeStart", theGame()->getSimTime().get());
}

void shadable::shaderUpdate(point2F texSize) const
{
	if (shader.expired())
		return;

	auto shL = shader.lock();
	shaderUpdateCustom(shL->get(), texSize);
}

void shadable::shaderUpdateCustom(sf::Shader* cShader, point2F texSize) const
{
	if (!cShader)
		return;

	cShader->setUniform("textureSize", texSize.toSf2());
	cShader->setUniform("texture", sf::Shader::CurrentTexture); // sfml black magic
	cShader->setUniform("clockTime", theGame()->getSimTime().get());
	cShader->setUniform("delta", theGame()->getDelta());
}


// SE Binding
#include "scriptingEngine.h"
MAGE_DeclareScriptingType(shadable);
MAGE_DeclareScriptingFunction(&shadable::shader, "shader");
MAGE_DeclareScriptingFunction(&shadable::shaderUpdate, "shaderUpdate");
MAGE_DeclareScriptingFunction(&shadable::shaderRestartEffect, "shaderRestartEffect");
MAGE_DeclareScriptingFunction(&shadable::shaderUpdate, "shaderUpdate");