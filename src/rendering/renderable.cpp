#include "renderable.h"
#include "Game.h"
#include "batchRenderer.h"

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
		m_cachedDrawRecipe = generateOptimizedDrawRecipe();
		m_dirty = false;
	}

	return &m_cachedDrawRecipe;
}

renderRecipe renderable::generateOptimizedDrawRecipe()
{
	auto r = generateDrawRecipe(); // our base draw recipe. Renderer can draw it, but it will be from its own texture.

	// this only works if we can get the renderer to "page" the object's texture.
	auto t = r.states.texture.lock();

	if (t && theGame()->renderer->pushPageTexture(t)) {
		r.shiftTextureVerts(theGame()->renderer->texturePagePosition(t));
		
		r.states.usePage = true;
	}

	return r;
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