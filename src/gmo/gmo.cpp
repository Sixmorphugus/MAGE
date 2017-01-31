#include "gmo.h"

#include "Game.h"
#include "prefabs.h"

using namespace mage;


// GMO
gmo::gmo()
{
	init();
}

gmo::gmo(const pointF& position)
	: transformableObject(position, pointF())
{
	init();
}

gmo::gmo(const pointF & position, const pointF& size)
	: transformableObject(position, size)
{
	init();
}

gmo::gmo(const gmo & cp)
{
	copyFrom(cp);
}

void gmo::init()
{
	m_scene = nullptr;
	m_prefabSource = nullptr;
	m_respectPixelGrid = false;
}

gmo & gmo::operator=(const gmo & cp)
{
	copyFrom(cp);
	return *this;
}

void gmo::preUpdate(interval elapsed)
{
	if (m_respectPixelGrid) {
		pixelLock();
	}

	onPreUpdate.notify(this, elapsed);
}

void gmo::update(interval elapsed)
{
	onUpdate.notify(this, elapsed);
}

scene* gmo::getScene()
{
	return m_scene;
}

std::shared_ptr<prefab> gmo::getPrefabSource()
{
	if (m_prefabSource)
		return m_prefabSource;

	const std::type_info& t = typeid(*this);
	return theGame()->prefabs->getDefaultPrefab(t);
}

bool gmo::getRespectsPixelGrid() const
{
	return m_respectPixelGrid;
}

void gmo::setRespectsPixelGrid(bool yes)
{
	m_respectPixelGrid = yes;
}

void gmo::moveWithCollision(pointF movement)
{
	move(movement); // temporary
}

unsigned int gmo::addComponent(std::shared_ptr<gmoComponent> newComp)
{
	if (newComp->m_gmo)
		newComp->m_gmo->removeComponent(newComp);

	m_components.push_back(newComp);
	newComp->m_gmo = this;

	return m_components.size() - 1;
}

void gmo::removeComponent(std::shared_ptr<gmoComponent> rComp)
{
	int c = indexOfComponent((rComp));

	if(c >= 0)
		removeComponent((unsigned int)c);
}

void gmo::removeComponent(unsigned int id)
{
	if (id > m_components.size())
		return;

	auto comp = m_components[id];
	comp->m_gmo = nullptr;

	m_components.erase(m_components.begin() + id);
}

int mage::gmo::indexOfComponent(std::shared_ptr<gmoComponent> newComp) const
{
	for (unsigned int i = 0; i < m_components.size(); i++) {
		if (m_components[i] == newComp)
			return i;
	}

	return -1;
}

std::shared_ptr<gmoComponent> mage::gmo::getComponent(unsigned int id)
{
	if (id > m_components.size())
		return nullptr;

	return m_components[id];
}

unsigned int mage::gmo::getNumComponents() const
{
	return m_components.size();
}

renderRecipe gmo::generateDrawRecipe()
{
	auto r = renderRecipe();
	return r;
}

void gmo::ownChildren()
{
	for (unsigned int i = 0; i < m_components.size(); i++) {
		m_components[i]->m_gmo = this;
	}
}

void gmo::copyFrom(const gmo & cp)
{
	m_prefabSource = cp.m_prefabSource;
	m_respectPixelGrid = cp.m_respectPixelGrid;
	m_scene = nullptr;
}

#include "scriptingEngine.h"

using namespace chaiscript;

MAGE_DeclareScriptingGmoTypeG(gmo);
MAGE_DeclareScriptingConstructor(gmo(), "gmo");
MAGE_DeclareScriptingConstructor(gmo(const gmo&), "gmo");
MAGE_DeclareScriptingConstructor(gmo(const pointF&), "gmo");
MAGE_DeclareScriptingFunction(&gmo::operator=, "=");
MAGE_DeclareScriptingFunction(&gmo::preUpdate, "preUpdate");
MAGE_DeclareScriptingFunction(&gmo::update, "update");
MAGE_DeclareScriptingFunction(&gmo::getScene, "getScene");
MAGE_DeclareScriptingFunction(&gmo::getPrefabSource, "getPrefabSource");
MAGE_DeclareScriptingFunction(&gmo::getRespectsPixelGrid, "getRespectsPixelGrid");
MAGE_DeclareScriptingFunction(&gmo::setRespectsPixelGrid, "setRespectsPixelGrid");
MAGE_DeclareScriptingFunction(&gmo::getNumComponents, "getNumComponents");
MAGE_DeclareScriptingFunction(&gmo::addComponent, "addComponent");
MAGE_DeclareScriptingFunction(&gmo::getComponent, "getComponent");
MAGE_DeclareScriptingFunction(&gmo::getNumComponents, "getNumComponents");
MAGE_DeclareScriptingFunction(&gmo::indexOfComponent, "indexOfComponent");
MAGE_DeclareScriptingCustom(fun<void, gmo, unsigned int>(&gmo::removeComponent), "removeComponent");
MAGE_DeclareScriptingCustom(fun<void, gmo, std::shared_ptr<gmoComponent>>(&gmo::removeComponent), "removeComponent");

MAGE_DeclareSerializationBase(gmo);

// GMO COMPONENT
gmoComponent::gmoComponent()
{
	m_gmo = nullptr;
}

void gmoComponent::preUpdate(interval elapsedSinceLastUpdate)
{
	onPreUpdate.notify(this, elapsedSinceLastUpdate);
}

void gmoComponent::update(interval elapsedSinceLastUpdate)
{
	onUpdate.notify(this, elapsedSinceLastUpdate);
}

gmo * mage::gmoComponent::getParent()
{
	return m_gmo;
}

renderRecipe gmoComponent::generateDrawRecipe()
{
	return renderRecipe();
}

MAGE_DeclareScriptingType(gmoComponent);
MAGE_DeclareScriptingBaseClass(renderable, gmoComponent);
MAGE_DeclareScriptingConstructor(gmoComponent(), "gmoComponent");
MAGE_DeclareScriptingConstructor(gmoComponent(const gmoComponent&), "gmoComponent");
MAGE_DeclareScriptingCopyOperator(gmoComponent);
MAGE_DeclareScriptingFunction(&gmoComponent::preUpdate, "preUpdate");
MAGE_DeclareScriptingFunction(&gmoComponent::update, "update");
MAGE_DeclareScriptingFunction(&gmoComponent::getParent, "getParent");

MAGE_DeclareSerializationBase(gmoComponent);