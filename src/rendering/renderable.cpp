#include "renderable.h"

using namespace mage;

renderable::renderable()
{
	m_visible = true;
	m_dirty = true;
}

renderRecipe* renderable::getDrawRecipe()
{
	onRendered.notify(this);

	if (m_dirty) {
		m_cachedDrawRecipe = generateDrawRecipe();
		m_dirty = false;
	}

	return &m_cachedDrawRecipe;
}

renderRecipe renderable::generateDrawRecipe()
{
	return renderRecipe();
}

void renderable::setIsVisible(bool visible)
{
	m_visible = visible;
}

void mage::renderable::makeDirty()
{
	m_dirty = true;
}

bool renderable::getIsVisible() const
{
	return m_visible;
}


// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(renderable);
MAGE_DeclareScriptingConstructor(renderable(), "renderable");
MAGE_DeclareScriptingCopyOperator(renderable);
MAGE_DeclareScriptingFunction(&renderable::setIsVisible, "setIsVisible");
MAGE_DeclareScriptingFunction(&renderable::getIsVisible, "getIsVisible");
MAGE_DeclareScriptingFunction(&renderable::getDrawRecipe, "getDrawRecipe");
MAGE_DeclareScriptingFunction(&renderable::onRendered, "onRendered");

MAGE_DeclareScriptingHook("renderableHook", renderable*);