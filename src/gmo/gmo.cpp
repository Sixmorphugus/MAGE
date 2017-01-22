#include "gmo.h"

#include "Game.h"
#include "prefabs.h"

using namespace mage;

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
	test = false;

	// object properties.
	addProperty(std::make_shared<prop<std::string>>("name", MAGE_PropLambdasFromMethods(std::string, gmo, setName, getName)));
	addProperty(std::make_shared<prop<pointF>>("position", MAGE_PropLambdasFromMethods(pointF, gmo, setPosition, getPosition)));
	addProperty(std::make_shared<prop<pointF>>("anchor", MAGE_PropLambdasFromMethods(pointF, gmo, setAnchor, getAnchor)));
	addProperty(std::make_shared<prop<pointF>>("scale", MAGE_PropLambdasFromMethods(pointF, gmo, setScale, getScale)));
	addProperty(std::make_shared<prop<float>>("rotation", MAGE_PropLambdasFromMethods(float, gmo, setRotation, getRotation)));
	addProperty(std::make_shared<prop<bool>>("respect pixel grid", MAGE_PropLambdasFromMethods(bool, gmo, setRespectsPixelGrid, getRespectsPixelGrid)));

	addProperty(std::make_shared<prop<bool>>("test", MAGE_PropLambdasFromMember(bool, gmo, test))); // test
}

bool gmo::isCloneable()
{
	return (typeid(*this) == typeid(*clone().get()));
}

std::shared_ptr<gmo> gmo::clone()
{
	return std::make_shared<gmo>(*this);
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

void gmo::copyFrom(const gmo & cp)
{
	m_prefabSource = cp.m_prefabSource;
	m_respectPixelGrid = cp.m_respectPixelGrid;
	m_scene = nullptr;
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingGmoType(gmo);
MAGE_DeclareScriptingConstructor(gmo(), "gmo");
MAGE_DeclareScriptingConstructor(gmo(const gmo&), "gmo");
MAGE_DeclareScriptingConstructor(gmo(const pointF&), "gmo");
MAGE_DeclareScriptingFunction(&gmo::isCloneable, "isClonable");
MAGE_DeclareScriptingFunction(&gmo::clone, "clone");
MAGE_DeclareScriptingFunction(&gmo::operator=, "=");
MAGE_DeclareScriptingFunction(&gmo::preUpdate, "preUpdate");
MAGE_DeclareScriptingFunction(&gmo::update, "update");
MAGE_DeclareScriptingFunction(&gmo::getScene, "getScene");
MAGE_DeclareScriptingFunction(&gmo::getPrefabSource, "getPrefabSource");
MAGE_DeclareScriptingFunction(&gmo::getRespectsPixelGrid, "getRespectsPixelGrid");
MAGE_DeclareScriptingFunction(&gmo::setRespectsPixelGrid, "setRespectsPixelGrid");