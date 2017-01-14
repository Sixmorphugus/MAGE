#include "gmo.h"

#include "Game.h"
#include "prefabs.h"

using namespace mage;

gmo::gmo(const pointF & position)
	: transformable(position)
{
	// defaults
	m_scene = nullptr;
	m_prefabSource = nullptr;
	m_respectPixelGrid = false;

	// object properties.
	addProperty(std::make_shared<prop<std::string>>("name",
		prop<std::string>::makeGetFunc<renamable>(&getName),
		prop<std::string>::makeSetFunc<renamable>(&setName)
		));

	addProperty(std::make_shared<prop<pointF>>("position",
		prop<pointF>::makeGetFunc<transformable>(&getPosition),
		prop<pointF>::makeSetFunc<transformable>(&setPosition)
		));

	addProperty(std::make_shared<prop<pointF>>("anchor",
		prop<pointF>::makeGetFunc<transformable>(&getAnchor),
		prop<pointF>::makeSetFunc<transformable>(&setAnchor)
		));

	addProperty(std::make_shared<prop<pointF>>("scale",
		prop<pointF>::makeGetFunc<transformable>(&getScale),
		prop<pointF>::makeSetFunc<transformable>(&setScale)
		));

	addProperty(std::make_shared<prop<float>>("rotation",
		prop<float>::makeGetFunc<transformable>(&getRotation),
		prop<float>::makeSetFunc<transformable>(&setRotation)
		));
}

gmo::gmo(const gmo & cp)
{
	copyFrom(cp);
}

gmo & gmo::operator=(const gmo & cp)
{
	copyFrom(cp);
	return *this;
}

void gmo::preUpdate(time elapsed)
{
	if (m_respectPixelGrid) {
		pixelLock();
	}

	onPreUpdate.notify(this, elapsed);
}

void mage::gmo::update(time elapsed)
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

bool gmo::getRespectsPixelGrid()
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
