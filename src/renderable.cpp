#include "renderable.h"

using namespace mage;

renderable::renderable()
{
	m_visible = true;
}

drawRecipe renderable::getDrawRecipe()
{
	onRendered.notify(this);

	auto r = generateDrawRecipe();
	return r;
}

drawRecipe renderable::generateDrawRecipe()
{
	return drawRecipe();
}

void renderable::setIsVisible(bool visible)
{
	m_visible = visible;
}

bool renderable::getIsVisible() const
{
	return m_visible;
}


// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(renderable);
MAGE_DeclareScriptingBaseClass(shadable, renderable);
MAGE_DeclareScriptingConstructor(renderable(), "renderable");
MAGE_DeclareScriptingCopyOperator(renderable);
MAGE_DeclareScriptingFunction(&renderable::setIsVisible, "setIsVisible");
MAGE_DeclareScriptingFunction(&renderable::getIsVisible, "getIsVisible");
MAGE_DeclareScriptingFunction(&renderable::getDrawRecipe, "getDrawRecipe");
MAGE_DeclareScriptingFunction(&renderable::onRendered, "onRendered");

MAGE_DeclareScriptingHook("renderableHook", renderable*);